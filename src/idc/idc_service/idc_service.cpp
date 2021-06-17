#include "idc_service/idc_service.h"

#include "vbh/common/vbh_encrypt_lib.h"
#include "vbh/common/vbh_comm_func.h"
#include "vbh/common/vbh_comm_error_code.h"
#include "vbh/sdk/common/send_vbh_msg.h"

#include "dsc/dsc_comm_def.h"

template<typename TYPE>
ACE_INT32 SendVbhSdkServiceMessage(TYPE& t)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetNodeType(CDscAppManager::m_nNodeType);
	addr.SetNodeID(CDscAppManager::m_nNodeID);
	addr.SetServiceType(VBH::EN_VBH_CLIENT_SERVICE_TYPE);
	addr.SetServiceID(1);

	//return VBH_SDK::SendVbhMessage(t, addr);
	return 0;
}

template<typename TYPE>
void Encode(TYPE& rCode, DSC::CDscBlob& blob)
{
	char* data = NULL;
	size_t dataLen = 0;

	DSC::Encode(rCode, data, dataLen);
	blob.Set(data, dataLen);
}

CIdcService::CIdcService(const CDscString& strIpAddr, const ACE_INT32 nPort, const ACE_INT32 nShareTime) : m_strIpAddr(strIpAddr)
, m_nPort(nPort)
, m_nShareTime(nShareTime)
{
}

CIdcService::~CIdcService()
{
}

ACE_INT32 CIdcService::OnInit(void)
{
	

	//VBH_SDK::InitClientSdk(0, m_vbhCallback);

	//VBH::CInitBcUserSdk initBcUserSdk;

	//initBcUserSdk.m_pCallBack = pCallBack;

	//return ::SendSdkServiceMessage(initBcUserSdk);

	m_nRandSeed = ACE_OS::time(0);

	if (LoadDbAddrMap())
	{
		DSC_RUN_LOG_ERROR("idc service load db addr failed!");
		return -1;
	}

	if (CDscHtsServerService::OnInit())
	{
		DSC_RUN_LOG_ERROR("idc service init failed!");
		return -1;
	}

	m_pAcceptor = DSC_THREAD_TYPE_NEW(CMcpAsynchAcceptor<CIdcService>) CMcpAsynchAcceptor<CIdcService>(*this);
	if (m_pAcceptor->Open(m_nPort, m_strIpAddr.c_str()))
	{
		DSC_THREAD_TYPE_DEALLOCATE(m_pAcceptor);
		m_pAcceptor = NULL;
		DSC_RUN_LOG_ERROR("acceptor failed, ip addr:%s, port:%d", "127.0.0.1", 9999);

		return -1;
	}
	else
	{
		this->RegistHandler(m_pAcceptor, ACE_Event_Handler::ACCEPT_MASK);
	}

	return 0;
}

ACE_INT32 CIdcService::OnExit(void)
{
	return ACE_INT32();
}


ACE_INT32 CIdcService::OnHtsMsg(IDC::CIdcCommonCltSrvReq& rCommonReq, CMcpHandler* pMcpHandler)
{
	DSC_RUN_LOG_INFO("CIdcCommonCltSrvReq");
	int nDecryptedBufLen;
	char* pDecryptedBuf;
	if (rCommonReq.m_nType < IDC::EN_IDC_PUB_CRYPTO_KEY_MSG)
	{
		pDecryptedBuf = VBH::vbhDecrypt((unsigned char*)m_peerEnvelopeKey.data(), nDecryptedBufLen, rCommonReq.m_info.GetBuffer(), rCommonReq.m_info.GetSize());
	}
	else
	{
		auto clientSession = m_mapClientSession.Find(rCommonReq.m_nSessionID);
		if (!clientSession)
		{
			return -1;
		}
		pDecryptedBuf = VBH::vbhDecrypt((unsigned char*)clientSession->m_envelopeKey.data(), nDecryptedBufLen, rCommonReq.m_info.GetBuffer(), rCommonReq.m_info.GetSize());
	}

	if (DSC_LIKELY(pDecryptedBuf))
	{
		DSC::CDscBlob data;
		DSC::CDscBlob nonce;
		IDC::CIdcCommonReqDataWrapper wrapper(data, nonce);
		
		DSC::Decode(wrapper, pDecryptedBuf, nDecryptedBufLen);

		ACE_INT32 ret = TypeProc(rCommonReq.m_nType, rCommonReq.m_nReqID, data, pMcpHandler, nonce, rCommonReq.m_nSessionID);

		if (ret)
		{
			// 出错返回错误相应
			return -1;
		}

		DSC_THREAD_FREE(pDecryptedBuf);
	}
	return 0;
}

