#include "dsc/mem_mng/dsc_allocator.h"
#include "dsc/configure/dsc_configure.h"

#include "common/vbh_comm_error_code.h"
#include "common/vbh_comm_msg_def.h"
#include "common/vbh_comm_wrap_msg_def.h"
#include "common/vbh_comm_func.h"
#include "common/vbh_encrypt_lib.h"
#include "common/crypt_key_codec.h"

#include "vbh_client_service/vbh_client_service.h"

CVbhClientService::IBaseSession::IBaseSession(CVbhClientService& rRegistUserService)
	: m_rClientBaseService(rRegistUserService)
{
}

CVbhClientService::CRegistUserSession::CRegistUserSession(CVbhClientService& rClientBaseService)
	: IBaseSession(rClientBaseService)
{
}

CVbhClientService::CRegistUserWaitNotitySession::CRegistUserWaitNotitySession(CVbhClientService& rClientBaseService)
	: IBaseSession(rClientBaseService)
{
}

CVbhClientService::CProposeSession::CProposeSession(CVbhClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

CVbhClientService::CProposeWaitNotifySession::CProposeWaitNotifySession(CVbhClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

CVbhClientService::CQueryUserSession::CQueryUserSession(CVbhClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

CVbhClientService::CQueryTransactionSession::CQueryTransactionSession(CVbhClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

ACE_INT32 CVbhClientService::OnInit(void)
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

	//读取背书节点个数// TODO:将来会有一个详细的背书策略数据库需要读取
	if (VBH::GetVbhProfileInt("ENDORSE_PEER_COUNT", m_nEndorsePeerCount))
	{
		DSC_RUN_LOG_WARNING("cann't read 'ENDORSE_PEER_COUNT' configure item value");

		return -1;
	}
	if (m_nEndorsePeerCount <= 0)
	{
		DSC_RUN_LOG_WARNING("'ENDORSE_PEER_COUNT' [%d] cann't be zero or negative value", m_nEndorsePeerCount);

		return -1;
	}

	//读取peer的公钥，用于非对称加密通信
	if (VBH::GetVbhProfileString("PEER_PUBLIC_KEY", m_peerPublicKey))
	{
		DSC_RUN_LOG_WARNING("cann't read 'PEER_PUBLIC_KEY' configure item value");

		return -1;
	}
	if (m_peerPublicKey.empty())
	{
		DSC_RUN_LOG_WARNING("'PEER_PUBLIC_KEY' cann't be empty");

		return -1;
	}

	ACE_OS::srand(ACE_OS::time(0));

	for (auto peer_it = m_lstBcPeer.begin(); peer_it != m_lstBcPeer.end(); ++peer_it)
	{
		CVbhPeer& rPeer = *peer_it;

		for (auto svr_it = rPeer.m_lstEndorserAddr.begin(); svr_it != rPeer.m_lstEndorserAddr.end(); ++svr_it)
		{
			CEndorserAddr& bcServer = *svr_it;
			PROT_COMM::CDscIpAddr remoteAddr(bcServer.m_strIpAddr, bcServer.m_nPort);

			this->DoConnect(remoteAddr, NULL, bcServer.m_nHandleID);
		}
	}

	return 0;
}

ACE_INT32 CVbhClientService::OnExit(void)
{
	CHandleSession* pHandleSession;
	for (auto it = m_mapHandleSession.begin(); it != m_mapHandleSession.end();)
	{
		pHandleSession = it.second;
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pHandleSession);
	}

	CRegistUserSession* pRegistUserSession;
	for (auto it = m_mapRegistUserSession.begin(); it != m_mapRegistUserSession.end();)
	{
		pRegistUserSession = it.second;
		this->CancelDscTimer(pRegistUserSession);
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pRegistUserSession);
	}

	CUnRegistUserSession* pUnRegistUserSession;
	for (auto it = m_mapUnRegistUserSession.begin(); it != m_mapUnRegistUserSession.end();)
	{
		pUnRegistUserSession = it.second;
		this->CancelDscTimer(pUnRegistUserSession);
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pUnRegistUserSession);
	}

	CQueryUserSession* pQueryUserSession;
	for (auto it = m_mapQueryUserSession.begin(); it != m_mapQueryUserSession.end();)
	{
		pQueryUserSession = it.second;
		this->CancelDscTimer(pQueryUserSession);
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
	}

	CProposeSession* pLaunchTransSession;
	for (auto it = m_mapProposeSession.begin(); it != m_mapProposeSession.end();)
	{
		pLaunchTransSession = it.second;
		this->CancelDscTimer(pLaunchTransSession);
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pLaunchTransSession);
	}

	for (ACE_UINT32 i = 0; i < m_vbhPeerConnSumInfo.m_nPeerSessionNum; ++i)
	{
		CVbhPeerConnectionInfo& rPeerSession = m_vbhPeerConnSumInfo.m_parrPeerConnInfo[i];

		DSC_DELETE_ARRAY(rPeerSession.m_ppMcpHandler);
	}
	DSC_DELETE_ARRAY(m_vbhPeerConnSumInfo.m_parrPeerConnInfo);

	return CDscHtsClientService::OnExit();
}

void CVbhClientService::OnResponse(CRegistUserSession* pRegistUserSession, const ACE_INT32 nRetrunCode, const bool bAllowRegist /*= false*/,
	CDscString& strCryptUserKey /*= CDscString()*/, CDscString& strGenUserKey /*= CDscString()*/)
{
	VBH::CRegistUserCltCltRsp rsp;

	rsp.m_nReturnCode = nRetrunCode;
	rsp.m_bAllowRegist = bAllowRegist;
	rsp.m_nSrcSessionID = pRegistUserSession->m_nSrcSessionID;
	if (nRetrunCode == VBH::EN_OK_TYPE)
	{
		VBH::Assign(rsp.m_genUserKey, strGenUserKey);
		VBH::Assign(rsp.m_cryptUserKey, strCryptUserKey);
	}

	this->SendDscMessage(rsp, pRegistUserSession->m_srcMsgAddr);
}

void CVbhClientService::OnTimeOut(CRegistUserSession* pRegistUserSession)
{
	OnResponse(pRegistUserSession, VBH::EN_TIMEOUT_ERROR_TYPE);
	m_mapRegistUserSession.Erase(pRegistUserSession);
	OnReleaseMultiHandleSession(pRegistUserSession);
}

void CVbhClientService::OnNetError(CRegistUserSession* pRegistUserSession)
{
	this->OnResponse(pRegistUserSession, VBH::EN_NETWORK_ERROR_TYPE);
	this->CancelDscTimer(pRegistUserSession);
	m_mapRegistUserSession.Erase(pRegistUserSession);
	this->OnReleaseMultiHandleSession(pRegistUserSession);
}

