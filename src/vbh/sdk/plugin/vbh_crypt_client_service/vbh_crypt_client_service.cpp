#include "dsc/mem_mng/dsc_allocator.h"
#include "dsc/configure/dsc_configure.h"

#include "common/vbh_comm_error_code.h"
#include "common/vbh_comm_msg_def.h"
#include "common/vbh_comm_wrap_msg_def.h"
#include "common/vbh_comm_func.h"
#include "common/vbh_encrypt_lib.h"

#include "vbh_crypt_client_service/vbh_crypt_client_service.h"

CVbhCryptClientService::IBaseSession::IBaseSession(CVbhCryptClientService& rRegistUserService)
	: m_rClientBaseService(rRegistUserService)
{
}

CVbhCryptClientService::CRegistUserSession::CRegistUserSession(CVbhCryptClientService& rClientBaseService)
	: IBaseSession(rClientBaseService)
{
}

CVbhCryptClientService::CProposeSession::CProposeSession(CVbhCryptClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

CVbhCryptClientService::CQueryUserSession::CQueryUserSession(CVbhCryptClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

CVbhCryptClientService::CQueryTransactionSession::CQueryTransactionSession(CVbhCryptClientService& rRegistUserService)
	: IBaseSession(rRegistUserService)
{
}

ACE_INT32 CVbhCryptClientService::OnInit(void)
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

ACE_INT32 CVbhCryptClientService::OnExit(void)
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

	if (m_pCallback)
	{
		m_pCallback->OnExit();
	} 

	return CDscHtsClientService::OnExit();
}

void CVbhCryptClientService::OnRegistUserResponse(ACE_INT32 nReturnCode, CRegistUserSession* pRegistUserSession, DSC::CDscBlob& genUserKey)
{
	DSC::CDscBlob userKey(pRegistUserSession->m_userKey.data(), pRegistUserSession->m_userKey.size());
	DSC::CDscBlob cryptUserKey(pRegistUserSession->m_encodedClientCryptKey.data(), pRegistUserSession->m_encodedClientCryptKey.size());

	this->OnRegistUserRsp(nReturnCode, pRegistUserSession->m_nChannelID, userKey, cryptUserKey, genUserKey);
}

void CVbhCryptClientService::OnRelease(CRegistUserSession* pRegistUserSession)
{
	//从所有的背书 handle-session中删除本 regist-user-session
	CHandleSession* pHandleSession;
	for (ACE_UINT32 idx = 0; idx < pRegistUserSession->m_arrHandleSession.Size(); ++idx)
	{
		pHandleSession = pRegistUserSession->m_arrHandleSession[idx];
		pHandleSession->m_arrSession.Erase(pRegistUserSession);
	}
	DSC_THREAD_TYPE_DELETE(pRegistUserSession);
}

void CVbhCryptClientService::OnTimeOut(CRegistUserSession* pRegistUserSession)
{
	m_mapRegistUserSession.Erase(pRegistUserSession);
	if (pRegistUserSession->m_bCompleteRegistRsp) //已经完成应答，等待事务完成后的最终通知
	{

	}
	else //没有完成背书，还在第一阶段
	{
		DSC::CDscBlob genUserKey;

		OnRegistUserResponse(VBH::EN_TIMEOUT_ERROR_TYPE, pRegistUserSession, genUserKey);
	}
	OnRelease(pRegistUserSession);
}

void CVbhCryptClientService::OnNetError(CRegistUserSession* pRegistUserSession)
{
	this->CancelDscTimer(pRegistUserSession);
	m_mapRegistUserSession.Erase(pRegistUserSession);
	if (pRegistUserSession->m_bCompleteRegistRsp) //已经完成应答，等待事务完成后的最终通知
	{

	}
	else //没有完成背书，还在第一阶段
	{
		DSC::CDscBlob genUserKey;

		OnRegistUserResponse(VBH::EN_NETWORK_ERROR_TYPE, pRegistUserSession, genUserKey);
	}
	OnRelease(pRegistUserSession);
}

void CVbhCryptClientService::OnProposeResponse(const ACE_INT32 nReturnCode, CProposeSession* pProposeSession, const ACE_UINT16 nOrderID, const ACE_UINT32 nSequenceNumber,
	const ACE_UINT32 nTransIdx, const ACE_UINT64 nBlockID, DSC::CDscShortList<VBH::CConciseOnCreateInformationKey>& lstCreatedInfoKey)
{
	DSC::CDscBlob userKey(pProposeSession->m_userKey.data(), pProposeSession->m_userKey.size());
	DSC::CDscBlob blobTransKey;
	VBH::COrderGenerateTransactionKey transKey;
	DSC::CDscShortList<VBH::COnCreateKey> lstOnCreateKey;
	char* pTransKey;
	size_t nTransKeyLen;

	transKey.m_nOrderID = nOrderID;
	transKey.m_nSequenceNumber = nSequenceNumber;
	transKey.m_nTransIdx = nTransIdx;
	transKey.m_nBlockID = nBlockID;
	DEF_ENCODE(transKey, pTransKey, nTransKeyLen);
	blobTransKey.Set(pTransKey, nTransKeyLen);

	this->OnProposeRsp(nReturnCode, pProposeSession->m_nChannelID, userKey, blobTransKey);
}

void CVbhCryptClientService::OnRelease(CProposeSession* pProposeSession)
{
	//从所有的背书 handle-session中删除本 propose-session
	CHandleSession* pHandleSession;
	for (ACE_UINT32 idx = 0; idx < pProposeSession->m_arrHandleSession.Size(); ++idx)
	{
		pHandleSession = pProposeSession->m_arrHandleSession[idx];
		pHandleSession->m_arrSession.Erase(pProposeSession);
	}

	DSC_THREAD_TYPE_DEALLOCATE(pProposeSession);
}

void CVbhCryptClientService::OnTimeOut(CProposeSession* pProposeSession)
{
	m_mapProposeSession.Erase(pProposeSession);
	if (pProposeSession->m_bCompleteRegistRsp)
	{

	}
	else
	{

		OnProposeResponse(VBH::EN_TIMEOUT_ERROR_TYPE, pProposeSession, 0, 0, 0, 0, lstCreatedInfoKey);
		this->OnProposeRsp(nReturnCode, pProposeSession->m_nChannelID, userKey, blobTransKey);
	}
	OnRelease(pProposeSession);
}

void CVbhCryptClientService::OnNetError(CProposeSession* pProposeSession)
{
	this->CancelDscTimer(pProposeSession);
	m_mapProposeSession.Erase(pProposeSession);
	if (pProposeSession->m_bCompleteRegistRsp)
	{

	}
	else
	{
		DSC::CDscShortList<VBH::CConciseOnCreateInformationKey> lstCreatedInfoKey;

		OnProposeResponse(VBH::EN_NETWORK_ERROR_TYPE, pProposeSession, 0, 0, 0, 0, lstCreatedInfoKey);
	}
	OnRelease(pProposeSession);
}

void CVbhCryptClientService::OnRelease(CQueryUserSession* pQueryUserSession, ACE_INT32 nReturnCode, DSC::CDscBlob& userInfo)
{
	DSC::CDscBlob userKey(pQueryUserSession->m_userKey.data(), pQueryUserSession->m_userKey.size());

	this->OnGetUserInfoRsp(nReturnCode, pQueryUserSession->m_nChannelID, userKey, userInfo);
	pQueryUserSession->m_pHandleSession->m_arrSession.Erase(pQueryUserSession);
	DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
}

void CVbhCryptClientService::OnTimeOut(CQueryUserSession* pQueryUserSession)
{
	this->CancelDscTimer(pQueryUserSession);
	m_mapQueryUserSession.Erase(pQueryUserSession);

	DSC::CDscBlob userInfo;

	OnRelease(pQueryUserSession, VBH::EN_TIMEOUT_ERROR_TYPE, userInfo);
}

void CVbhCryptClientService::OnNetError(CQueryUserSession* pQueryUserSession)
{
	DSC::CDscBlob userInfo;

	this->CancelDscTimer(pQueryUserSession);
	m_mapQueryUserSession.Erase(pQueryUserSession);
	OnRelease(pQueryUserSession, VBH::EN_NETWORK_ERROR_TYPE, userInfo);
}

void CVbhCryptClientService::OnRelease(CQueryTransactionSession* pQueryTransSession, ACE_INT32 nReturnCode, DSC::CDscBlob& transInfo)
{
	DSC::CDscBlob transKey(pQueryTransSession->m_transKey.data(), pQueryTransSession->m_transKey.size());
	DSC::CDscBlob userKey(pQueryTransSession->m_userKey.data(), pQueryTransSession->m_userKey.size());

	OnGetTransInfoRsp(nReturnCode, pQueryTransSession->m_nChannelID, userKey, transKey, transInfo);
	pQueryTransSession->m_pHandleSession->m_arrSession.Erase(pQueryTransSession);
	DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
}

void CVbhCryptClientService::OnTimeOut(CQueryTransactionSession* pQueryTransSession)
{
	DSC::CDscBlob transInfo;

	m_mapQueryTransSession.Erase(pQueryTransSession);
	OnRelease(pQueryTransSession, VBH::EN_TIMEOUT_ERROR_TYPE, transInfo);
}

void CVbhCryptClientService::OnNetError(CQueryTransactionSession* pQueryTransSession)
{
	DSC::CDscBlob transInfo;

	this->CancelDscTimer(pQueryTransSession);
	m_mapQueryTransSession.Erase(pQueryTransSession);
	OnRelease(pQueryTransSession, VBH::EN_NETWORK_ERROR_TYPE, transInfo);
}

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptRegistUserEsCltRsp& rRegistUserRsp, CMcpHandler* pMcpHandler)
{
	CRegistUserSession* pRegistUserSession = m_mapRegistUserSession.Find(rRegistUserRsp.m_nCltSessionID);

	//1.找session
	if (pRegistUserSession)
	{
		ACE_INT32 nReturnCode = rRegistUserRsp.m_nReturnCode;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			//2.解密、解码
			int nDecryptBufLen;
			char* pDecryptBuf = VBH::vbhDecrypt((unsigned char*)m_peerEnvelopeKey.data(), nDecryptBufLen, rRegistUserRsp.m_userData.GetBuffer(), rRegistUserRsp.m_userData.GetSize());

			if (pDecryptBuf)
			{
				bool bAllowRegist;
				DSC::CDscBlob userInitInfo;
				DSC::CDscBlob nonce;
				DSC::CDscBlob svrPubKey;
				DSC::CDscBlob cltPriKey;
				DSC::CDscBlob envelopeKey;

				VBH::CCryptRegistUserEsCltRspDataWrapper wrapper(bAllowRegist, userInitInfo, nonce, svrPubKey, cltPriKey, envelopeKey);

				if (DSC::Decode(wrapper, pDecryptBuf, nDecryptBufLen))
				{
					DSC_RUN_LOG_ERROR("decode error.");
					nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
				}
				else
				{
					if (VBH::IsEqual(pRegistUserSession->m_strNonce, nonce))
					{
						if (bAllowRegist) //同意创建
						{
							//3.背书
							if (pRegistUserSession->m_ccGenerateUserInfo.empty()) //初次收到应答
							{
								VBH::Assign(pRegistUserSession->m_ccGenerateUserInfo, userInitInfo);
								++pRegistUserSession->m_nSucceedRspNum;
							}
							else//第N次收到应答 N>1
							{
								if (VBH::IsEqual(pRegistUserSession->m_ccGenerateUserInfo, userInitInfo))
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

								//4.生成秘钥文件
								if (VBH::EncodeClientCryptKey(pRegistUserSession->m_encodedClientCryptKey, svrPubKey, cltPriKey, envelopeKey))
								{
									DSC_RUN_LOG_ERROR("encode client crypt key error.");
									nReturnCode = VBH::EN_SYSTEM_ERROR_TYPE;
								}
							}

							//TODO: 背书策略或许会调整
							if ((nReturnCode == VBH::EN_OK_TYPE) && (pRegistUserSession->m_nSucceedRspNum >= m_nEndorsePeerCount)) //背书通过
							{
								//组织回应答(再次提交注册事务)							
								int nEncryptBufLen;
								char* pEncryptBuf = VBH::vbhEncrypt((unsigned char*)m_peerEnvelopeKey.data(), nEncryptBufLen,
									pRegistUserSession->m_strNonce.data(), pRegistUserSession->m_strNonce.size());

								if (DSC_LIKELY(pEncryptBuf))
								{
									//5.向服务端提交
									VBH::CCryptSubmitRegistTransactionCltEsReq req;

									req.m_nEsSessionID = pRegistUserSession->m_nEsSessionID;
									req.m_userData.Set(pEncryptBuf, nEncryptBufLen);

									if (DSC_UNLIKELY(this->SendHtsMsg(req, pRegistUserSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler)))
									{
										DSC_RUN_LOG_ERROR("send hts msg failed.");
										nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
									}

									DSC_THREAD_FREE(pEncryptBuf);
								}
								else
								{
									DSC_RUN_LOG_ERROR("encrypt error.");
									nReturnCode = VBH::EN_ENCRYPT_ERROR_TYPE;
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
						DSC_RUN_LOG_WARNING("nonce is error.");
						nReturnCode = VBH::EN_NONCE_VERIFY_ERROR_TYPE;
					}
				}

				DSC_THREAD_FREE(pDecryptBuf);
			}
			else
			{
				DSC_RUN_LOG_ERROR("decrypt error.");
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
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
				VBH::CCryptCancelRegistUserCltEsReq req;
				int nEncryptBufLen;
				char* pEncryptBuf = VBH::vbhEncrypt((unsigned char*)m_peerEnvelopeKey.data(), nEncryptBufLen,
					pRegistUserSession->m_strNonce.data(), pRegistUserSession->m_strNonce.size());

				if (pEncryptBuf)
				{
					req.m_nEsSessionID = pRegistUserSession->m_nEsSessionID;
					req.m_userData.Set(pEncryptBuf, nEncryptBufLen);

					this->SendHtsMsg(req, pRegistUserSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler);

					DSC_THREAD_FREE(pEncryptBuf);
				}
				else
				{
					DSC_RUN_LOG_ERROR("encrypt error.");
				}
			}

			//向客户端上层业务应答，//删除session
			DSC::CDscBlob genUserKey;

			this->CancelDscTimer(pRegistUserSession);
			m_mapRegistUserSession.Erase(pRegistUserSession);
			OnRegistUserResponse(nReturnCode, pRegistUserSession, genUserKey);
			OnRelease(pRegistUserSession); //删除session
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("can not find session, session-id:%d", pRegistUserSession->m_nCltSessionID);
	}

	return 0;
}

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptSubmitRegistTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler)
{
	CRegistUserSession* pRegistUserSession = m_mapRegistUserSession.Find(rSubmitTransRsp.m_nCltSessionID);

	if (pRegistUserSession)
	{
		ACE_INT32 nReturnCode = rSubmitTransRsp.m_nReturnCode;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			//1. 解密数据
			int nDecryptBufLen;
			char* pDecryptBuf = VBH::vbhDecrypt((unsigned char*)pRegistUserSession->m_envelopeKey.data(), nDecryptBufLen,
				rSubmitTransRsp.m_userData.GetBuffer(), rSubmitTransRsp.m_userData.GetSize());

			if (DSC_LIKELY(pDecryptBuf))
			{
				VBH::COrderGenerateKey orderGenKey;
				DSC::CDscBlob nonce;
				VBH::CCryptSubmitRegistTransactionEsCltRspDataWrapper wrapper(orderGenKey, nonce);

				//2. 解码解密后的数据，得到应答实际内容
				if (DSC::Decode(wrapper, pDecryptBuf, nDecryptBufLen))
				{
					nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
					DSC_RUN_LOG_INFO("decode error.");
				}
				else
				{
					//3. 校验nonce
					if (VBH::IsEqual(pRegistUserSession->m_strNonce, nonce))
					{
						DSC::CDscBlob genUserKey;
						char* pGenUserKey;
						size_t nGenUserKeyLen;

						DEF_ENCODE(orderGenKey, pGenUserKey, nGenUserKeyLen);
						genUserKey.Set(pGenUserKey, nGenUserKeyLen);

						//返回给用户应答
						this->OnRegistUserResponse(nReturnCode, pRegistUserSession, genUserKey);

						if (pRegistUserSession->m_bWaitNotify)//如果客户等待通知，则重置定时器，
						{
							this->ResetDscTimer(pRegistUserSession, EN_SESSION_WAIT_NOTIFY_TIMEOUT_VALUE);
							pRegistUserSession->m_bCompleteRegistRsp = true;
						}
						else //客户不等待通知，则 ，删除session 
						{
							this->CancelDscTimer(pRegistUserSession);
							m_mapRegistUserSession.Erase(pRegistUserSession);
							this->OnRelease(pRegistUserSession);
						}
					}
					else
					{
						nReturnCode = VBH::EN_NONCE_VERIFY_ERROR_TYPE;
						DSC_RUN_LOG_INFO("verify nonce failed.");
					}
				}

				DSC_THREAD_FREE(pDecryptBuf);
			}
			else
			{
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
				DSC_RUN_LOG_INFO("decrypt_error");
			}
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

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptUnRegistUserRsp& rUnRegistUserRsp, CMcpHandler* pMcpHandler)
{

	return 0;
}

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptSubmitProposalEsCltRsp& rSubmitProposalRsp, CMcpHandler* pMcpHandler)
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
			int nDecryptBufLen;
			char* pDecryptBuf = VBH::vbhDecrypt((unsigned char*)pProposeSession->m_envelopeKey.data(), nDecryptBufLen, rSubmitProposalRsp.m_userData.GetBuffer(), rSubmitProposalRsp.m_userData.GetSize());

			if (DSC_LIKELY(pDecryptBuf))
			{
				DSC::CDscBlob transContent; //事务内容， 参与背书
				DSC::CDscBlob nonce;
				VBH::CCryptSubmitProposalEsCltRspDataWrapper userData(transContent, nonce);

				if (DSC_UNLIKELY(DSC::Decode(userData, pDecryptBuf, nDecryptBufLen)))
				{
					DSC_RUN_LOG_ERROR("decode error.");
					nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
				}
				else
				{
					if (VBH::IsEqual(pProposeSession->m_strNonce, nonce))
					{
						if (pProposeSession->m_transContent.empty())
						{
							VBH::Assign(pProposeSession->m_transContent, transContent);
							++pProposeSession->m_nSucceedRspNum;
						}
						else
						{
							if (pProposeSession->m_transContent.IsEqual(transContent.GetBuffer(), transContent.GetSize()))
							{
								++pProposeSession->m_nSucceedRspNum;
							}
							else
							{
								DSC_RUN_LOG_ERROR("endorse failed.");
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
							int nEncryptBufLen;
							char* pEncryptBuf = VBH::vbhEncrypt((unsigned char*)pProposeSession->m_envelopeKey.data(), nEncryptBufLen,
								pProposeSession->m_strNonce.data(), pProposeSession->m_strNonce.size());

							if (DSC_LIKELY(pEncryptBuf))
							{
								VBH::CCryptSubmitProposalTransactionCltEsReq req;

								req.m_nEsSessionID = pProposeSession->m_nEsSessionID;
								req.m_userData.Set(pEncryptBuf, nEncryptBufLen);

								if (DSC_UNLIKELY(this->SendHtsMsg(req, pProposeSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler)))
								{
									DSC_RUN_LOG_ERROR("send hts msg failed.");
									nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
								}

								DSC_THREAD_FREE(pEncryptBuf);
							}
							else
							{
								DSC_RUN_LOG_ERROR("encrypt error.");
								nReturnCode = VBH::EN_ENCRYPT_ERROR_TYPE;
							}
						}
						else
						{
							//无需处理，再等下一应答消息
						}
					}
					else
					{
						DSC_RUN_LOG_WARNING("nonce is error.");
						nReturnCode = VBH::EN_NONCE_VERIFY_ERROR_TYPE;
					}
				}
				DSC_THREAD_FREE(pDecryptBuf);
			}
			else
			{
				DSC_RUN_LOG_ERROR("decrypt error.");
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("submit proposal rsp return failed, error code:%d, channelID:%d",
				rSubmitProposalRsp.m_nReturnCode, pProposeSession->m_nChannelID);
		}

		if (nReturnCode != VBH::EN_OK_TYPE)
		{
			// 不是网络错误的情况下， 出错后，向选中的提交节点发送取消事务的消息
			if (nReturnCode != VBH::EN_NETWORK_ERROR_TYPE)
			{
				VBH::CCryptCancelProposalTransactionCltEsReq req;
				int nEncryptBufLen;
				char* pEncryptBuf = VBH::vbhEncrypt((unsigned char*)m_peerEnvelopeKey.data(), nEncryptBufLen, pProposeSession->m_strNonce.data(), pProposeSession->m_strNonce.size());

				if (pEncryptBuf)
				{
					req.m_nEsSessionID = pProposeSession->m_nEsSessionID;
					req.m_userData.Set(pEncryptBuf, nEncryptBufLen);

					this->SendHtsMsg(req, pProposeSession->m_pSubmitEsMcpHandleSession->m_pMcpHandler);

					DSC_THREAD_FREE(pEncryptBuf);
				}
				else
				{
					DSC_RUN_LOG_ERROR("encrypt error.");
				}
			}

			//向客户端上层通知 //删除session
			DSC::CDscShortList<VBH::CConciseOnCreateInformationKey> lstCreatedInfoKey;

			this->CancelDscTimer(pProposeSession);
			m_mapProposeSession.Erase(pProposeSession);
			OnProposeResponse(nReturnCode, pProposeSession, 0, 0, 0, 0, lstCreatedInfoKey);
			OnRelease(pProposeSession);
		}
	}

	return 0;
}

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptSubmitProposalTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler)
{
	CProposeSession* pProposeSession = m_mapProposeSession.Find(rSubmitTransRsp.m_nCltSessionID);

	if (pProposeSession)
	{
		ACE_INT32 nReturnCode = rSubmitTransRsp.m_nReturnCode;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			//1. 解密数据
			int nDecryptBufLen;
			char* pDecryptBuf = VBH::vbhDecrypt((unsigned char*)pProposeSession->m_envelopeKey.data(), nDecryptBufLen,
				rSubmitTransRsp.m_userData.GetBuffer(), rSubmitTransRsp.m_userData.GetSize());

			if (DSC_LIKELY(pDecryptBuf))
			{
				VBH::COrderGenerateTransactionKey genTransKey;
				DSC::CDscBlob nonce;
				VBH::CCryptSubmitProposalTransactionEsCltRspDataWrapper wrapper(genTransKey, nonce);

				//2. 解码解密后的数据，得到应答实际内容
				if (DSC::Decode(wrapper, pDecryptBuf, nDecryptBufLen))
				{
					nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
					DSC_RUN_LOG_INFO("decode error.");
				}
				else
				{
					//3. 校验nonce
					if (VBH::IsEqual(pProposeSession->m_strNonce, nonce))
					{
						//返回提案应答
						this->OnProposeResponse(nReturnCode, pProposeSession, wrapper.m_nOrderID, wrapper.m_nSequenceNumber, wrapper.m_nTransIdx, wrapper.m_nBlockID, lstCreatedInfoKey);

						if (pProposeSession->m_bWaitNotify)//如果客户等待通知，则重置定时器，//TODO: 另起session
						{
							this->ResetDscTimer(pProposeSession, EN_SESSION_WAIT_NOTIFY_TIMEOUT_VALUE);
							pProposeSession->m_bCompleteRegistRsp = true;
						}
						else //客户不等待通知，则删除session
						{
							this->CancelDscTimer(pProposeSession);
							m_mapProposeSession.Erase(pProposeSession);
							this->OnRelease(pProposeSession);
						}
					}
					else
					{
						nReturnCode = VBH::EN_NONCE_VERIFY_ERROR_TYPE;
						DSC_RUN_LOG_INFO("verify nonce failed.");
					}
				}

				DSC_THREAD_FREE(pDecryptBuf);
			}
			else
			{
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
				DSC_RUN_LOG_INFO("decrypt_error");
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("submit proposal transaction rsp return failed, error code:%d, channelID:%d", rSubmitTransRsp.m_nReturnCode, pProposeSession->m_nChannelID);
		}
	}
	else
	{
		DSC_RUN_LOG_WARNING("can not find propose session, session-id:%d", rSubmitTransRsp.m_nCltSessionID);
	}

	return 0;
}

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptQueryUserInfoEdsCltRsp& rQueryUserRsp, CMcpHandler* pMcpHandler)
{
	CQueryUserSession* pUserSession = m_mapQueryUserSession.Erase(rQueryUserRsp.m_nSessionID);

	if (DSC_LIKELY(pUserSession))
	{
		ACE_INT32 nReturnCode = rQueryUserRsp.m_nReturnCode;
		char* pDecryptBuf;
		DSC::CDscBlob userInfo;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			int nDecryptBufLen;

			pDecryptBuf = VBH::vbhDecrypt((unsigned char*)pUserSession->m_envelopeKey.data(), nDecryptBufLen, rQueryUserRsp.m_userData.GetBuffer(), rQueryUserRsp.m_userData.GetSize());
			if (DSC_LIKELY(pDecryptBuf))
			{
				DSC::CDscBlob nonce;
				VBH::CCryptQueryUserInfoRspWrapper userData(userInfo, nonce);

				if (DSC::Decode(userData, pDecryptBuf, nDecryptBufLen))
				{
					DSC_RUN_LOG_ERROR("decode error.");
					nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
				}
				else
				{
					if (!VBH::IsEqual(pUserSession->m_strNonce, nonce))
					{
						DSC_RUN_LOG_ERROR("verify nonce failed.");
						nReturnCode = VBH::EN_NONCE_VERIFY_ERROR_TYPE;
					}
				}
			}
			else
			{
				DSC_RUN_LOG_ERROR("decrypt error.");
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
			}
		}
		else
		{
			DSC_RUN_LOG_ERROR("query user info rsp return failed, error code:%d, channelID:%d", rQueryUserRsp.m_nReturnCode, pUserSession->m_nChannelID);
			pDecryptBuf = NULL;
		}

		this->CancelDscTimer(pUserSession);
		this->OnRelease(pUserSession, nReturnCode, userInfo);

		if (pDecryptBuf)
		{
			DSC_THREAD_FREE(pDecryptBuf);
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("can not find query user session.");
	}

	return 0;
}

