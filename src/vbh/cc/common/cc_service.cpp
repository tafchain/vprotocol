#include "dsc/db/per/persistence.h"

#include "common/vbh_comm_error_code.h"
#include "common/vbh_comm_wrap_msg_def.h"
#include "common/crypt_key_codec.h"
#include "common/vbh_comm_func.h"

#include "common/cc_service.h"


CCcService::CSession::CSession(CCcService& rCcService)
	:m_rCcService(rCcService)
{
}

CCcService::CRegistUserSession::CRegistUserSession(CCcService& rCcService)
	: CSession(rCcService)
{
}

CCcService::CProcessProposalSession::CProcessProposalSession(CCcService& rCcService)
	:CSession(rCcService)
{
}

CCcService::CCcService()
{
}

ACE_INT32 CCcService::OnInit(void)
{
	if (CDscService::OnInit())
	{
		DSC_RUN_LOG_ERROR("cc demo service init failed!");

		return -1;
	}

	if (m_xcsRouter.Init())
	{
		DSC_RUN_LOG_ERROR("x committer router init failed.");

		return -1;
	}

	DSC_RUN_LOG_FINE("cc demo service init succeed!");

	return 0;
}

ACE_INT32 CCcService::OnExit(void)
{
	CRegistUserSession* pRegistUserSession;
	for (auto it = m_mapRegistUserSession.begin(); it != m_mapRegistUserSession.end();)
	{
		pRegistUserSession = it.second;
		this->CancelDscTimer(pRegistUserSession);
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pRegistUserSession);
	}

	CProcessProposalSession* pProposalSession;
	for (auto it = m_mapProcessProposalSession.begin(); it != m_mapProcessProposalSession.end();)
	{
		pProposalSession = it.second;
		this->CancelDscTimer(pProposalSession);
		++it;
		DSC_THREAD_TYPE_DEALLOCATE(pProposalSession);
	}

	return CDscService::OnExit();
}

void CCcService::OnDscMsg(VBH::CRegistUserEsCcReq& rRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_INT32 nReturnCode = VBH::EN_OK_TYPE;
	CRegistUserSession* pSession = DSC_THREAD_TYPE_NEW(CRegistUserSession) CRegistUserSession(*this);

	pSession->m_nCcSessionID = m_nSessionID;
	pSession->m_nEsSessionID = rRegistUserReq.m_nEsSessionID;
	pSession->m_nChannelID = rRegistUserReq.m_nChannelID;
	pSession->m_esAddr = rSrcMsgAddr;
	VBH::Assign(pSession->m_userKey, rRegistUserReq.m_userKey);
	VBH::Assign(pSession->m_userRegistInfo, rRegistUserReq.m_userInfo);

	this->SetDscTimer(pSession, EN_SESSION_TIMEOUT_VALUE);
	m_mapRegistUserSession.DirectInsert(m_nSessionID, pSession);
	++m_nSessionID;

	const ACE_UINT64 nCcSessionID = CombineUserCCSessionID(pSession->m_nCcSessionID, EN_CC_REGIST_USER_SESSION_TYPE);
	//TODO: ȥ��ע���û���action-id
	if (RegistUserProc(nCcSessionID, pSession->m_nChannelID, pSession->m_userKey, pSession->m_userRegistInfo))
	{
		VBH::CRegistUserCcEsRsp rsp;

		m_mapProcessProposalSession.Erase(pSession->m_nCcSessionID);
		this->CancelDscTimer(pSession);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);

		rsp.m_nReturnCode = VBH::EN_CC_REPORT_ERROR_TYPE;
		rsp.m_nEsSessionID = rRegistUserReq.m_nEsSessionID;
		this->SendDscMessage(rsp, rSrcMsgAddr);

		DSC_RUN_LOG_INFO("cc report error");
	}
}