void CVbhClientService::OnResponse(CRegistUserWaitNotitySession* pNotifySession, const ACE_INT32 nRetrunCode, const bool bSuccess /*= false*/)
{
	VBH::CRegistUserCltCltNotify notify;

	notify.m_bSuccess = bSuccess;
	notify.m_nReturnCode = nRetrunCode;
	notify.m_nSrcSessionID = pNotifySession->m_nSrcSessionID;

	this->SendDscMessage(notify, pNotifySession->m_srcMsgAddr);
}

void CVbhClientService::OnTimeOut(CRegistUserWaitNotitySession* pNotifySession)
{
	OnResponse(pNotifySession, VBH::EN_TIMEOUT_ERROR_TYPE);
	m_mapRegistUserWaitNotifySession.Erase(pNotifySession);
	this->OnReleaseSingleHanleSession(pNotifySession);
}

void CVbhClientService::OnNetError(CRegistUserWaitNotitySession* pNotifySession)
{
	this->OnResponse(pNotifySession, VBH::EN_NETWORK_ERROR_TYPE);
	this->CancelDscTimer(pNotifySession);
	m_mapRegistUserWaitNotifySession.Erase(pNotifySession);
	this->OnReleaseSingleHanleSession(pNotifySession);
}

void CVbhClientService::OnResponse(CProposeSession* pProposeSession, const ACE_INT32 nRetrunCode, const char* pTransKey /*= nullptr*/, const size_t nTranKeyLen /*= 0*/)
{
	VBH::CProposeCltCltRsp rsp;

	rsp.m_nReturnCode = nRetrunCode;
	rsp.m_nSrcSessionID = pProposeSession->m_nSrcSessionID;
	if (VBH::EN_OK_TYPE == nRetrunCode)
	{
		rsp.m_genTransKey.Set((char*)pTransKey, nTranKeyLen);
	}

	this->SendDscMessage(rsp, pProposeSession->m_srcMsgAddr);
}

void CVbhClientService::OnTimeOut(CProposeSession* pProposeSession)
{
	this->OnResponse(pProposeSession, VBH::EN_TIMEOUT_ERROR_TYPE);
	m_mapProposeSession.Erase(pProposeSession);
	this->OnReleaseMultiHandleSession(pProposeSession);
}

void CVbhClientService::OnNetError(CProposeSession* pProposeSession)
{
	this->CancelDscTimer(pProposeSession);
	this->OnResponse(pProposeSession, VBH::EN_NETWORK_ERROR_TYPE);
	m_mapProposeSession.Erase(pProposeSession);
	this->OnReleaseMultiHandleSession(pProposeSession);
}

void CVbhClientService::OnResponse(CProposeWaitNotifySession* pNotifySession, const ACE_INT32 nRetrunCode, const bool bSuccess /*= false*/)
{
	VBH::CProposeCltCltNotify notify;

	notify.m_bSuccess = bSuccess;
	notify.m_nReturnCode = nRetrunCode;
	notify.m_nSrcSessionID = pNotifySession->m_nSrcSessionID;

	this->SendDscMessage(notify, pNotifySession->m_srcMsgAddr);
}

void CVbhClientService::OnTimeOut(CProposeWaitNotifySession* pNotifySession)
{
	OnResponse(pNotifySession, VBH::EN_TIMEOUT_ERROR_TYPE);
	m_mapProposeWaitNotifySession.Erase(pNotifySession);
	this->OnReleaseSingleHanleSession(pNotifySession);
}

void CVbhClientService::OnNetError(CProposeWaitNotifySession* pNotifySession)
{
	this->OnResponse(pNotifySession, VBH::EN_NETWORK_ERROR_TYPE);
	this->CancelDscTimer(pNotifySession);
	m_mapProposeWaitNotifySession.Erase(pNotifySession);
	this->OnReleaseSingleHanleSession(pNotifySession);
}

void CVbhClientService::OnResponse(CQueryUserSession* pQueryUserSession, const ACE_INT32 nRetrunCode, DSC::CDscBlob& userInfo /*= DSC::CDscBlob()*/)
{
	VBH::CQueryUserInfoCltCltRsp rsp;

	rsp.m_nReturnCode = nRetrunCode;
	rsp.m_nSrcSessionID = pQueryUserSession->m_nSrcSessionID;
	if (VBH::EN_OK_TYPE == nRetrunCode)
	{
		rsp.m_userInfo = userInfo;
	}

	this->SendDscMessage(rsp, pQueryUserSession->m_srcMsgAddr);
}

void CVbhClientService::OnTimeOut(CQueryUserSession* pQueryUserSession)
{
	this->CancelDscTimer(pQueryUserSession);
	this->OnResponse(pQueryUserSession, VBH::EN_TIMEOUT_ERROR_TYPE);
	m_mapQueryUserSession.Erase(pQueryUserSession);
	this->OnReleaseSingleHanleSession(pQueryUserSession);
}

void CVbhClientService::OnNetError(CQueryUserSession* pQueryUserSession)
{
	this->OnResponse(pQueryUserSession, VBH::EN_NETWORK_ERROR_TYPE);
	this->CancelDscTimer(pQueryUserSession);
	m_mapQueryUserSession.Erase(pQueryUserSession);
	this->OnReleaseSingleHanleSession(pQueryUserSession);
}

void CVbhClientService::OnResponse(CQueryTransactionSession* pQueryTransSession, const ACE_INT32 nRetrunCode, DSC::CDscBlob& transInfo /*= DSC::CDscBlob()*/)
{
	VBH::CQueryTransInfoCltCltRsp rsp;

	rsp.m_nReturnCode = nRetrunCode;
	rsp.m_nSrcSessionID = pQueryTransSession->m_nSrcSessionID;
	if (nRetrunCode == VBH::EN_OK_TYPE)
	{
		rsp.m_transInfo = transInfo;
	}

	this->SendDscMessage(rsp, pQueryTransSession->m_srcMsgAddr);
}

void CVbhClientService::OnTimeOut(CQueryTransactionSession* pQueryTransSession)
{
	this->OnResponse(pQueryTransSession, VBH::EN_TIMEOUT_ERROR_TYPE);
	m_mapQueryTransSession.Erase(pQueryTransSession);
	this->OnReleaseSingleHanleSession(pQueryTransSession);
}