ACE_INT32 CVbhCryptClientService::OnHtsMsg(VBH::CCryptQueryTransInfoEsCltRsp& rQueryTransRsp, CMcpHandler* pMcpHandler)
{
	CQueryTransactionSession* pQuerySession = m_mapQueryTransSession.Erase(rQueryTransRsp.m_nCltSessionID);

	if (DSC_LIKELY(pQuerySession))
	{
		ACE_INT32 nReturnCode = rQueryTransRsp.m_nReturnCode;
		char* pDecryptBuf;
		DSC::CDscBlob transInfo;

		if (nReturnCode == VBH::EN_OK_TYPE)
		{
			int nDecryptBufLen;

			pDecryptBuf = VBH::vbhDecrypt((unsigned char*)pQuerySession->m_envelopeKey.data(), nDecryptBufLen, rQueryTransRsp.m_userData.GetBuffer(), rQueryTransRsp.m_userData.GetSize());
			if (DSC_LIKELY(pDecryptBuf))
			{
				DSC::CDscBlob nonce;
				VBH::CCryptQueryTransactionRspDataWrapper wrapper(nonce, transInfo);

				if (DSC_UNLIKELY(DSC::Decode(wrapper, pDecryptBuf, nDecryptBufLen)))
				{
					nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
					DSC_RUN_LOG_INFO("decode error.");
				}
				else
				{
					if (!VBH::IsEqual(pQuerySession->m_strNonce, nonce))
					{
						nReturnCode = VBH::EN_NONCE_VERIFY_ERROR_TYPE;
						DSC_RUN_LOG_INFO("verify nonce failed.");
					}
				}
			}
			else
			{
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("query transaction rsp return failed, error code:%d, channelID:%d", rQueryTransRsp.m_nReturnCode, pQuerySession->m_nChannelID);
			pDecryptBuf = NULL;
		}

		this->CancelDscTimer(pQuerySession);
		this->OnRelease(pQuerySession, nReturnCode, transInfo);

		if (pDecryptBuf)
		{
			DSC_THREAD_FREE(pDecryptBuf);
		}
	}
	else
	{
		DSC_RUN_LOG_WARNING("query user rsp timeout");
	}

	return 0;
}

void CVbhCryptClientService::OnDscMsg(VBH::CInitBcUserSdk& rInitUserSdk, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	if (m_pCallback)
	{
		DSC_RUN_LOG_WARNING("repleat init regist user msg");
	}
	else
	{
		m_pCallback = (VBH_SDK::IClientSdkMsgCallback*)rInitUserSdk.m_pCallBack;

		if(m_bReady)
		{
			m_pCallback->OnReady();
		}
	}
}

void CVbhCryptClientService::OnDscMsg(VBH::CRegistUserApiCltReq& rRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = this->RegistUser(rRegistUserReq);

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH_SDK::CRegistUserRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nChannelID = rRegistUserReq.m_nChannelID;

		rsp.m_pUserKey = rRegistUserReq.m_userKey.GetBuffer();
		rsp.m_nUserKeyLen = rRegistUserReq.m_userKey.GetSize();

		rsp.m_pGenUserKey = NULL;
		rsp.m_nGenUserKeyLen = 0;

		m_pCallback->OnRegistUserRsp(rsp);

		DSC_RUN_LOG_WARNING("regist user failed, error code:%d", nReturnCode);
	}
}