void CCcService::OnDscMsg(VBH::CSubmitProposalEsCcReq& rSubmitProposeReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	CProcessProposalSession* pSession = DSC_THREAD_TYPE_NEW(CProcessProposalSession) CProcessProposalSession(*this);

	pSession->m_nCcSessionID = m_nSessionID;
	pSession->m_nEsSessionID = rSubmitProposeReq.m_nEsSessionID;
	pSession->m_nChannelID = rSubmitProposeReq.m_nChannelID;
	pSession->m_proposal.assign(rSubmitProposeReq.m_proposal.GetBuffer(), rSubmitProposeReq.m_proposal.GetSize());
	pSession->m_esAddr = rSrcMsgAddr;

	this->SetDscTimer(pSession, EN_SESSION_TIMEOUT_VALUE);
	m_mapProcessProposalSession.DirectInsert(m_nSessionID, pSession);
	++m_nSessionID;

	if (this->ProposalProc(pSession->m_nCcSessionID, rSubmitProposeReq.m_nChannelID, rSubmitProposeReq.m_nAction, pSession->m_proposal))
	{
		VBH::CSubmitProposalCcEsRsp rsp;

		m_mapProcessProposalSession.Erase(pSession->m_nCcSessionID);
		this->CancelDscTimer(pSession);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);
		rsp.m_nReturnCode = VBH::EN_CC_REPORT_ERROR_TYPE;
		rsp.m_nEsSessionID = rSubmitProposeReq.m_nEsSessionID;

		this->SendDscMessage(rsp, rSrcMsgAddr);
		DSC_RUN_LOG_INFO("cc report error");
	}
}

void CCcService::OnDscMsg(VBH::CQueryUserInfoXcsCcRsp rsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, rsp.m_nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			if (rsp.m_nReturnCode == VBH::EN_OK_TYPE)
			{
				CSimpleUser simpleUser;

				for (auto it = pSession->m_lstUser.begin(); it != pSession->m_lstUser.end(); ++it)
				{
					if (it->m_userKey.m_nSystemID == rsp.m_nUserInfoVersion)
					{
						it->m_nVersion = rsp.m_nUserInfoVersion;
						VBH::Assign(it->m_userInfo, rsp.m_userInfo);

						VBH::Assign(simpleUser.m_userKey, it->m_strUserKey);
						VBH::Assign(simpleUser.m_userInfo, it->m_userInfo);

						this->OnGetVbhUserRsp(VBH::EN_OK_TYPE, rsp.m_nCcSessionID, simpleUser);
						break;
					}
				}
			}
			else
			{
				CSimpleUser simpleUser;

				this->OnGetVbhUserRsp(rsp.m_nReturnCode, rsp.m_nCcSessionID, simpleUser);
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("cann't find process prosal session, session id:%d", rsp.m_nCcSessionID);
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("error session-type, when receive CQueryUserInfoUcsCcRsp.");
	}
}

void CCcService::OnDscMsg(VBH::CQueryInformationXcsCcRsp rsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, rsp.m_nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			if (rsp.m_nReturnCode == VBH::EN_OK_TYPE)
			{
				CSimpleInformation simpleInfo;

				for (auto it = pSession->m_lstInfo.begin(); it != pSession->m_lstInfo.end(); ++it)
				{
					if (it->m_key.m_nSystemID == rsp.m_nInfoSysIndex)
					{
						it->m_nVersion = rsp.m_nVersion;
						VBH::Assign(it->m_value, rsp.m_value);

						VBH::Assign(simpleInfo.m_key, it->m_strKey);
						VBH::Assign(simpleInfo.m_value, it->m_value);

						this->OnGetVbhInfoRsp(VBH::EN_OK_TYPE, rsp.m_nCcSessionID, simpleInfo);
						break;
					}
				}
			}
			else
			{
				CSimpleInformation simpleInfo;

				this->OnGetVbhInfoRsp(rsp.m_nReturnCode, rsp.m_nCcSessionID, simpleInfo);
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("cann't find process prosal session, session id:%d", rsp.m_nCcSessionID);
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("error session-type, when receive CQueryUserInfoUcsCcRsp.");
	}
}

void CCcService::OnTimeOut(CRegistUserSession* pRegistUserSession)
{
	VBH::CRegistUserCcEsRsp rsp;

	rsp.m_nReturnCode = VBH::EN_TIMEOUT_ERROR_TYPE;
	rsp.m_nEsSessionID = pRegistUserSession->m_nEsSessionID;

	this->SendDscMessage(rsp, pRegistUserSession->m_esAddr);
	m_mapRegistUserSession.Erase(pRegistUserSession);
	DSC_THREAD_TYPE_DELETE(pRegistUserSession);

	DSC_RUN_LOG_INFO("cc regist user session timeout.");
}