ACE_INT32 CIdcService::OnTypeProc(IDC::CIdcRegisterUserCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID)
{
	CDscString userKey("");
	VBH::CRegistUserCltCltReq vbhReq;

	userKey += req.m_nId;
	vbhReq.m_bWaitNotify = true;
	vbhReq.m_nCcID = 0;					// todo
	vbhReq.m_nChannelID = 0;			// todo 
	vbhReq.m_nSrcSessionID = m_nReqId;
	vbhReq.m_userKey.Set(userKey.data(), userKey.size());
	vbhReq.m_userInfo.Set(req.m_info.GetBuffer(), req.m_info.GetSize());

	CRegisterSession* regSession = DSC_THREAD_TYPE_NEW(CRegisterSession) CRegisterSession(*this);

	regSession->m_nReqID = m_nReqId;
	regSession->m_nClientReqID = reqID;
	regSession->m_nSessionId = nClientSessionID;
	VBH::Assign(regSession->m_nonce, nonce);
	regSession->m_nHandleID = pMcpHandler->GetHandleID();

	regSession->m_nUid = req.m_nId;

	this->SetDscTimer(regSession, CIdcService::EN_SESSION_TIMEOUT_VALUE);
	m_mapReqSession.Insert(m_nReqId, regSession);

	m_nReqId++;

	return SendVbhSdkServiceMessage(vbhReq);
}

void CIdcService::OnDscMsg(VBH::CRegistUserCltCltRsp& rRegistUserRsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	auto pSession = m_mapReqSession.Erase(rRegistUserRsp.m_nSrcSessionID);

	if (!pSession)
	{
		DSC_RUN_LOG_ERROR("receive vbh register user rsp, but no session");
		return;
	}

	if (pSession->m_nType != IDC::CIdcRegisterUserCltSrvReq::EN_MSG_ID)
	{
		DSC_RUN_LOG_ERROR(" session type error");
		return;
	}

	auto pRegSession = (CRegisterSession*)pSession;

	IDC::CIdcRegisterUserSrvCltRsp rsp;

	rsp.m_nId = pRegSession->m_nUid;
	if (rRegistUserRsp.m_nReturnCode)
	{
	}
	else
	{
		rsp.m_cryptoKey.Set(rRegistUserRsp.m_cryptUserKey.GetBuffer(), rRegistUserRsp.m_cryptUserKey.GetSize());
	}

	char* data = NULL;
	size_t dataLen = 0;
	DSC::CDscBlob blob;

	DSC::Encode(rsp, data, dataLen);
	blob.Set(data, dataLen);

	SendClientMsg(rRegistUserRsp.m_nReturnCode, IDC::CIdcRegisterUserSrvCltRsp::EN_MSG_ID, blob, m_peerEnvelopeKey, pSession);

	DSC_THREAD_TYPE_DELETE(pRegSession);
	DSC_THREAD_SIZE_FREE(data, dataLen);
}

ACE_INT32 CIdcService::OnTypeProc(IDC::CIdcLoginCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID)
{
	VBH::CQueryUserInfoCltCltReq vbhReq;

	vbhReq.m_cryptUserKey.Set(req.m_cryptoKey.GetBuffer(), req.m_cryptoKey.GetSize());
	vbhReq.m_nSrcSessionID = m_nReqId;

	CLoginSession* loginSession = DSC_THREAD_TYPE_NEW(CLoginSession) CLoginSession(*this);

	loginSession->m_nReqID = m_nReqId;
	loginSession->m_nClientReqID = reqID;
	loginSession->m_nSessionId = nClientSessionID;
	VBH::Assign(loginSession->m_nonce, nonce);
	loginSession->m_nHandleID = pMcpHandler->GetHandleID();

	loginSession->m_nUid = req.m_nId;
	VBH::Assign(loginSession->m_cryptoKey, req.m_cryptoKey);

	this->SetDscTimer(loginSession, CIdcService::EN_SESSION_TIMEOUT_VALUE);
	m_mapReqSession.Insert(m_nReqId, loginSession);

	m_nReqId++;
	
	return SendVbhSdkServiceMessage(vbhReq);
}