void CVbhCryptClientService::OnDscMsg(VBH::CCryptUnRegistUserReq& rUnRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = this->UnRegistUser(rUnRegistUserReq);

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH_SDK::CUnRegistUserRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nChannelID = rUnRegistUserReq.m_nChannelID;

		//TODO: 取消注册的完整性暂时不考虑
		//rsp.m_pUserKey = rUnRegistUserReq.m_userKey.GetBuffer();
		//rsp.m_nUserKeyLen = rUnRegistUserReq.m_userKey.GetSize();

		m_pCallback->OnUnRegistUserRsp(rsp);

		DSC_RUN_LOG_INFO("regist user failed, error code:%d", nReturnCode);
	}
}

void CVbhCryptClientService::OnDscMsg(VBH::CProposeApiCltReq& rProposeApiCltReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = this->Propose(rProposeApiCltReq);

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH_SDK::CProposeRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nChannelID = rProposeApiCltReq.m_nChannelID;

		rsp.m_pUserKey = rProposeApiCltReq.m_userKey.GetBuffer();
		rsp.m_nUserKeyLen = rProposeApiCltReq.m_userKey.GetSize();

		m_pCallback->OnLaunchTransRsp(rsp);

		DSC_RUN_LOG_WARNING("set user info failed, error code:%d, error string:%s", nReturnCode, VBH::GetErrorString(nReturnCode));
	}
}