void CVbhClientService::OnNetError(CQueryTransactionSession* pQueryTransSession)
{
	this->OnResponse(pQueryTransSession, VBH::EN_NETWORK_ERROR_TYPE);
	this->CancelDscTimer(pQueryTransSession);
	m_mapQueryTransSession.Erase(pQueryTransSession);
	this->OnReleaseSingleHanleSession(pQueryTransSession);
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CRegistUserEsCltRsp& rRegistUserRsp, CMcpHandler* pMcpHandler)
{
	CRegistUserSession* pRegistUserSession = m_mapRegistUserSession.Find(rRegistUserRsp.m_nCltSessionID);

	//1.找session
	if (pRegistUserSession)
	{
		ACE_INT32 nReturnCode = rRegistUserRsp.m_nReturnCode;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
		//2.解密、解码
			if (rRegistUserRsp.m_bAllowRegist) //同意创建
			{
				//3.背书
				if (pRegistUserSession->m_ccGenerateUserInfo.empty()) //初次收到应答
				{
					VBH::Assign(pRegistUserSession->m_ccGenerateUserInfo, rRegistUserRsp.m_userInitInfo);
					++pRegistUserSession->m_nSucceedRspNum;
				}
				else//第N次收到应答 N>1
				{
					if (VBH::IsEqual(pRegistUserSession->m_ccGenerateUserInfo, rRegistUserRsp.m_userInitInfo))
					{
						++pRegistUserSession->m_nSucceedRspNum;
					}
					else
					{
						DSC_RUN_LOG_ERROR("endorse failed.");
						nReturnCode = VBH::EN_SYSTEM_ERROR_TYPE;
					}
				}

				//是选中的endorser节点发送的应答
				if ((nReturnCode == VBH::EN_OK_TYPE) && (pMcpHandler == pRegistUserSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler))
				{
					pRegistUserSession->m_nEsSessionID = rRegistUserRsp.m_nEsSessionID; //endorser session-id
				}

				//TODO: 背书策略或许会调整
				if ((nReturnCode == VBH::EN_OK_TYPE) && (pRegistUserSession->m_nSucceedRspNum >= m_nEndorsePeerCount)) //背书通过
				{
					//组织回应答(再次提交注册事务)							
					//5.向服务端提交
					VBH::CSubmitRegistTransactionCltEsReq req;

					req.m_nEsSessionID = pRegistUserSession->m_nEsSessionID;

					if (DSC_UNLIKELY(this->SendHtsMsg(req, pRegistUserSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler)))
					{
						DSC_RUN_LOG_ERROR("send hts msg failed.");
						nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
					}
				}
				else
				{
					//不用处理
				}
			}
			else //拒绝创建
			{
				DSC_RUN_LOG_INFO("regist user is denied by peer:%d", GetPeerID(pMcpHandler->GetHandleID()));
				nReturnCode = VBH::EN_REGIST_USER_IS_REFUSED;
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("regist user rsp return failed, error code:%d, channelID:%d", rRegistUserRsp.m_nReturnCode, pRegistUserSession->m_nChannelID);
		}

		if (nReturnCode != VBH::EN_OK_TYPE)
		{
			//出错后，向选中的提交节点发送取消事务的消息//网络出错就不再发送
			if (nReturnCode != VBH::EN_NETWORK_ERROR_TYPE)
			{
				VBH::CCancelRegistUserCltEsReq req;

				req.m_nEsSessionID = pRegistUserSession->m_nEsSessionID;
				this->SendHtsMsg(req, pRegistUserSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler);
			}

			//向客户端上层业务应答，//删除session
			this->OnResponse(pRegistUserSession, nReturnCode);
			this->CancelDscTimer(pRegistUserSession);
			m_mapRegistUserSession.Erase(pRegistUserSession);
			OnReleaseMultiHandleSession(pRegistUserSession); //删除session
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("can not find session, session-id:%d", pRegistUserSession->m_nCltSessionID);
	}

	return 0;
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CSubmitRegistTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler)
{
	CRegistUserSession* pRegistUserSession = m_mapRegistUserSession.Erase(rSubmitTransRsp.m_nCltSessionID);

	if (pRegistUserSession)
	{
		ACE_INT32 nReturnCode = rSubmitTransRsp.m_nReturnCode;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			//为用户生成key和秘钥
			CDscString strCryptUserKey;
			CDscString svrPubCryptKey(rSubmitTransRsp.m_svrPubCryptKey.c_str(), rSubmitTransRsp.m_svrPubCryptKey.size());
			CDscString cltPriCryptKey(rSubmitTransRsp.m_cltPriCryptKey.c_str(), rSubmitTransRsp.m_cltPriCryptKey.size());
			CDscString envelopeKey(rSubmitTransRsp.m_envelopeKey.c_str(), rSubmitTransRsp.m_envelopeKey.size()); //客户端要存储的秘钥组
			CDscString strGenUserKey;

			nReturnCode = VBH::EncodeGenerateUserKey(strGenUserKey, pRegistUserSession->m_nChannelID, rSubmitTransRsp.m_genUserKey);
			if (nReturnCode == VBH::EN_OK_TYPE)
			{
				nReturnCode = VBH::EncodeClientCryptKey(strCryptUserKey, pRegistUserSession->m_nChannelID, rSubmitTransRsp.m_genUserKey, svrPubCryptKey, cltPriCryptKey, envelopeKey);
			}
			//向客户端上层业务应答
			this->OnResponse(pRegistUserSession, nReturnCode, true, strCryptUserKey, strGenUserKey);

			if (pRegistUserSession->m_bWaitNotify)//如果客户等待通知，则创建wait-notify-session
			{
				CRegistUserWaitNotitySession* pNotifySession = DSC_THREAD_TYPE_NEW(CRegistUserWaitNotitySession) CRegistUserWaitNotitySession(*this);

				pNotifySession->m_srcMsgAddr = pRegistUserSession->m_srcMsgAddr;
				pNotifySession->m_nSrcSessionID = pRegistUserSession->m_nSrcSessionID;
				pNotifySession->m_nCltSessionID = pRegistUserSession->m_nCltSessionID;
				pNotifySession->m_pHandleSession = pRegistUserSession->m_pSubmitEsMcpHandleSession;

				pNotifySession->m_pHandleSession->m_arrSession.Insert(pNotifySession);

				m_mapRegistUserWaitNotifySession.DirectInsert(pNotifySession->m_nCltSessionID, pNotifySession);
				this->SetDscTimer(pNotifySession, EN_SESSION_WAIT_NOTIFY_TIMEOUT_VALUE);
			}
			//删除session 
			this->CancelDscTimer(pRegistUserSession);
			this->OnReleaseMultiHandleSession(pRegistUserSession);
		}
		else
		{
			DSC_RUN_LOG_INFO("submit regist transaction rsp return failed, error code:%d, channelID:%d", rSubmitTransRsp.m_nReturnCode, pRegistUserSession->m_nChannelID);
		}
	}
	else
	{
		DSC_RUN_LOG_WARNING("can not find regist session, session-id:%d", rSubmitTransRsp.m_nCltSessionID);
	}

	return 0;
}