void CCcService::OnTimeOut(CProcessProposalSession* pProcessProposalSession)
{
	VBH::CSubmitProposalCcEsRsp rsp;

	rsp.m_nReturnCode = VBH::EN_TIMEOUT_ERROR_TYPE;
	rsp.m_nEsSessionID = pProcessProposalSession->m_nEsSessionID;

	this->SendDscMessage(rsp, pProcessProposalSession->m_esAddr);
	m_mapProcessProposalSession.Erase(pProcessProposalSession);
	DSC_THREAD_TYPE_DELETE(pProcessProposalSession);

	DSC_RUN_LOG_INFO("cc proposal session timeout");
}

ACE_INT32 CCcService::GetVbhUser(const ACE_UINT64 nCcSessionID, const char* pUserKey, const size_t nUserKey)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			//������Դ�session���ҵ�����ʹ��session�л�������
			auto it = pSession->m_lstUser.begin();

			for (; it != pSession->m_lstUser.end(); ++it)
			{
				if (it->m_strUserKey.IsEqual(pUserKey, nUserKey))
				{
					break;
				}
			}
			
			if (it == pSession->m_lstUser.end()) //û���ҵ��� ���Ͳ�ѯ����ucs
			{
				VBH::CQueryUserInfoCcXcsReq req;
				CDscMsg::CDscMsgAddr addr;
				CCcUser user;
				ACE_UINT32 nChannelID;
				DSC::CDscBlob encodeGenUserKey((char*)pUserKey, nUserKey);

				if (m_xcsRouter.GetXcsAddr(addr, pSession->m_nChannelID))
				{
					DSC_RUN_LOG_INFO("can not find channel's xcs addr, channel-id:%d", pSession->m_nChannelID);

					return VBH::EN_CANNOT_FOUND_CHANNEL_ERROR_TYPE;
				}
				if (VBH::DecodeGenerateUserKey(encodeGenUserKey, nChannelID, user.m_userKey))
				{
					DSC_RUN_LOG_ERROR("decode user key error.");

					return VBH::EN_DECODE_ERROR_TYPE;
				}

				req.m_genUserKey = user.m_userKey;
				req.m_nCcSessionID = nCcSessionID;
				
				user.m_strUserKey.assign(pUserKey, nUserKey);
				pSession->m_lstUser.push_back(user);

				return this->SendDscMessage(req, addr);
			}
			else //�ҵ��ˣ� ֱ�ӷ��ظ��ϲ��߼�
			{
				CSimpleUser simpleUser;

				simpleUser.m_userKey.Set((char*)pUserKey, nUserKey);
				VBH::Assign(simpleUser.m_userInfo, it->m_userInfo);

				this->OnGetVbhUserRsp(VBH::EN_OK_TYPE, nCcSessionID, simpleUser);

				return 0;
			}
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find session, session id:%d", nSessionID);

			return VBH::EN_SYSTEM_ERROR_TYPE;
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("call get-vbh-user on error session-type.");

		return VBH::EN_SYSTEM_ERROR_TYPE;
	}
}

ACE_INT32 CCcService::SetVbhUser(const ACE_UINT64 nCcSessionID, const char* pUserKey, const size_t nUserKey, const char* pUserInfo, const size_t nUserInfo)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			auto it = pSession->m_lstUser.begin();

			for (; it != pSession->m_lstUser.end(); ++it)
			{
				if (it->m_strUserKey.IsEqual(pUserKey, nUserKey))
				{
					it->m_userInfo.assign(pUserInfo, nUserInfo);

					if (!it->m_bSetted) //û������ʱ���汾��+1���ٴ�����ʱ���汾�Ų���
					{
						it->m_bSetted = true;
						VBH::IncreaseVersion(it->m_nVersion);
					}

					break;
				}
			}

			return it == pSession->m_lstUser.end() ? -1 : 0;
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find session, session id:%d", nSessionID);

			return VBH::EN_SYSTEM_ERROR_TYPE;
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("call set-vbh-user on error session-type.");

		return VBH::EN_SYSTEM_ERROR_TYPE;
	}
}