void CVbhCryptClientService::OnDscMsg(VBH::CQueryUserInfoApiCltReq& rQuerUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = this->GetUserInfo(rQuerUserReq);

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH_SDK::CGetUserInfoRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nChannelID = rQuerUserReq.m_nChannelID;

		rsp.m_pUserKey = rQuerUserReq.m_userKey.GetBuffer();
		rsp.m_nUserKeyLen = rQuerUserReq.m_userKey.GetSize();

		rsp.m_pUserUserInfo = NULL;
		rsp.m_nUserUserInfoLen = 0;

		m_pCallback->OnGetUserInfoRsp(rsp);

		DSC_RUN_LOG_WARNING("get user info failed, error code:%d, error string:%s", rsp.m_nReturnCode, VBH::GetErrorString(rsp.m_nReturnCode));
	}
}

void CVbhCryptClientService::OnDscMsg(VBH::CQueryTransInfoApiCltReq& rQueryTransReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = this->GetTransInfo(rQueryTransReq);

	if (DSC_UNLIKELY(nReturnCode))
	{
		VBH_SDK::CGetTransactionInfoRsp rsp;

		rsp.m_nReturnCode = nReturnCode;
		rsp.m_nChannelID = rQueryTransReq.m_nChannelID;
		
		rsp.m_pUserKey = rQueryTransReq.m_userKey.GetBuffer();
		rsp.m_nUserKeyLen = rQueryTransReq.m_userKey.GetSize();

		rsp.m_pTransKey = rQueryTransReq.m_transKey.GetBuffer();
		rsp.m_nTransKeyLen = rQueryTransReq.m_transKey.GetSize();

		rsp.m_pTransInfo = NULL;
		rsp.m_nTransInfoLen = 0;

		m_pCallback->OnGetTransInfoRsp(rsp);

		DSC_RUN_LOG_WARNING("get transaction failed, error code:%d", nReturnCode);
	}
}