ACE_VERSIONED_NAMESPACE_NAME::ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CSubscribeRegistTransactionEsCltNotify& rNotify, CMcpHandler* pMcpHandler)
{
	CRegistUserWaitNotitySession* pSession = m_mapRegistUserWaitNotifySession.Erase(rNotify.m_nCltSessionID);

	if (DSC_LIKELY(pSession))
	{
		this->OnResponse(pSession, rNotify.m_nReturnCode, rNotify.m_bTransResult);
		this->CancelDscTimer(pSession);
		this->OnReleaseSingleHanleSession(pSession);
	}
	else
	{
		DSC_RUN_LOG_ERROR("can not find regist user wait notfiy session.");
	}

	return 0;
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CUnRegistUserRsp& rUnRegistUserRsp, CMcpHandler* pMcpHandler)
{

	return 0;
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CSubmitProposalEsCltRsp& rSubmitProposalRsp, CMcpHandler* pMcpHandler)
{
	CProposeSession* pProposeSession = m_mapProposeSession.Find(rSubmitProposalRsp.m_nCltSessionID);

	if (DSC_UNLIKELY(NULL == pProposeSession))
	{
		DSC_RUN_LOG_WARNING("submit proposal rsp timeout");
	}
	else
	{
		ACE_INT32 nReturnCode = rSubmitProposalRsp.m_nReturnCode;

		if (DSC_LIKELY(nReturnCode == VBH::EN_OK_TYPE))
		{
			if (pProposeSession->m_transContent.empty())
			{
				VBH::Assign(pProposeSession->m_transContent, rSubmitProposalRsp.m_transContent);
				++pProposeSession->m_nSucceedRspNum;
			}
			else
			{
				if (pProposeSession->m_transContent.IsEqual(rSubmitProposalRsp.m_transContent.GetBuffer(), rSubmitProposalRsp.m_transContent.GetSize()))
				{
					++pProposeSession->m_nSucceedRspNum;
				}
				else
				{
					DSC_RUN_LOG_ERROR("endorser failed.");
					nReturnCode = VBH::EN_SYSTEM_ERROR_TYPE;
				}
			}

			//是选中的节点发送的应答
			if ((nReturnCode == VBH::EN_OK_TYPE) && (pMcpHandler == pProposeSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler))
			{
				pProposeSession->m_nEsSessionID = rSubmitProposalRsp.m_nEsSessionID; //记录endorser session-id
			}

			//TODO: 背书策略或许会调整
			if (pProposeSession->m_nSucceedRspNum >= m_nEndorsePeerCount)
			{
				VBH::CSubmitProposalTransactionCltEsReq req;

				req.m_nEsSessionID = pProposeSession->m_nEsSessionID;

				if (DSC_UNLIKELY(this->SendHtsMsg(req, pProposeSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler)))
				{
					DSC_RUN_LOG_ERROR("send hts msg failed.");
					nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
				}
			}
			else
			{
				//无需处理，再等下一应答消息
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("submit proposal rsp return failed, error code:%d", rSubmitProposalRsp.m_nReturnCode);
		}

		if (nReturnCode != VBH::EN_OK_TYPE)
		{
			// 不是网络错误的情况下， 出错后，向选中的提交节点发送取消事务的消息
			if (nReturnCode != VBH::EN_NETWORK_ERROR_TYPE)
			{
				VBH::CCancelProposalTransactionCltEsReq req;

				req.m_nEsSessionID = pProposeSession->m_nEsSessionID;
				this->SendHtsMsg(req, pProposeSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler);
			}

			//向客户端上层通知 //删除session
			this->OnResponse(pProposeSession, nReturnCode);
			this->CancelDscTimer(pProposeSession);
			m_mapProposeSession.Erase(pProposeSession);
			this->OnReleaseMultiHandleSession(pProposeSession);
		}
	}

	return 0;
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CSubmitProposalTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler)
{
	CProposeSession* pProposeSession = m_mapProposeSession.Erase(rSubmitTransRsp.m_nCltSessionID);

	if (pProposeSession)
	{
		ACE_INT32 nReturnCode = rSubmitTransRsp.m_nReturnCode;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			//返回提案应答
			char* pTransKey;
			size_t nTranKeyLen;

			DEF_ENCODE(rSubmitTransRsp.m_genTransKey, pTransKey, nTranKeyLen);
			this->OnResponse(pProposeSession, VBH::EN_OK_TYPE, pTransKey, nTranKeyLen);

			if (pProposeSession->m_bWaitNotify) //如果客户等待通知，则创建wait-notify-session
			{
				CProposeWaitNotifySession* pNotifySession = DSC_THREAD_TYPE_NEW(CProposeWaitNotifySession) CProposeWaitNotifySession(*this);

				pNotifySession->m_srcMsgAddr = pProposeSession->m_srcMsgAddr;
				pNotifySession->m_nSrcSessionID = pProposeSession->m_nSrcSessionID;
				pNotifySession->m_nCltSessionID = pProposeSession->m_nCltSessionID;
				pNotifySession->m_pHandleSession = pProposeSession->m_pSubmitEsMcpHandleSession;

				pNotifySession->m_pHandleSession->m_arrSession.Insert(pNotifySession);

				m_mapProposeWaitNotifySession.DirectInsert(pNotifySession->m_nCltSessionID, pNotifySession);
				this->SetDscTimer(pNotifySession, EN_SESSION_WAIT_NOTIFY_TIMEOUT_VALUE);
			}
			//客户不等待通知，则删除session
			this->CancelDscTimer(pProposeSession);
			this->OnReleaseMultiHandleSession(pProposeSession);
		}
		else
		{
			DSC_RUN_LOG_INFO("submit proposal transaction rsp return failed");
		}
	}
	else
	{
		DSC_RUN_LOG_WARNING("can not find propose session, session-id:%d", rSubmitTransRsp.m_nCltSessionID);
	}

	return 0;
}

ACE_VERSIONED_NAMESPACE_NAME::ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CSubscribeProposeTransactionEsCltNotify& rNotify, CMcpHandler* pMcpHandler)
{
	CProposeWaitNotifySession* pSession = m_mapProposeWaitNotifySession.Erase(rNotify.m_nCltSessionID);

	if (DSC_LIKELY(pSession))
	{
		this->OnResponse(pSession, rNotify.m_nReturnCode, rNotify.m_bTransResult);
		this->CancelDscTimer(pSession);
		this->OnReleaseSingleHanleSession(pSession);
	}
	else
	{
		DSC_RUN_LOG_ERROR("can not find propose wait notfiy session.");
	}

	return 0;
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CQueryUserInfoEdsCltRsp& rQueryUserRsp, CMcpHandler* pMcpHandler)
{
	CQueryUserSession* pQueryUserSession = m_mapQueryUserSession.Erase(rQueryUserRsp.m_nSessionID);

	if (DSC_LIKELY(pQueryUserSession))
	{
		this->OnResponse(pQueryUserSession, rQueryUserRsp.m_nReturnCode, rQueryUserRsp.m_userInfo);
		this->CancelDscTimer(pQueryUserSession);
		this->OnReleaseSingleHanleSession(pQueryUserSession);
	}
	else
	{
		DSC_RUN_LOG_ERROR("can not find query user session.");
	}

	return 0;
}