void CIdcService::OnDscMsg(VBH::CQueryUserInfoCltCltRsp& rQueryUserRsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_UINT32 nReturnCode = VBH::EN_OK_TYPE;
	auto pSession = m_mapReqSession.Erase(rQueryUserRsp.m_nSrcSessionID);

	if (!pSession)
	{
		DSC_RUN_LOG_ERROR("receive vbh register user rsp, but no session");
		return;
	}

	DSC::CDscBlob data;
	if (pSession->m_nType == IDC::CIdcLoginCltSrvReq::EN_MSG_ID)
	{
		auto loginSession = (CLoginSession*)pSession;
		if (rQueryUserRsp.m_nReturnCode == VBH::EN_OK_TYPE)
		{
			auto clientSession = NewClientSession();

			if (clientSession)
			{
				clientSession->m_envelopeKey = loginSession->m_cryptoKey;
				clientSession->m_nHandleID = loginSession->m_nHandleID;

				IDC::CIdcLoginSrvCltRsp rsp;

				rsp.m_nSessionId = clientSession->m_nSessionId;
				rsp.m_nToken = clientSession->m_nToken;

				Encode(rsp, data);

				SendClientMsg(VBH::EN_OK_TYPE, IDC::CIdcLoginCltSrvReq::EN_MSG_ID, data, clientSession->m_envelopeKey, pSession);
				DSC_THREAD_TYPE_DEALLOCATE(pSession);
				DSC_THREAD_SIZE_FREE(data.GetBuffer(), data.GetSize());

				this->SetDscTimer(clientSession, CIdcService::EN_CLIENT_SESSION_REFRESH_VALUE);
				m_mapClientSession.Insert(clientSession->m_nSessionId, clientSession);

				return;
			}
		}
		SendClientMsg(VBH::EN_OK_TYPE, IDC::CIdcLoginCltSrvReq::EN_MSG_ID, data, m_peerEnvelopeKey, pSession);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);
	}
	else if (pSession->m_nType == IDC::CIdcQueryUserInfoCltSrvReq::EN_MSG_ID)
	{
		auto querySession = (CQueryUserInfoSession*)pSession;
		auto clientSession = m_mapClientSession.Find(querySession->m_nSessionId);

		if (!clientSession)
		{
			DSC_THREAD_TYPE_DEALLOCATE(pSession);
			return ;
		}

		if (rQueryUserRsp.m_nReturnCode == VBH::EN_OK_TYPE)
		{
			IDC::CIdcQueryUserInfoSrvCltRsp rsp;

			rsp.m_nToken = MaybeSessionTokenExpire(clientSession);
			rsp.m_info.Set(rQueryUserRsp.m_userInfo.GetBuffer(), rQueryUserRsp.m_userInfo.GetSize());
			Encode(rsp, data);

			SendClientMsg(VBH::EN_OK_TYPE, IDC::CIdcLoginCltSrvReq::EN_MSG_ID, data, clientSession->m_envelopeKey, pSession);
			DSC_THREAD_TYPE_DEALLOCATE(pSession);
			DSC_THREAD_SIZE_FREE(data.GetBuffer(), data.GetSize());

			this->ResetDscTimer(clientSession, CIdcService::EN_CLIENT_SESSION_REFRESH_VALUE);

			return;
		}
		SendClientMsg(VBH::EN_OK_TYPE, IDC::CIdcLoginCltSrvReq::EN_MSG_ID, data, clientSession->m_envelopeKey, pSession);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);
	}
}