ACE_INT32 CVbhCryptClientService::OnConnectedNodify(CMcpClientHandler* pMcpClientHandler)
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

	if (bReady)
	{
		this->OnReady();
	}

	return 0;
}

void CVbhCryptClientService::OnNetworkError(CMcpHandler* pMcpHandler)
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

	//如果和所有的peer都断连了，通知上层应用
	if (!m_vbhPeerConnSumInfo.m_nPeerSessionNum)
	{
		this->OnAbnormal();
	}

	CDscHtsClientService::OnNetworkError(pMcpHandler);
}

ACE_INT32 CVbhCryptClientService::RegistUser(VBH::CRegistUserApiCltReq& rRegistUserReq)
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

		VBH::Assign(pRegistUserSession->m_userKey, rRegistUserReq.m_userKey);
		VBH::Assign(pRegistUserSession->m_userInfo, rRegistUserReq.m_userInfo);
		pRegistUserSession->m_strNonce = ACE_OS::rand_r(&m_seed);
		pRegistUserSession->m_nCcID = rRegistUserReq.m_nCcID;
		pRegistUserSession->m_nChannelID = rRegistUserReq.m_nChannelID;
		pRegistUserSession->m_bWaitNotify = rRegistUserReq.m_bWaitNotify;

		bool bSubmitNode; //标记是否是被选中的
		DSC::CDscBlob nonce(pRegistUserSession->m_strNonce.data(), pRegistUserSession->m_strNonce.size());
		VBH::CCryptRegistUserCltEsReqDataWrapper wrapper(rRegistUserReq.m_bWaitNotify, bSubmitNode, pRegistUserSession->m_nCcID, pRegistUserSession->m_nChannelID, rRegistUserReq.m_userKey, rRegistUserReq.m_userInfo, nonce);

		char* pTrueMsgBuf;
		size_t nTrueMsgBufLen; //标记被选中的消息 编码后缓冲区
		char* pFalseMsgBuf;
		size_t nFalseMsgBufLen; //标记未被选中的消息 编码后缓冲区

		bSubmitNode = true;
		DSC::Encode(wrapper, pTrueMsgBuf, nTrueMsgBufLen);

		bSubmitNode = false;
		DSC::Encode(wrapper, pFalseMsgBuf, nFalseMsgBufLen);
		//如果Encode失败，则表明系统已经出了严重的问题，正常情况下是不会的；故对Encode执行结果不做判断

		char* pEncryptTrueMsgBuf;
		int nEncryptTrueMsgBufLen;
		char* pEncryptFalseMsgBuf;
		int nEncryptFalseMsgBufLen;

		//TODO: 将来肯定要调整，暂时先用从数据库中读取到的对称秘钥对数据进行加密
		pEncryptTrueMsgBuf = VBH::vbhEncrypt((unsigned char*)m_peerEnvelopeKey.data(), nEncryptTrueMsgBufLen, pTrueMsgBuf, nTrueMsgBufLen);
		pEncryptFalseMsgBuf = VBH::vbhEncrypt((unsigned char*)m_peerEnvelopeKey.data(), nEncryptFalseMsgBufLen, pFalseMsgBuf, nFalseMsgBufLen);

		DSC_THREAD_SIZE_FREE(pTrueMsgBuf, nTrueMsgBufLen);
		DSC_THREAD_SIZE_FREE(pFalseMsgBuf, nFalseMsgBufLen); //释放Encode时释放的资源

		if (DSC_LIKELY(pEncryptTrueMsgBuf && pEncryptFalseMsgBuf))
		{
			VBH::CCryptRegistUserCltEsReq req;

			req.m_nCltSessionID = m_nSessionID;

			CMcpHandler* pMcpHandler;
			ACE_UINT32 nHandleID;
			CHandleSession* pHandleSession;
			const ACE_INT32 nRand = abs(ACE_OS::rand_r(&m_seed)) % m_nEndorsePeerCount; //TODO:将来背书策略有调整

			for (ACE_INT32 i = 0; i < m_nEndorsePeerCount; ++i)
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

				if (nRand == i)
				{
					pRegistUserSession->m_pSubmitEsMcpHandleSession = pHandleSession;
					req.m_userData.Set(pEncryptTrueMsgBuf, nEncryptTrueMsgBufLen);
				}
				else
				{
					req.m_userData.Set(pEncryptFalseMsgBuf, nEncryptFalseMsgBufLen);
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
		else
		{
			DSC_RUN_LOG_ERROR("encrypt error.");
			DSC_THREAD_TYPE_DELETE(pRegistUserSession);

			nReturnCode = VBH::EN_SYSTEM_ERROR_TYPE;
		}

		if (pEncryptTrueMsgBuf)
		{
			DSC_THREAD_FREE(pEncryptTrueMsgBuf);
		}

		if (pEncryptFalseMsgBuf)
		{
			DSC_THREAD_FREE(pEncryptFalseMsgBuf);
		}
	}

	return nReturnCode;
}