ACE_INT32 CVbhClientService::OnHtsMsg(VBH::CQueryTransInfoEsCltRsp& rQueryTransRsp, CMcpHandler* pMcpHandler)
{
	CQueryTransactionSession* pQueryTransSession = m_mapQueryTransSession.Erase(rQueryTransRsp.m_nCltSessionID);

	if (DSC_LIKELY(pQueryTransSession))
	{
		this->OnResponse(pQueryTransSession, rQueryTransRsp.m_nReturnCode, rQueryTransRsp.m_transInfo);
		this->CancelDscTimer(pQueryTransSession);
		this->OnReleaseSingleHanleSession(pQueryTransSession);
	}
	else
	{
		DSC_RUN_LOG_WARNING("can not find query transaction session.");
	}

	return 0;
}

void CVbhClientService::OnDscMsg(VBH::CRegistUserCltCltReq& rRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;

	if (m_vbhPeerConnSumInfo.m_nPeerSessionNum < m_nEndorsePeerCount)
	{
		DSC_RUN_LOG_WARNING("connected peer num:%d cann't less than configed endorse peer count:%d", m_vbhPeerConnSumInfo.m_nPeerSessionNum, m_nEndorsePeerCount);

		nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
	}
	else
	{
		CRegistUserSession* pRegistUserSession = DSC_THREAD_TYPE_NEW(CRegistUserSession) CRegistUserSession(*this);
		VBH::CRegistUserCltEsReq req;

		pRegistUserSession->m_srcMsgAddr = rSrcMsgAddr;
		pRegistUserSession->m_nSrcSessionID = rRegistUserReq.m_nSrcSessionID;
		pRegistUserSession->m_nChannelID = rRegistUserReq.m_nChannelID;
		pRegistUserSession->m_bWaitNotify = rRegistUserReq.m_bWaitNotify;

		req.m_bWaitNotify = rRegistUserReq.m_bWaitNotify;
		req.m_nCcID = rRegistUserReq.m_nCcID;
		req.m_nCltSessionID = m_nSessionID;
		req.m_nChannelID = rRegistUserReq.m_nChannelID;
		req.m_userKey = rRegistUserReq.m_userKey;
		req.m_userInfo = rRegistUserReq.m_userInfo;

		CMcpHandler* pMcpHandler;
		ACE_UINT32 nHandleID;
		CHandleSession* pHandleSession;
		const ACE_INT32 nRand = abs(ACE_OS::rand_r(&m_seed)) % m_nEndorsePeerCount; //TODO:将来背书策略有调整

		for (ACE_INT32 idx = 0; idx < m_nEndorsePeerCount; ++idx)
		{
			//记录 regist-user-session 所用的 handler，并将 handler 放入对应的 handle-session中
			pMcpHandler = this->GetMcpHandler();
			nHandleID = pMcpHandler->GetHandleID();
			pHandleSession = m_mapHandleSession.Find(nHandleID);
			if (!pHandleSession)
			{
				pHandleSession = DSC_THREAD_TYPE_NEW(CHandleSession) CHandleSession;
				pHandleSession->m_pMcpHandler = pMcpHandler;
				m_mapHandleSession.DirectInsert(nHandleID, pHandleSession);
			}
			//记录handle-session和 regist-user-session 的勾连关系
			pRegistUserSession->m_arrHandleSession.Insert(pHandleSession);
			pHandleSession->m_arrSession.Insert(pRegistUserSession);

			if (nRand == idx)
			{
				pRegistUserSession->m_pSubmitEsMcpHandleSession = pHandleSession;
				req.m_bSubmitNode = true;
			}
			else
			{
				req.m_bSubmitNode = false;
			}

			if (DSC_UNLIKELY(this->SendHtsMsg(req, pMcpHandler)))
			{
				DSC_RUN_LOG_ERROR("network error.");
				OnNetError(pRegistUserSession);

				nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
				break;
			}
		}

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			pRegistUserSession->m_nCltSessionID = m_nSessionID;

			SetDscTimer(pRegistUserSession, EN_SESSION_TIMEOUT_VALUE);
			m_mapRegistUserSession.DirectInsert(m_nSessionID, pRegistUserSession);
			++m_nSessionID;
		}
		else
		{
			//不需要处理
		}
	}

	if (nReturnCode != VBH::EN_OK_TYPE)
	{
		VBH::CRegistUserCltCltRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nSrcSessionID = rRegistUserReq.m_nSrcSessionID; // 上层业务service发起请求时，发送方的sessionID
		//失败场景下，rsp其他属性不必要填写；
		this->SendDscMessage(rsp, rSrcMsgAddr);
	}
}

void CVbhClientService::OnDscMsg(VBH::CUnRegistUserReq& rUnRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	//ACE_INT32 nReturnCode = this->UnRegistUser(rUnRegistUserReq);

	//if (DSC_UNLIKELY(nReturnCode))
	//{
	//	VBH_SDK::CUnRegistUserRsp rsp;

	//	rsp.m_nReturnCode = nReturnCode;
	//	rsp.m_nChannelID = rUnRegistUserReq.m_nChannelID;

	//	//TODO: 取消注册的完整性暂时不考虑
	//	//rsp.m_pUserKey = rUnRegistUserReq.m_userKey.GetBuffer();
	//	//rsp.m_nUserKeyLen = rUnRegistUserReq.m_userKey.GetSize();

	//	m_pCallback->OnUnRegistUserRsp(rsp);

	//	DSC_RUN_LOG_INFO("regist user failed, error code:%d", nReturnCode);
	//}
}