ACE_INT32 CCcService::GetVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			//������Դ�session���ҵ�����ʹ��session�л�������
			auto it = pSession->m_lstInfo.begin();

			for (; it != pSession->m_lstInfo.end(); ++it)
			{
				if (it->m_strKey.IsEqual(pKey, nKey))
				{
					break;
				}
			}

			if (it == pSession->m_lstInfo.end()) //û���ҵ��� ���Ͳ�ѯ����ics
			{
				VBH::CQueryInformationCcXcsReq req;
				CDscMsg::CDscMsgAddr addr;
				CCcInformation info;

				if (m_xcsRouter.GetXcsAddr(addr, pSession->m_nChannelID))
				{
					DSC_RUN_LOG_INFO("can not find channel information group.");

					return VBH::EN_CANNOT_FOUND_CHANNEL_ERROR_TYPE;
				}
				if (DSC::Decode(info.m_key, pKey, nKey))
				{
					DSC_RUN_LOG_ERROR("decode information-key error.");

					return VBH::EN_DECODE_ERROR_TYPE;
				}
				
				req.m_nCcSessionID = nCcSessionID;
				req.m_genInfoKey = info.m_key;

				
				info.m_strKey.assign(pKey, nKey);
				pSession->m_lstInfo.push_back(info);

				return this->SendDscMessage(req, addr);
			}
			else //�ҵ��ˣ� ֱ�ӷ��ظ��ϲ��߼�
			{
				CSimpleInformation simpleInfo;

				simpleInfo.m_key.Set((char*)pKey, nKey);
				VBH::Assign(simpleInfo.m_value, it->m_value);

				this->OnGetVbhInfoRsp(VBH::EN_OK_TYPE, nCcSessionID, simpleInfo);

				return 0;
			}
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find session, session id:%d", nSessionID);

			return -1;
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("call get-vbh-user on error session-type.");

		return -1;
	}
}

ACE_INT32 CCcService::SetVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey, const char* pValue, const size_t nValue)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			auto it = pSession->m_lstInfo.begin();
			for (; it != pSession->m_lstInfo.end(); ++it)
			{
				if (it->m_strKey.IsEqual(pKey, nKey))
				{
					it->m_value.assign(pValue, nValue);

					if (!it->m_bSetted) //û������ʱ���汾��+1�� �ٴ�����ʱ���汾�Ų���
					{
						it->m_bSetted = true;
						VBH::IncreaseVersion(it->m_nVersion);
					}

					break;
				}
			}

			//���û�в鵽�����ش��� ���ܴ�������ר�õĴ���api
			if (it == pSession->m_lstInfo.end())
			{
				DSC_RUN_LOG_INFO("can not find information when set it.");

				return -1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find process proposal session, session id:%d", nSessionID);

			return -1;
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("call set-vbh-user on error session-type.");

		return -1;
	}
}

ACE_INT32 CCcService::CreateVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey, const char* pValue, const size_t nValue)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Find(nSessionID);

		if (pSession)
		{
			auto it = pSession->m_lstCreatedInfo.begin();

			for (; it != pSession->m_lstCreatedInfo.end(); ++it)
			{
				if (it->m_key.IsEqual(pKey, nKey))
				{
					break;
				}
			}

			if (it == pSession->m_lstCreatedInfo.end()) //û���ҵ��ظ���
			{
				CCcCreatedInformation info;

				info.m_key.assign(pKey, nKey);
				info.m_value.assign(pValue, nValue);

				pSession->m_lstCreatedInfo.push_back(info);

				return 0;
			}
			else //�ҵ����ظ���
			{
				DSC_RUN_LOG_INFO("repeate create information");

				return -1;
			}
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find process proposal session, session id:%d", nSessionID);

			return -1;
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("call create-vbh-user on error session-type.");

		return -1;
	}
}

ACE_INT32 CCcService::RegistUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, const bool bAllowRegist, char* pUserInitInfo, const size_t nUserInitInfo)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_REGIST_USER_SESSION_TYPE == nSessionType) //ע���û�
	{
		CRegistUserSession* pSession = m_mapRegistUserSession.Erase(nSessionID); //���һ��ʹ�ã�ֱ��ɾ��

		if (pSession)
		{
			VBH::CRegistUserCcEsRsp rsp;

			rsp.m_nReturnCode = nReturnCode;
			rsp.m_nEsSessionID = pSession->m_nEsSessionID;

			if (rsp.m_nReturnCode == VBH::EN_OK_TYPE)
			{
				rsp.m_bAllowRegist = bAllowRegist;

				if (bAllowRegist) //ͬ��ע��
				{
					rsp.m_userInitInfo.Set(pUserInitInfo, nUserInitInfo);
				}
			}

			this->SendDscMessage(rsp, pSession->m_esAddr);

			//����session
			this->CancelDscTimer(pSession);
			DSC_THREAD_TYPE_DELETE(pSession);
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find session, session-id:%d", nSessionID);

			return -1;
		}

		return 0;
	}
	else
	{
		DSC_RUN_LOG_ERROR("call regist-user-response on error session-type.");

		return -1;
	}
}