ACE_INT32 CVbhCryptClientService::UnRegistUser(VBH::CCryptUnRegistUserReq& rUnRegistUserReq)
{
	//TODO: 
	return -1;
}

ACE_INT32 CVbhCryptClientService::Propose(VBH::CProposeApiCltReq& rProposeReq)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;

	if (m_vbhPeerConnSumInfo.m_nPeerSessionNum < m_nEndorsePeerCount)
	{
		DSC_RUN_LOG_WARNING("connected peer num:%d cann't less than configed endorse peer count:%d", m_vbhPeerConnSumInfo.m_nPeerSessionNum, m_nEndorsePeerCount);
		nReturnCode = VBH::EN_NETWORK_ERROR_TYPE;
	}
	else
	{
		CProposeSession* pProposeSession = DSC_THREAD_TYPE_NEW(CProposeSession) CProposeSession(*this);
		CDscString svrPubKey; //服务端公钥
		CDscString cltPriKey; //客户端私钥 //session中目前不保存着两个秘钥，所以声明为栈变量

		if (DSC_UNLIKELY(VBH::DecodeClientCryptKey(rProposeReq.m_cryptUserKey, svrPubKey, cltPriKey, pProposeSession->m_envelopeKey)))
		{
			DSC_THREAD_TYPE_DELETE(pProposeSession);
			DSC_RUN_LOG_ERROR("decode client crypt key error.");
			nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
		}
		else
		{
			VBH::CCryptSubmitProposalCltEsReq req;

			if (DSC::Decode(req.m_genUserKey, rProposeReq.m_userKey.GetBuffer(), rProposeReq.m_userKey.GetSize())) //从字符串中解码出系统生成的user-key
			{
				DSC_THREAD_TYPE_DELETE(pProposeSession);
				DSC_RUN_LOG_ERROR("decode generate user key error.");
				nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
			}
			else
			{
				char* pTrueMsgBuf;
				size_t nTrueMsgBufLen;
				char* pFalseMsgBuf;
				size_t nFalseMsgBufLen;

				bool bSubmitNode; //通知endorser保留session，用于后续commit transaction

				pProposeSession->m_strNonce = ACE_OS::rand_r(&m_seed);

				DSC::CDscBlob nonce(pProposeSession->m_strNonce.data(), pProposeSession->m_strNonce.size());

				VBH::CCryptSubmitProposalCltEsReqDataWrapper proposalWrapper(pProposeSession->m_bWaitNotify, bSubmitNode, rProposeReq.m_nCcID,
					rProposeReq.m_nActionID, rProposeReq.m_proposal, nonce);

				bSubmitNode = true;
				DSC::Encode(proposalWrapper, pTrueMsgBuf, nTrueMsgBufLen);
				bSubmitNode = false;
				DSC::Encode(proposalWrapper, pFalseMsgBuf, nFalseMsgBufLen);

				char* pTrueEncryptBuf;
				int nTrueEncryptBufLen;
				char* pFalseEncryptBuf;
				int nFalseEncryptBufLen;

				pTrueEncryptBuf = VBH::vbhEncrypt((unsigned char*)pProposeSession->m_envelopeKey.data(), nTrueEncryptBufLen, pTrueMsgBuf, nTrueMsgBufLen);
				pFalseEncryptBuf = VBH::vbhEncrypt((unsigned char*)pProposeSession->m_envelopeKey.data(), nFalseEncryptBufLen, pFalseMsgBuf, nFalseMsgBufLen);

				DSC_THREAD_SIZE_FREE(pTrueMsgBuf, nTrueMsgBufLen);
				DSC_THREAD_SIZE_FREE(pFalseMsgBuf, nFalseMsgBufLen);

				if (DSC_LIKELY(pTrueEncryptBuf && pFalseEncryptBuf))
				{
					req.m_nChannelID = rProposeReq.m_nChannelID;
					req.m_nCltSessionID = m_nSessionID;

					CMcpHandler* pMcpHandler;
					ACE_UINT32 nHandleID;
					CHandleSession* pHandleSession;
					const ACE_INT32 nRand = abs(ACE_OS::rand_r(&m_seed)) % m_nEndorsePeerCount; //TODO:将来背书策略有调整

					for (ACE_INT32 i = 0; i < m_nEndorsePeerCount; ++i)
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

						if (nRand == i)
						{
							pProposeSession->m_pSubmitEsMcpHandleSession = pHandleSession;
							req.m_userData.Set(pTrueEncryptBuf, nTrueEncryptBufLen);
						}
						else
						{
							req.m_userData.Set(pFalseEncryptBuf, nFalseEncryptBufLen);
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
						VBH::Assign(pProposeSession->m_userKey, rProposeReq.m_userKey);
						pProposeSession->m_nCltSessionID = m_nSessionID;

						this->SetDscTimer(pProposeSession, CVbhCryptClientService::EN_SESSION_TIMEOUT_VALUE);
						m_mapProposeSession.Insert(m_nSessionID, pProposeSession);
						++m_nSessionID;
					}
					else
					{
						//网络出错不处理了
					}
				}
				else
				{
					DSC_THREAD_TYPE_DEALLOCATE(pProposeSession);
					DSC_RUN_LOG_ERROR("encrypt error.");
					nReturnCode = VBH::EN_ENCRYPT_ERROR_TYPE;
				}

				if (pTrueEncryptBuf)
				{
					DSC_THREAD_FREE(pTrueEncryptBuf);
				}
				if (pFalseEncryptBuf)
				{
					DSC_THREAD_FREE(pFalseEncryptBuf);
				}
			}
		}
	}

	return nReturnCode;
}