ACE_INT32  CIdcService::OnTypeProc(IDC::CIdcQueryUserInfoCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID)
{
	auto session = m_mapClientSession.Find(req.m_nSessionId);
	VBH::CQueryUserInfoCltCltReq vbhReq;

	if (!session)
	{
		return -1;
	}

	if (session->m_nToken != req.m_nToken)
	{
		if (session->m_nPreToken == req.m_nToken)
		{
			// token 过期，告诉client刷新token
		}
		return -1;
	}

	vbhReq.m_cryptUserKey.Set(session->m_envelopeKey.data(), session->m_envelopeKey.size());
	vbhReq.m_nSrcSessionID = m_nReqId;

	CQueryUserInfoSession* qSession = DSC_THREAD_TYPE_NEW(CQueryUserInfoSession) CQueryUserInfoSession(*this);

	qSession->m_nReqID = m_nReqId;
	qSession->m_nClientReqID = reqID;
	qSession->m_nSessionId = nClientSessionID;
	VBH::Assign(qSession->m_nonce, nonce);
	qSession->m_nHandleID = pMcpHandler->GetHandleID();

	this->SetDscTimer(qSession, CIdcService::EN_SESSION_TIMEOUT_VALUE);
	m_mapReqSession.Insert(m_nReqId, qSession);

	m_nReqId++;

	return SendVbhSdkServiceMessage(vbhReq);
}

ACE_INT32 CIdcService::OnTypeProc(IDC::CIdcUpdateUserInfoCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID)
{

}

class CDbAddr
{
public:
	CDbAddr()
		: m_nodeID("NODE_ID")
		, m_serviceID("SERVICE_ID")
		, m_dbID("DB_ID")
	{
	}

public:
	PER_BIND_ATTR(m_nodeID, m_serviceID, m_dbID);

public:
	CColumnWrapper< ACE_INT32 > m_nodeID;
	CColumnWrapper< ACE_INT32 > m_serviceID;
	CColumnWrapper< ACE_INT32 > m_dbID;

};


ACE_INT32 CIdcService::LoadDbAddrMap(void)
{
	CDscDatabase database;
	CDBConnection dbConnection;
	ACE_INT32 nRet = CDscDatabaseFactoryDemon::instance()->CreateDatabase(database, dbConnection);

	if (nRet)
	{
		DSC_RUN_LOG_ERROR("connect database failed.");

		return -1;
	}

	CTableWrapper< CCollectWrapper<CDbAddr> > lstCfg("IDC_DB_CFG");

	nRet = ::PerSelect(lstCfg, database, dbConnection);
	if (nRet)
	{
		DSC_RUN_LOG_ERROR("select from IDC_DB_CFG failed");

		return -1;
	}

	SDscAddr addr;

	for (auto it = lstCfg->begin(); it != lstCfg->end(); ++it)
	{
		addr.m_nNodeID = *it->m_nodeID;
		addr.m_nServiceID = *it->m_serviceID;

		m_mapDbAddr[*it->m_dbID] = addr;
	}

	if (m_mapDbAddr.size() == 0)
	{
		DSC_RUN_LOG_ERROR("IDC_DB_CFG empty");

		return -1;
	}

	int cand = ACE_OS::rand_r(&m_nRandSeed) % m_mapDbAddr.size();
	int i = 0;

	m_itCandDb = m_mapDbAddr.begin();
	while (i < cand)
	{
		++m_itCandDb;
	}

	return 0;
}

ACE_INT32 CIdcService::GetDbAddr(CDscMsg::CDscMsgAddr& addr, ACE_INT32 id)
{
	auto it = m_itCandDb;

	if (id != -1)
	{
		it = m_mapDbAddr.find(id);
		if (it == m_mapDbAddr.end())
		{
			return -1;
		}
	}
	else
	{
		++m_itCandDb;
		if (m_itCandDb == m_mapDbAddr.end())
		{
			m_itCandDb = m_mapDbAddr.begin();
		}
	}

	addr.SetNodeType(IDC::EN_IDC_DB_SERVER_APP_TYPE);
	addr.SetNodeID(it->second.m_nNodeID);
	addr.SetServiceType(IDC::EN_IDC_DB_SERVER_SERVICE_TYPE);
	addr.SetServiceID(it->second.m_nServiceID);

	return 0;
}

