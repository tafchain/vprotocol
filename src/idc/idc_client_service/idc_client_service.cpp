#include "idc_client_service/idc_client_service.h"

#include <openssl/rand.h>
#include "dsc/configure/dsc_configure.h"
#include "dsc/dsc_comm_def.h"
#include "idc_sdk/idc_client.h"
#include "common/vbh_comm_error_code.h"
#include "common/vbh_encrypt_lib.h"

namespace IDC_SDK
{
	class CBlockchainAddr
	{
	public:
		CBlockchainAddr()
			: m_IpAddr("IP_ADDR")
			, m_port("PORT")
			, m_peerID("PEER_ID")
		{
		}

	public:
		PER_BIND_ATTR(m_IpAddr, m_port, m_peerID);

	public:
		CColumnWrapper< CDscString > m_IpAddr;
		CColumnWrapper< ACE_INT32 > m_port;
		CColumnWrapper< ACE_INT32 > m_peerID;
	};
	class CIDomAddrCriterion : public CSelectCriterion
	{
	public:
		virtual void SetCriterion(CPerSelect& rPerSelect)
		{
			rPerSelect.Where(rPerSelect["NODE_TYPE"] == IDC::EN_IDC_SERVER_TYPE);
			rPerSelect.OrderByAsc("PEER_ID");
		}
	};
	ACE_INT32 CIdcClientService::LoadPeerInfo(void)
	{
		ACE_INT32 nPeerID = -1;

		CDscDatabase database;
		CDBConnection dbConnection;

		if (CDscDatabaseFactoryDemon::instance()->CreateDatabase(database, dbConnection))
		{
			DSC_RUN_LOG_ERROR("connect database failed.");

			return -1;
		}

		CTableWrapper< CCollectWrapper<CBlockchainAddr> > lstIDomAddr("T_IDOM_SVR_CONFIG");
		CIDomAddrCriterion criterionIDom;

		if (::PerSelect(lstIDomAddr, database, dbConnection, &criterionIDom))
		{
			DSC_RUN_LOG_ERROR("select from T_IDom_ADDR failed");

			return -1;
		}

		CIDcPeer bcPeerIDom;

		for (CCollectWrapper<CBlockchainAddr>::iterator it = lstIDomAddr->begin(); it != lstIDomAddr->end(); ++it)
		{
			CIDcServer bcServer;

			if (nPeerID != *it->m_peerID)
			{
				if (!bcPeerIDom.m_lstIDcServer.empty())
				{
					bcPeerIDom.m_nPeerID = nPeerID;
					m_lstIDcPeer.push_back(bcPeerIDom);
					bcPeerIDom.m_lstIDcServer.clear();
				}
				nPeerID = *it->m_peerID;
			}

			bcServer.m_nHandleID = this->AllocHandleID();
			bcServer.m_strIDcServerIpAddr = *it->m_IpAddr;
			bcServer.m_nIDcServerPort = *it->m_port;
			bcPeerIDom.m_lstIDcServer.push_back(bcServer);
		}

		if (!bcPeerIDom.m_lstIDcServer.empty())
		{
			bcPeerIDom.m_nPeerID = nPeerID;
			m_lstIDcPeer.push_back(bcPeerIDom);
		}

		return 0;
	}

	ACE_INT32 CIdcClientService::OnInit()
	{
		if (CDscHtsClientService::OnInit())
		{
			DSC_RUN_LOG_ERROR("bc user client service init failed!");

			return -1;
		}

		if (this->LoadPeerInfo())
		{
			DSC_RUN_LOG_WARNING("load bc kernel addr info failed");

			return -1;
		}

		m_nRandSeed = ACE_OS::time(0);

		for (DSC::CDscList <CIDcPeer>::iterator peer_it = m_lstIDcPeer.begin(); peer_it != m_lstIDcPeer.end(); ++peer_it)
		{
			CIDcPeer& rPeer = *peer_it;

			for (DSC::CDscList <CIDcServer>::iterator svr_it = rPeer.m_lstIDcServer.begin(); svr_it != rPeer.m_lstIDcServer.end(); ++svr_it)
			{
				CIDcServer& bcServer = *svr_it;
				PROT_COMM::CDscIpAddr remoteAddr(bcServer.m_strIDcServerIpAddr, bcServer.m_nIDcServerPort);

				this->DoConnect(remoteAddr, NULL, bcServer.m_nHandleID);
			}
		}

		return 0;
	}

	ACE_INT32 CIdcClientService::OnExit(void)
	{
		return ACE_INT32();
	}