ACE_INT32 CVbhCryptClientService::GetUserInfo(VBH::CQueryUserInfoApiCltReq& rQueryUserReq)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;
	CQueryUserSession* pQueryUserSession = DSC_THREAD_TYPE_NEW(CQueryUserSession) CQueryUserSession(*this);
	CDscString svrPubKey;
	CDscString cltPriKey; //临时存储解码出的秘钥，因为session中暂时不要这两个秘钥

	//从查询消息中取出服务端公钥和客户端私钥
	if (VBH::DecodeClientCryptKey(rQueryUserReq.m_cryptUserKey, svrPubKey, cltPriKey, pQueryUserSession->m_envelopeKey))
	{
		DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
		DSC_RUN_LOG_ERROR("decode client crypt key error.");
		nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
	}
	else
	{
		VBH::CCryptQueryUserInfoCltEsReq req;

		VBH::Assign(pQueryUserSession->m_userKey, rQueryUserReq.m_userKey);

		if (DSC::Decode(req.m_genUserKey, rQueryUserReq.m_userKey.GetBuffer(), rQueryUserReq.m_userKey.GetSize()))
		{
			DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
			DSC_RUN_LOG_ERROR("decode user key failed.");
			nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
		}
		else
		{
			char* pEncryptBuf;
			int nEncryptBufLen;

			pQueryUserSession->m_strNonce = ACE_OS::rand_r(&m_seed);
			pEncryptBuf = VBH::vbhEncrypt((unsigned char*)pQueryUserSession->m_envelopeKey.data(), nEncryptBufLen,
				pQueryUserSession->m_strNonce.data(), pQueryUserSession->m_strNonce.size());

			if (DSC_LIKELY(pEncryptBuf))
			{
				CMcpHandler* pMcpHandler = this->GetMcpHandler();

				req.m_nCltSessionID = m_nSessionID;
				req.m_nChannelID = rQueryUserReq.m_nChannelID;
				req.m_userData.Set(pEncryptBuf, nEncryptBufLen);

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

					pQueryUserSession->m_nCltSessionID = m_nSessionID;
					pQueryUserSession->m_nChannelID = rQueryUserReq.m_nChannelID;

					this->SetDscTimer(pQueryUserSession, CVbhCryptClientService::EN_SESSION_TIMEOUT_VALUE);
					m_mapQueryUserSession.Insert(m_nSessionID, pQueryUserSession);
					++m_nSessionID;
				}

				DSC_THREAD_FREE(pEncryptBuf);
			}
			else
			{
				DSC_THREAD_TYPE_DEALLOCATE(pQueryUserSession);
				DSC_RUN_LOG_ERROR("encrypt error.");
				nReturnCode = VBH::EN_ENCRYPT_ERROR_TYPE;
			}
		}
	}


	return nReturnCode;
}