ACE_INT32 CCcService::PorposalRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, char* pTransProcessResult, const size_t nTransProcessResult)
{
	ACE_UINT32 nSessionID;
	ACE_UINT32 nSessionType;

	SplitUserCCSessionID(nSessionID, nSessionType, nCcSessionID);

	if (EN_CC_PROCESS_PROPOSAL_SESSION_TYPE == nSessionType) //�����᰸
	{
		CProcessProposalSession* pSession = m_mapProcessProposalSession.Erase(nSessionID);

		if (pSession)
		{
			VBH::CSubmitProposalCcEsRsp rsp;
			char* pTransContent = NULL;
			size_t nTransContentLen;

			rsp.m_nReturnCode = VBH::EN_OK_TYPE;
			rsp.m_nEsSessionID = pSession->m_nEsSessionID;

			if (nReturnCode == VBH::EN_OK_TYPE)
			{
				DSC::CDscBlob proposal(pSession->m_proposal.data(), pSession->m_proposal.size()); //�᰸����
				DSC::CDscBlob transProcessResult(pTransProcessResult, nTransProcessResult); //�᰸������ //remark
				DSC::CDscShortVector<VBH::CVbhUser> vecUpdateUser; //�漰������û�д�� version ��> 0
				DSC::CDscShortVector<VBH::CVbhOnCreateInformation> vecOnCreateInfo; //�´�����information�б�
				DSC::CDscShortVector<VBH::CVbhInformation> vecUpdateInfo; //�漰����� information д�� version ��> 0
				VBH::CRoughProposalTransactionWrapper wrapper(proposal, transProcessResult, vecUpdateUser, vecOnCreateInfo, vecUpdateInfo);

				VBH::CVbhUser vbhUser;
				//�������б�set�����û�
				for (auto it = pSession->m_lstUser.begin(); it != pSession->m_lstUser.end(); ++it)
				{
					if (it->m_bSetted)
					{
						vbhUser.m_nVersion = it->m_nVersion;
						vbhUser.m_genUserKey = it->m_userKey;
						vbhUser.m_userInfo.Set(it->m_userInfo.data(), it->m_userInfo.size());
						vecUpdateUser.push_back(vbhUser);
					}
				}

				VBH::CVbhOnCreateInformation onCreateInfo;
				//������������information //�½���information���ڶ��е�ǰ��
				for (auto it = pSession->m_lstCreatedInfo.begin(); it != pSession->m_lstCreatedInfo.end(); ++it)
				{
					VBH::Assign(onCreateInfo.m_key, it->m_key);
					VBH::Assign(onCreateInfo.m_value, it->m_value);
					vecOnCreateInfo.push_back(onCreateInfo);
				}

				VBH::CVbhInformation vbhInfo;
				//�������б�set����information
				for (auto it = pSession->m_lstInfo.begin(); it != pSession->m_lstInfo.end(); ++it)
				{
					if (it->m_bSetted)
					{
						vbhInfo.m_nVersion = it->m_nVersion;
						VBH::Assign(vbhInfo.m_key, it->m_strKey); //���������COrderGenerateKey
						VBH::Assign(vbhInfo.m_value, it->m_value);

						vecUpdateInfo.push_back(vbhInfo);
					}
				}

				DSC::Encode(wrapper, pTransContent, nTransContentLen);

				if (pTransContent)
				{
					rsp.m_transContent.Set(pTransContent, nTransContentLen);
				}
				else
				{
					rsp.m_nReturnCode = VBH::EN_SYSTEM_ERROR_TYPE;
					DSC_RUN_LOG_ERROR("encode error.");
				}
			}
			else
			{
				rsp.m_nReturnCode = VBH::EN_CC_REPORT_ERROR_TYPE;
			}

			this->SendDscMessage(rsp, pSession->m_esAddr);
			this->CancelDscTimer(pSession);
			DSC_THREAD_TYPE_DEALLOCATE(pSession);
			if (pTransContent)
			{
				DSC_THREAD_SIZE_FREE(pTransContent, nTransContentLen);
			}

			return 0;
		}
		else
		{
			DSC_RUN_LOG_ERROR("can not find session, session id:%d", nSessionID);

			return -1;
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("call proposal-response on error session-type.");

		return -1;
	}
}