void CVbhClientService::OnDscMsg(VBH::CProposeCltCltReq& rProposeReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;

	if (m_vbhPeerConnSumInfo.m_nPeerSessionNum < m_nEndorsePeerCount)
	{
		DSC_RUN_LOG_WARNING("connected peer num:%d cann't less than configed endorse peer count:%d", m_vbhPeerConnSumInfo.m_nPeerSessionNum, m_nEndorsePeerCount);
		nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
	}
	else
	{
		VBH::CSubmitProposalCltEsReq req;
		CProposeSession* pProposeSession = DSC_THREAD_TYPE_NEW(CProposeSession) CProposeSession(*this);

		if (DSC_UNLIKELY(VBH::DecodeClientCryptKey(rProposeReq.m_cryptUserKey, req.m_nChannelID)))
		{
			DSC_THREAD_TYPE_DELETE(pProposeSession);
			DSC_RUN_LOG_ERROR("decode client crypt key error.");
			nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
		}
		else
		{
			req.m_bWaitNotify = rProposeReq.m_bWaitNotify;
			req.m_nCcID = rProposeReq.m_nCcID;
			req.m_nCltSessionID = m_nSessionID;
			req.m_nAction = rProposeReq.m_nActionID;
			req.m_proposal = rProposeReq.m_proposal;

			CMcpHandler* pMcpHandler;
			ACE_UINT32 nHandleID;
			CHandleSession* pHandleSession;
			const ACE_INT32 nRand = abs(ACE_OS::rand_r(&m_seed)) % m_nEndorsePeerCount; //TODO:将来背书策略有调整

			for (ACE_INT32 idx = 0; idx < m_nEndorsePeerCount; ++idx)
			{
				//记录 propose-session 所用的 handler，并将 handler 放入对应的 handle-session中
				pMcpHandler = this->GetMcpHandler();
				nHandleID = pMcpHandler->GetHandleID();
				pHandleSession = m_mapHandleSession.Find(nHandleID);
				if (!pHandleSession)
				{
					pHandleSession = DSC_THREAD_TYPE_NEW(CHandleSession) CHandleSession;
					pHandleSession->m_pMcpHandler = pMcpHandler;
					m_mapHandleSession.DirectInsert(nHandleID, pHandleSession);
				}
				//记录handle-session和 propose-session的勾连关系
				pProposeSession->m_arrHandleSession.Insert(pHandleSession);
				pHandleSession->m_arrSession.Insert(pProposeSession);

				if (nRand == idx)
				{
					req.m_bSubmitNode = true;
					pProposeSession->m_pSubmitEsMcpHandleSession = pHandleSession;
				}
				else
				{
					req.m_bSubmitNode = false;
				}

				if (DSC_UNLIKELY(this->SendHtsMsg(req, pMcpHandler)))
				{
					OnNetError(pProposeSession); //释放session
					nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
					break;
				}
			}

			if (nReturnCode == VBH::EN_OK_TYPE)
			{
				pProposeSession->m_nCltSessionID = m_nSessionID;
				pProposeSession->m_srcMsgAddr = rSrcMsgAddr;
				pProposeSession->m_nSrcSessionID = rProposeReq.m_nSrcSessionID;

				this->SetDscTimer(pProposeSession, CVbhClientService::EN_SESSION_TIMEOUT_VALUE);
				m_mapProposeSession.Insert(m_nSessionID, pProposeSession);
				++m_nSessionID;
			}
			else
			{
				//网络出错不处理了
			}
		}
	}

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH::CProposeCltCltRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nSrcSessionID = rProposeReq.m_nSrcSessionID;

		this->SendDscMessage(rsp, rSrcMsgAddr);
	}
}

void CVbhClientService::OnDscMsg(VBH::CQueryUserInfoCltCltReq& rQueryUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;
	VBH::CQueryUserInfoCltEsReq req;
	CQueryUserSession* pQueryUserSession = DSC_THREAD_TYPE_NEW(CQueryUserSession) CQueryUserSession(*this);

	//从查询消息中取出服务端公钥和客户端私钥
	if (VBH::DecodeClientCryptKey(rQueryUserReq.m_cryptUserKey, req.m_nChannelID, req.m_genUserKey))
	{
		DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
		DSC_RUN_LOG_ERROR("decode client crypt key error.");
		nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
	}
	else
	{
		CMcpHandler* pMcpHandler = this->GetMcpHandler();

		req.m_nCltSessionID = m_nSessionID;

		if (this->SendHtsMsg(req, pMcpHandler))
		{
			DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
			nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
		}
		else
		{
			//记录 query-user-session 所用的 handler，并将 handler 放入对应的 handle-session中
			ACE_UINT32 nHandleID = pMcpHandler->GetHandleID();
			CHandleSession* pHandleSession = m_mapHandleSession.Find(nHandleID);
			if (!pHandleSession)
			{
				pHandleSession = DSC_THREAD_TYPE_NEW(CHandleSession) CHandleSession;
				pHandleSession->m_pMcpHandler = pMcpHandler;
				m_mapHandleSession.DirectInsert(nHandleID, pHandleSession);
			}
			//记录handle-session和 propose-session的勾连关系
			pQueryUserSession->m_pHandleSession = pHandleSession;
			pHandleSession->m_arrSession.Insert(pQueryUserSession);

			pQueryUserSession->m_nSrcSessionID = rQueryUserReq.m_nSrcSessionID;
			pQueryUserSession->m_nCltSessionID = m_nSessionID;
			pQueryUserSession->m_srcMsgAddr = rSrcMsgAddr;

			this->SetDscTimer(pQueryUserSession, CVbhClientService::EN_SESSION_TIMEOUT_VALUE);
			m_mapQueryUserSession.Insert(m_nSessionID, pQueryUserSession);
			++m_nSessionID;
		}
	}

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH::CQueryUserInfoCltCltRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nSrcSessionID = rQueryUserReq.m_nSrcSessionID;

		this->SendDscMessage(rsp, rSrcMsgAddr);
	}
}

void CVbhClientService::OnDscMsg(VBH::CQueryTransInfoCltCltReq& rQueryTransReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;
	VBH::CQueryTransInfoCltEsReq req;
	CQueryTransactionSession* pQueryTransSession = DSC_THREAD_TYPE_NEW(CQueryTransactionSession) CQueryTransactionSession(*this);

	if (VBH::DecodeClientCryptKey(rQueryTransReq.m_cryptUserKey, req.m_nChannelID))
	{
		DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
		DSC_RUN_LOG_ERROR("decode client crypt key failed.");
		nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
	}
	else
	{
		if (DSC::Decode(req.m_genTransKey, rQueryTransReq.m_transKey.GetBuffer(), rQueryTransReq.m_transKey.GetSize()))
		{
			DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
			DSC_RUN_LOG_ERROR("decode transaction key error.");
			nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
		}
		else
		{
			CMcpHandler* pMcpHandler = this->GetMcpHandler();

			req.m_nCltSessionID = m_nSessionID;
			if (this->SendHtsMsg(req, pMcpHandler))
			{
				DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
				nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
			}
			else
			{
				//记录 query-trans-session 所用的 handler，并将 handler 放入对应的 handle-session中
				ACE_UINT32 nHandleID = pMcpHandler->GetHandleID();
				CHandleSession* pHandleSession = m_mapHandleSession.Find(nHandleID);
				if (!pHandleSession)
				{
					pHandleSession = DSC_THREAD_TYPE_NEW(CHandleSession) CHandleSession;
					pHandleSession->m_pMcpHandler = pMcpHandler;
					m_mapHandleSession.DirectInsert(nHandleID, pHandleSession);
				}
				//记录handle-session和 propose-session的勾连关系
				pQueryTransSession->m_pHandleSession = pHandleSession;
				pHandleSession->m_arrSession.Insert(pQueryTransSession);

				pQueryTransSession->m_nSrcSessionID = rQueryTransReq.m_nSrcSessionID;
				pQueryTransSession->m_srcMsgAddr = rSrcMsgAddr;
				pQueryTransSession->m_nCltSessionID = m_nSessionID;

				this->SetDscTimer(pQueryTransSession, EN_SESSION_TIMEOUT_VALUE);
				m_mapQueryTransSession.Insert(m_nSessionID, pQueryTransSession);
				++m_nSessionID;
			}
		}
	}

	if (nReturnCode != VBH::EN_OK_TYPE)
	{
		VBH::CQueryTransInfoCltCltRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nSrcSessionID = rQueryTransReq.m_nSrcSessionID;

		this->SendDscMessage(rsp, rSrcMsgAddr);
	}
}