ACE_INT32 CVbhCryptClientService::GetTransInfo(VBH::CQueryTransInfoApiCltReq& rQueryTransReq)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;

	CQueryTransactionSession* pQueryTransSession = DSC_THREAD_TYPE_NEW(CQueryTransactionSession) CQueryTransactionSession(*this);
	CDscString svrPubKey;
	CDscString cltPriKey; //临时存储解码出的秘钥，因为session中暂时不要这两个秘钥

	if (VBH::DecodeClientCryptKey(rQueryTransReq.m_cryptUserKey, svrPubKey, cltPriKey, pQueryTransSession->m_envelopeKey))
	{
		DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
		DSC_RUN_LOG_ERROR("decode client crypt key failed.");
		nReturnCode = VBH::EN_DECRYPT_ERROR_TYPE;
	}
	else
	{
		VBH::CCryptQueryTransInfoCltEsReq req;

		VBH::Assign(pQueryTransSession->m_userKey, rQueryTransReq.m_userKey);
		VBH::Assign(pQueryTransSession->m_transKey, rQueryTransReq.m_transKey);

		if (DSC::Decode(req.m_genUserKey, rQueryTransReq.m_userKey.GetBuffer(), rQueryTransReq.m_userKey.GetSize()))
		{
			DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
			DSC_RUN_LOG_ERROR("decode user key error.");
			nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
		}
		else
		{
			VBH::COrderGenerateTransactionKey transKey;

			if (DSC::Decode(transKey, rQueryTransReq.m_transKey.GetBuffer(), rQueryTransReq.m_transKey.GetSize()))
			{
				DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
				DSC_RUN_LOG_ERROR("decode transaction key error.");
				nReturnCode = VBH::EN_DECODE_ERROR_TYPE;
			}
			else
			{
				pQueryTransSession->m_strNonce = ACE_OS::rand_r(&m_seed);

				DSC::CDscBlob nonce(pQueryTransSession->m_strNonce.data(), pQueryTransSession->m_strNonce.size());
				VBH::CCryptQueryTransactionReqDataWrapper wrapper(nonce, transKey);
				char* pMsgBuf;
				size_t nMsgBufLen;

				DSC::Encode(wrapper, pMsgBuf, nMsgBufLen);

				if (pMsgBuf)
				{
					int nEncryptBufLen;
					char* pEncryptBuf = VBH::vbhEncrypt((unsigned char*)pQueryTransSession->m_envelopeKey.data(), nEncryptBufLen, pMsgBuf, nMsgBufLen);

					DSC_THREAD_SIZE_FREE(pMsgBuf, nMsgBufLen);

					if (pEncryptBuf)
					{
						CMcpHandler* pMcpHandler = this->GetMcpHandler();

						req.m_nCltSessionID = m_nSessionID;
						req.m_nChannelID = rQueryTransReq.m_nChannelID;
						req.m_userData.Set(pEncryptBuf, nEncryptBufLen);

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

							pQueryTransSession->m_nCltSessionID = m_nSessionID;

							this->SetDscTimer(pQueryTransSession, EN_SESSION_TIMEOUT_VALUE);
							m_mapQueryTransSession.Insert(m_nSessionID, pQueryTransSession);
							++m_nSessionID;
						}
						DSC_THREAD_FREE(pEncryptBuf);
					}
					else
					{
						DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
						nReturnCode = VBH::EN_ENCRYPT_ERROR_TYPE;
					}
				}
				else
				{
					DSC_THREAD_TYPE_DEALLOCATE(pQueryTransSession);
					DSC_RUN_LOG_ERROR("encode error.");
					nReturnCode = VBH::EN_SYSTEM_ERROR_TYPE;
				}

			}
		}
	}

	return nReturnCode;
}

ACE_INT32 CVbhCryptClientService::GetPeerID(const ACE_UINT32 nHandleID)
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

CMcpHandler* CVbhCryptClientService::GetMcpHandler(void)
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

ACE_INT32 CVbhCryptClientService::LoadPeerInfo(void)
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

void CVbhCryptClientService::OnReady(void)
{
	m_bReady = true;
	if (m_pCallback)
	{
		m_pCallback->OnReady();
	}
}

void CVbhCryptClientService::OnAbnormal(void)
{
	m_bReady = false;
	if (m_pCallback)
	{
		m_pCallback->OnAbnormal();
	}
}

void CVbhCryptClientService::OnRegistUserRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& cryptKey, DSC::CDscBlob& genUserKey)
{
	VBH_SDK::CRegistUserRsp rsp;

	rsp.m_nReturnCode = nReturnCode;
	rsp.m_nChannelID = nChannelID;

	rsp.m_pUserKey = userKey.GetBuffer();
	rsp.m_nUserKeyLen = userKey.GetSize();

	rsp.m_pCryptUserKey = cryptKey.GetBuffer();
	rsp.m_nCryptUserKeyLen = cryptKey.GetSize();

	rsp.m_pGenUserKey = genUserKey.GetBuffer();
	rsp.m_nGenUserKeyLen = genUserKey.GetSize();

	m_pCallback->OnRegistUserRsp(rsp);
}

void CVbhCryptClientService::OnUnRegistUserRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey)
{
	VBH_SDK::CUnRegistUserRsp rsp;

	rsp.m_nReturnCode = nReturnCode;
	rsp.m_nChannelID = nChannelID;

	rsp.m_pUserKey = userKey.GetBuffer();
	rsp.m_nUserKeyLen = userKey.GetSize();

	m_pCallback->OnUnRegistUserRsp(rsp);
}

void CVbhCryptClientService::OnProposeRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& transKey)
{
	VBH_SDK::CProposeRsp rsp;

	rsp.m_nReturnCode = nReturnCode;
	rsp.m_nChannelID = nChannelID;

	rsp.m_pUserKey = userKey.GetBuffer();
	rsp.m_nUserKeyLen = userKey.GetSize();

	rsp.m_pTransKey = transKey.GetBuffer();
	rsp.m_nTransKeyLen = transKey.GetSize();

	m_pCallback->OnLaunchTransRsp(rsp);
}

void CVbhCryptClientService::OnGetUserInfoRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& userInfo)
{
	VBH_SDK::CGetUserInfoRsp rsp;

	rsp.m_nReturnCode = nReturnCode;
	rsp.m_nChannelID = nChannelID;

	rsp.m_pUserKey = userKey.GetBuffer();
	rsp.m_nUserKeyLen = userKey.GetSize();

	rsp.m_pUserUserInfo = userInfo.GetBuffer();
	rsp.m_nUserUserInfoLen = userInfo.GetSize();

	m_pCallback->OnGetUserInfoRsp(rsp);
}

void CVbhCryptClientService::OnGetTransInfoRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& transKey, DSC::CDscBlob& transInfo)
{
	VBH_SDK::CGetTransactionInfoRsp rsp;

	rsp.m_nReturnCode = nReturnCode;
	rsp.m_nChannelID = nChannelID;

	rsp.m_pUserKey = userKey.GetBuffer();
	rsp.m_nUserKeyLen = userKey.GetSize();

	rsp.m_pTransKey = transKey.GetBuffer();
	rsp.m_nTransKeyLen = transKey.GetSize();

	rsp.m_pTransInfo = transInfo.GetBuffer();
	rsp.m_nTransInfoLen = transInfo.GetSize();

	m_pCallback->OnGetTransInfoRsp(rsp);
}