	void CIdcClientService::OnDscMsg(IDC::CInitClientSdk& rInitUserSdk, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
	{
		if (m_pCallback)
		{
			DSC_RUN_LOG_WARNING("repleat init regist user msg");
		}
		else
		{
			m_pCallback = (IDC_SDK::CIdcMsgCallback*)rInitUserSdk.m_pCallBack;

			if (m_bReady)
			{
				m_pCallback->OnReady();
			}
		}
	}

	void CIdcClientService::OnDscMsg(IDC::CIdcCommonApiCltReq& rCommonApiCltReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
	{
		ACE_INT32 nReturnCode = this->CommonApiReq(rCommonApiCltReq);
		if (DSC_UNLIKELY(nReturnCode))
		{
			DSC::CDscBlob data;
			OnCommonRsp(rCommonApiCltReq.m_nReqID, nReturnCode, rCommonApiCltReq.m_nType, data);

			DSC_RUN_LOG_WARNING("common api clt failed, error code:%d", nReturnCode);
		}
	}

	ACE_INT32 CIdcClientService::CommonApiReq(IDC::CIdcCommonApiCltReq& rIDomCommonApiCltReq)
	{
		ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;
		CMcpHandler* pMcpHandler = this->GetMcpHandler();

		if (pMcpHandler)
		{
			CCommonSession* pCommonSession = DSC_THREAD_TYPE_NEW(CCommonSession) CCommonSession(*this);

			pCommonSession->m_strNonce = ACE_OS::rand_r(0);

			IDC::CIdcCommonCltSrvReq req;
			char* encryptoBuf = nullptr;
			ACE_UINT32 encryptoBufLen = 0;
			DSC::CDscBlob nonce(pCommonSession->m_strNonce.data(), pCommonSession->m_strNonce.size());
			IDC::CIdcCommonReqDataWrapper wrapper(rIDomCommonApiCltReq.m_info, nonce);
			unsigned char* envolopeKey = nullptr;

			if (rIDomCommonApiCltReq.m_nType < IDC::EN_IDC_PUB_CRYPTO_KEY_MSG)
			{
				envolopeKey = (unsigned char*)m_peerEnvelopeKey.data();
			}
			else
			{
				envolopeKey = (unsigned char*)m_userEnvelopeKey.data();
			}

			EncodeCommonReq(wrapper, envolopeKey, encryptoBuf, encryptoBufLen);
			if (DSC_LIKELY(encryptoBuf))
			{
				req.m_nType = rIDomCommonApiCltReq.m_nType;
				req.m_nReqID = rIDomCommonApiCltReq.m_nReqID;
				req.m_nSessionID = m_nSessionID;
				req.m_info.Set(encryptoBuf, encryptoBufLen);
			}
			

			if (DSC_UNLIKELY(this->SendHtsMsg(req, pMcpHandler)))
			{
				nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
				DSC_THREAD_TYPE_DEALLOCATE(pCommonSession);
			}
			else
			{
				pCommonSession->m_nReqId = rIDomCommonApiCltReq.m_nReqID;
				pCommonSession->m_nType = rIDomCommonApiCltReq.m_nType;

				this->SetDscTimer(pCommonSession, CIdcClientService::EN_SESSION_TIMEOUT_VALUE);
				m_mapCommonSession.Insert(pCommonSession->m_nReqId, pCommonSession);
			}

			DSC_THREAD_SIZE_FREE(encryptoBuf, encryptoBufLen);
		}

		return 0;
	}

	ACE_INT32 CIdcClientService::OnHtsMsg(IDC::CIdcCommonSrvCltRsp& commonRsp, CMcpHandler* pMcpHandler)
	{
		OnCommonRsp(commonRsp.m_nReqID, commonRsp.m_nReturnCode, commonRsp.m_nType, commonRsp.m_info);

		return 0;
	}

	void CIdcClientService::OnCommonRsp(ACE_INT32 nReqID, ACE_INT32 nReturnCode, ACE_INT32 nType, DSC::CDscBlob &data)
	{
		m_pCallback->OnCommonRsp( nReqID,  nReturnCode,  nType,  data);
	}
	
	CMcpHandler* CIdcClientService::GetMcpHandler(void)
	{
		if (m_idcKernelSession.m_nPeerSessionNum)
		{
			if (m_idcKernelSession.m_nIndex >= m_idcKernelSession.m_nPeerSessionNum)
			{
				m_idcKernelSession.m_nIndex = 0;
			}

			CIDcPeerSession& rBcPeerSession = m_idcKernelSession.m_parrPeerSession[m_idcKernelSession.m_nIndex];

			++m_idcKernelSession.m_nIndex;
			if (rBcPeerSession.m_nIndex >= rBcPeerSession.m_nHandleIDNum)
			{
				rBcPeerSession.m_nIndex = 0;
			}

			return rBcPeerSession.m_ppMcpHandler[rBcPeerSession.m_nIndex++];
		}

		return NULL;
	}

	void CIdcClientService::EncodeCommonReq(IDC::CIdcCommonReqDataWrapper& wrapper,const unsigned char*envolopeKey, char*& outBuf, ACE_UINT32& outLen)
	{
		char* encodeBuf = nullptr;
		size_t encodeBufLen = 0;
		char* pEncryptMsgBuf;
		int nEncryptMsgBufLen;

		DSC::Encode(wrapper, encodeBuf, encodeBufLen);

		pEncryptMsgBuf = VBH::vbhEncrypt(envolopeKey, nEncryptMsgBufLen, encodeBuf, encodeBufLen);

		DSC_THREAD_SIZE_FREE(encodeBuf, encodeBufLen);
		outBuf = pEncryptMsgBuf;
		outLen = nEncryptMsgBufLen;
	}

	void CIdcClientService::CCommonSession::OnTimer()
	{

	}
} // namespace IDC_SDK