ACE_INT32 CVbhClientService::OnConnectedNodify(CMcpClientHandler* pMcpClientHandler)
{
	const ACE_INT32 nPeerID = this->GetPeerID(pMcpClientHandler->GetHandleID());
	CVbhPeerConnectionInfo* pPeerSession = NULL;
	bool bReady = false;

	ACE_ASSERT(nPeerID != -1);
	if (m_vbhPeerConnSumInfo.m_parrPeerConnInfo)
	{
		for (ACE_UINT32 i = 0; i < m_vbhPeerConnSumInfo.m_nPeerSessionNum; ++i)
		{
			if (m_vbhPeerConnSumInfo.m_parrPeerConnInfo[i].m_nPeerID == nPeerID)
			{
				pPeerSession = m_vbhPeerConnSumInfo.m_parrPeerConnInfo + i;
				break;
			}
		}

		if (!pPeerSession)
		{
			CVbhPeerConnectionInfo* parrPeerSession;

			DSC_NEW_ARRAY(parrPeerSession, CVbhPeerConnectionInfo, m_vbhPeerConnSumInfo.m_nPeerSessionNum + 1);
			ACE_OS::memcpy(parrPeerSession, m_vbhPeerConnSumInfo.m_parrPeerConnInfo, sizeof(CVbhPeerConnectionInfo) * m_vbhPeerConnSumInfo.m_nPeerSessionNum);
			DSC_DELETE_ARRAY(m_vbhPeerConnSumInfo.m_parrPeerConnInfo);
			m_vbhPeerConnSumInfo.m_parrPeerConnInfo = parrPeerSession;
			pPeerSession = m_vbhPeerConnSumInfo.m_parrPeerConnInfo + m_vbhPeerConnSumInfo.m_nPeerSessionNum;
			pPeerSession->m_nPeerID = nPeerID;
			++m_vbhPeerConnSumInfo.m_nPeerSessionNum;
			m_vbhPeerConnSumInfo.m_nIndex = abs(ACE_OS::rand_r(&m_seed)) % m_vbhPeerConnSumInfo.m_nPeerSessionNum; //连接的peer数变动时，重新计算一遍随机值，保证每个peer被选为背书peer的机会均等
		}
	}
	else
	{
		m_vbhPeerConnSumInfo.m_nPeerSessionNum = 1;
		DSC_NEW_ARRAY(m_vbhPeerConnSumInfo.m_parrPeerConnInfo, CVbhPeerConnectionInfo, m_vbhPeerConnSumInfo.m_nPeerSessionNum);
		pPeerSession = m_vbhPeerConnSumInfo.m_parrPeerConnInfo;
		pPeerSession->m_nPeerID = nPeerID;

		bReady = true;
	}

	if (pPeerSession->m_ppMcpHandler)
	{
		CMcpHandlerPtr* ppMcpHandler;

		DSC_NEW_ARRAY(ppMcpHandler, CMcpHandlerPtr, pPeerSession->m_nHandleIDNum + 1);
		ACE_OS::memcpy(ppMcpHandler, pPeerSession->m_ppMcpHandler, sizeof(CMcpHandlerPtr) * pPeerSession->m_nHandleIDNum);
		DSC_DELETE_ARRAY(pPeerSession->m_ppMcpHandler);
		pPeerSession->m_ppMcpHandler = ppMcpHandler;
		pPeerSession->m_ppMcpHandler[pPeerSession->m_nHandleIDNum] = pMcpClientHandler;
		++pPeerSession->m_nHandleIDNum;
		pPeerSession->m_nIndex = abs(ACE_OS::rand_r(&m_seed)) % pPeerSession->m_nHandleIDNum; //当连接数变动时，重新计算一遍随机值，保证session平均的发送到每个endorser
	}
	else
	{
		pPeerSession->m_nHandleIDNum = 1;
		DSC_NEW_ARRAY(pPeerSession->m_ppMcpHandler, CMcpHandlerPtr, pPeerSession->m_nHandleIDNum);
		*pPeerSession->m_ppMcpHandler = pMcpClientHandler;
	}

	return 0;
}