ACE_INT32 CIdcService::SendClientMsg(ACE_UINT32 nReturnCode, ACE_UINT32 nType, DSC::CDscBlob& data, CDscString & envelopeKey, CIdcService::CBaseSession * session)
{
	IDC::CIdcCommonSrvCltRsp rsp;

	rsp.m_nReturnCode = nReturnCode;
	rsp.m_nType = nType;
	rsp.m_nReqID = session->m_nClientReqID;
	
	DSC::CDscBlob nonce;
	
	nonce.Set(session->m_nonce.data(), session->m_nonce.size());

	IDC::CIdcCommonReqDataWrapper wrapper(data, nonce);

	char* encodeBuf = nullptr;
	size_t encodeBufLen = 0;
	char* pEncryptMsgBuf;
	int nEncryptMsgBufLen;

	DSC::Encode(wrapper, encodeBuf, encodeBufLen);
	pEncryptMsgBuf = VBH::vbhEncrypt((unsigned char*)envelopeKey.data(), nEncryptMsgBufLen, encodeBuf, encodeBufLen);

	DSC_THREAD_SIZE_FREE(encodeBuf, encodeBufLen);

	if (pEncryptMsgBuf)
	{
		rsp.m_info.Set(pEncryptMsgBuf, nEncryptMsgBufLen);

		this->SendHtsMsg(rsp, session->m_nHandleID);

		DSC_THREAD_SIZE_FREE(pEncryptMsgBuf, nEncryptMsgBufLen);
		return 0;
	}

	return -1;
}

ACE_UINT32 CIdcService::NewSessionId()
{
	ACE_UINT16 r1 = ACE_OS::rand_r(&m_nRandSeed) & 0xffff;
	ACE_UINT16 r2 = ACE_OS::rand_r(&m_nRandSeed) & 0xffff;
	ACE_UINT32 r = (r1 << 16) | (r2);

	while (true)
	{
		if (m_mapClientSession.Find(r))
		{
			r++;
		}
		else
		{
			break;
		}
	}
	return r;
}

ACE_UINT64 CIdcService::NewSessionToken()
{
	ACE_UINT16 r1 = ACE_OS::rand_r(&m_nRandSeed) & 0xffff;
	ACE_UINT16 r2 = ACE_OS::rand_r(&m_nRandSeed) & 0xffff;
	ACE_UINT16 r3 = ACE_OS::rand_r(&m_nRandSeed) & 0xffff;
	ACE_UINT16 r4 = ACE_OS::rand_r(&m_nRandSeed) & 0xffff;
	ACE_UINT64 r = (r1 << 16) | r2;

	r <<= 32;
	r |= (r3 << 16) | r4;

	return r;
}

ACE_UINT64 CIdcService::MaybeSessionTokenExpire(CIdcClientSession* client)
{
	auto now = ACE_OS::time();
	
	if (now > client->m_tokenTime + EN_CLIENT_SESSION_REFRESH_VALUE)
	{
		client->m_nPreToken = client->m_nToken;
		client->m_nToken = NewSessionToken();
		client->m_tokenTime = now;
	}

	return client->m_nToken;
}

CIdcService::CIdcClientSession* CIdcService::NewClientSession()
{
	auto session = DSC_THREAD_TYPE_NEW(CIdcClientSession) CIdcClientSession(*this);

	if (DSC_UNLIKELY(!session))
	{
		return nullptr;
	}

	session->m_nPreToken = 0;
	session->m_nToken = NewSessionToken();
	session->m_tokenTime = ACE_OS::time();
	session->m_nSessionId = NewSessionId();

	return session;
}

void CIdcService::CIdcClientSession::OnTimer()
{
	m_idcService.OnClientSessionTimeout(this);
}
void CIdcService::CRegisterSession::OnTimer()
{
	m_idcService.OnSessionTimeOut(this);
}
void CIdcService::CLoginSession::OnTimer()
{
	m_idcService.OnSessionTimeOut(this);
}
void CIdcService::CQueryUserInfoSession::OnTimer()
{
	m_idcService.OnSessionTimeOut(this);
}

void CIdcService::OnClientSessionTimeout(CIdcService::CIdcClientSession* clientSession)
{
	
}

void CIdcService::OnSessionTimeOut(CIdcService::CBaseSession* sessoin)
{
	DSC::CDscBlob blob;
	SendClientMsg(IDC::EN_IDC_ERROR_TIMEOUT, sessoin->m_nType, blob, m_peerEnvelopeKey, sessoin);

	DSC_THREAD_TYPE_DEALLOCATE(sessoin);
}