void CVbhClientService::OnNetworkError(CMcpHandler* pMcpHandler)
{
	const ACE_INT32 nPeerID = this->GetPeerID(pMcpHandler->GetHandleID());

	//断连后，更新和peer的连接信息以及和某个peer的endorser的连接信息
	ACE_ASSERT(nPeerID != -1);
	for (ACE_UINT32 i = 0; i < m_vbhPeerConnSumInfo.m_nPeerSessionNum; ++i)
	{
		if (m_vbhPeerConnSumInfo.m_parrPeerConnInfo[i].m_nPeerID == nPeerID)
		{
			CVbhPeerConnectionInfo& rBcPeerSession = m_vbhPeerConnSumInfo.m_parrPeerConnInfo[i];

			for (ACE_UINT32 j = 0; j < rBcPeerSession.m_nHandleIDNum; ++j)
			{
				if (rBcPeerSession.m_ppMcpHandler[j] == pMcpHandler)
				{
					if (rBcPeerSession.m_nHandleIDNum > 1)
					{
						CMcpHandlerPtr* ppMcpHandler;
						ACE_UINT32 n = 0;

						DSC_NEW_ARRAY(ppMcpHandler, CMcpHandlerPtr, rBcPeerSession.m_nHandleIDNum - 1);
						for (ACE_UINT32 k = 0; k < rBcPeerSession.m_nHandleIDNum; ++k)
						{
							if (rBcPeerSession.m_ppMcpHandler[k] != pMcpHandler)
							{
								ppMcpHandler[n++] = rBcPeerSession.m_ppMcpHandler[k];
							}
						}
						DSC_DELETE_ARRAY(rBcPeerSession.m_ppMcpHandler);
						rBcPeerSession.m_ppMcpHandler = ppMcpHandler;
						--rBcPeerSession.m_nHandleIDNum;
						if (rBcPeerSession.m_nIndex >= rBcPeerSession.m_nHandleIDNum)
						{
							rBcPeerSession.m_nIndex = 0;
						}
					}
					else
					{
						DSC_DELETE_ARRAY(rBcPeerSession.m_ppMcpHandler);
						rBcPeerSession.m_ppMcpHandler = NULL;

						if (m_vbhPeerConnSumInfo.m_nPeerSessionNum > 1)
						{
							CVbhPeerConnectionInfo* parrPeerSession;
							ACE_UINT32 n = 0;

							DSC_NEW_ARRAY(parrPeerSession, CVbhPeerConnectionInfo, m_vbhPeerConnSumInfo.m_nPeerSessionNum - 1);
							for (ACE_UINT32 k = 0; k < m_vbhPeerConnSumInfo.m_nPeerSessionNum; ++k)
							{
								if (m_vbhPeerConnSumInfo.m_parrPeerConnInfo[k].m_nPeerID != nPeerID)
								{
									parrPeerSession[n++] = m_vbhPeerConnSumInfo.m_parrPeerConnInfo[k];
								}
							}
							DSC_DELETE_ARRAY(m_vbhPeerConnSumInfo.m_parrPeerConnInfo);
							m_vbhPeerConnSumInfo.m_parrPeerConnInfo = parrPeerSession;
							--m_vbhPeerConnSumInfo.m_nPeerSessionNum;
							if (m_vbhPeerConnSumInfo.m_nIndex >= m_vbhPeerConnSumInfo.m_nPeerSessionNum)
							{
								m_vbhPeerConnSumInfo.m_nIndex = 0;
							}
						}
						else
						{
							DSC_DELETE_ARRAY(m_vbhPeerConnSumInfo.m_parrPeerConnInfo);
							m_vbhPeerConnSumInfo.m_parrPeerConnInfo = NULL;
							m_vbhPeerConnSumInfo.m_nPeerSessionNum = 0;
							m_vbhPeerConnSumInfo.m_nIndex = 0;
						}
					}
					break;
				}
			}
		}
	}

	//断连后，删除在该连接上的所有session
	CHandleSession* pHandleSession = m_mapHandleSession.Erase(pMcpHandler->GetHandleID());
	if (pHandleSession)
	{
		CDscTypeArray<IBaseSession>& arrSession = pHandleSession->m_arrSession;
		const ACE_UINT32 nSize = arrSession.Size();

		for (ACE_UINT32 i = 0; i < nSize; ++i)
		{
			arrSession[i]->OnNetError();
		}
		DSC_THREAD_TYPE_DEALLOCATE(pHandleSession);
	}

	//如果和所有的peer都断连了，通知上层应用 //TODO: 后面再定处理策略
	//if (!m_vbhPeerConnSumInfo.m_nPeerSessionNum)
	//{
	//	this->OnAbnormal();
	//}

	CDscHtsClientService::OnNetworkError(pMcpHandler);
}

ACE_INT32 CVbhClientService::GetPeerID(const ACE_UINT32 nHandleID)
{
	for (DSC::CDscList <CVbhPeer>::iterator peer_it = m_lstBcPeer.begin(); peer_it != m_lstBcPeer.end(); ++peer_it)
	{
		CVbhPeer& rPeer = *peer_it;

		for (DSC::CDscList <CEndorserAddr>::iterator svr_it = rPeer.m_lstEndorserAddr.begin(); svr_it != rPeer.m_lstEndorserAddr.end(); ++svr_it)
		{
			if (nHandleID == svr_it->m_nHandleID)
			{
				return rPeer.m_nPeerID;
			}
		}
	}

	return -1;
}

CMcpHandler* CVbhClientService::GetMcpHandler(void)
{
	if (m_vbhPeerConnSumInfo.m_nPeerSessionNum)
	{
		if (m_vbhPeerConnSumInfo.m_nIndex >= m_vbhPeerConnSumInfo.m_nPeerSessionNum)
		{
			m_vbhPeerConnSumInfo.m_nIndex = 0;
		}

		CVbhPeerConnectionInfo& rBcPeerSession = m_vbhPeerConnSumInfo.m_parrPeerConnInfo[m_vbhPeerConnSumInfo.m_nIndex];

		++m_vbhPeerConnSumInfo.m_nIndex;
		if (rBcPeerSession.m_nIndex >= rBcPeerSession.m_nHandleIDNum)
		{
			rBcPeerSession.m_nIndex = 0;
		}

		return rBcPeerSession.m_ppMcpHandler[rBcPeerSession.m_nIndex++];
	}

	return NULL;
}

class CBlockchainAddr
{
public:
	CBlockchainAddr()
		: m_IpAddr("ES_IP_ADDR")
		, m_port("ES_PORT")
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

class CBcAddrCriterion : public CSelectCriterion
{
public:
	virtual void SetCriterion(CPerSelect& rPerSelect)
	{
		rPerSelect.OrderByAsc("PEER_ID");
	}
};

ACE_INT32 CVbhClientService::LoadPeerInfo(void)
{
	CTableWrapper< CCollectWrapper<CBlockchainAddr> > lstEndorserAddr("ES_CFG");

	CDscDatabase database;
	CDBConnection dbConnection;
	CBcAddrCriterion criterion;

	if (CDscDatabaseFactoryDemon::instance()->CreateDatabase(database, dbConnection))
	{
		DSC_RUN_LOG_ERROR("connect database failed.");

		return -1;
	}

	if (::PerSelect(lstEndorserAddr, database, dbConnection, &criterion))
	{
		DSC_RUN_LOG_ERROR("select from ES_CFG failed");

		return -1;
	}

	CVbhPeer* pVbhPeer = NULL;
	ACE_INT32 nPeerID = std::numeric_limits<ACE_INT32>::min();

	for (auto it = lstEndorserAddr->begin(); it != lstEndorserAddr->end(); ++it)
	{
		if (nPeerID != *it->m_peerID)
		{
			CVbhPeer vbhPeer;

			m_lstBcPeer.push_back(vbhPeer);
			pVbhPeer = &m_lstBcPeer.back();

			nPeerID = *it->m_peerID;
			pVbhPeer->m_nPeerID = nPeerID;
		}

		CEndorserAddr endorserAddr;

		endorserAddr.m_nHandleID = this->AllocHandleID();
		endorserAddr.m_strIpAddr = *it->m_IpAddr;
		endorserAddr.m_nPort = *it->m_port;

		if (pVbhPeer)
		{
			pVbhPeer->m_lstEndorserAddr.push_back(endorserAddr);
		}
		else
		{
			DSC_RUN_LOG_ERROR("invalid PEER_ID value: %d", *it->m_peerID);

			return -1;
		}
	}

	return 0;
}
