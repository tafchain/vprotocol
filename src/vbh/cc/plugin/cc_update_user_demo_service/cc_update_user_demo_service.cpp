#include "cc_update_user_demo_service/cc_update_user_demo_service.h"
#include "common/vbh_comm_error_code.h"

CCcUpdateUserDemoService::CCcUpdateUserDemoService()
{
}

CCcUpdateUserDemoService::CSession::CSession(CCcUpdateUserDemoService& rCcDemoService)
: m_rCcService(rCcDemoService)
{

}

ACE_INT32 CCcUpdateUserDemoService::OnInit(void)
{
	if(CCcObjService::OnInit())
	{
		DSC_RUN_LOG_ERROR("cc demo service init failed!");

		return -1;
	}

	DSC_RUN_LOG_INFO("cc demo service init succeed!");

	return 0;
}

ACE_INT32 CCcUpdateUserDemoService::OnExit(void)
{
	return CCcObjService::OnExit();
}

ACE_INT32 CCcUpdateUserDemoService::OnPorposal(const ACE_UINT32 nSessionID, CTestUpdateUser& rTestUpdateUser)
{	
	CSession* pSession = DSC_THREAD_TYPE_NEW(CSession) CSession(*this);

	pSession->m_updateUser = rTestUpdateUser;
	pSession->m_user.userKey = rTestUpdateUser.user.userKey;
	pSession->m_user.userInfo = rTestUpdateUser.user.userInfo;

	if (m_mapSession.Insert(nSessionID, pSession))
	{
		DSC_RUN_LOG_INFO("insert session to map failed, session:%d", nSessionID);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);

		return - 1;
	}
	else
	{
		this->SetDscTimer(pSession, EN_SESSION_TIMEOUT_VALUE);
		DSC_FORWARD_CALL(this->GetVbhUser(nSessionID, EN_TEST_CHANNEL_ID, rTestUpdateUser.user.userKey));
	}

	return 0;
}

void CCcUpdateUserDemoService::OnGetVbhUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT32 nSessionID, CSimpleUser& rUser)
{
	CSession* pSession = m_mapSession.Find(nSessionID);

	if (pSession)
	{
		if(nReturnCode == VBH::EN_OK_TYPE)
		{
			CTestUser user;

			if (this->DecodeUser(user.userKey, user.userInfo, rUser))
			{
				this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed, decode erorr"));

				this->CancelDscTimer(pSession);
				m_mapSession.Erase(nSessionID);
				DSC_THREAD_TYPE_DEALLOCATE(pSession);
			}
			else
			{
				if (pSession->m_user.userKey == user.userKey)
				{
					this->SetVbhUser(nSessionID, EN_TEST_CHANNEL_ID, pSession->m_user.userKey, pSession->m_user.userInfo);

					this->PorposalRsp(VBH::EN_OK_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade succeed"));
					this->CancelDscTimer(pSession);
					m_mapSession.Erase(nSessionID);
					DSC_THREAD_TYPE_DEALLOCATE(pSession);
				}
				else
				{
					DSC_RUN_LOG_INFO("user key error");
					this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed, decode erorr"));

					this->CancelDscTimer(pSession);
					m_mapSession.Erase(nSessionID);
					DSC_THREAD_TYPE_DEALLOCATE(pSession);

					return -1;
				}


			}
		}
		else
		{
			this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed"));

			this->CancelDscTimer(pSession);
			m_mapSession.Erase(nSessionID);
			DSC_THREAD_TYPE_DEALLOCATE(pSession);
		}
	}
	else
	{
		DSC_RUN_LOG_INFO("cann't find session:%d", nSessionID);
	}

	return 0;
}


void CCcUpdateUserDemoService::OnTimeOut(CSession* pUserSession)
{
	DSC_RUN_LOG_INFO("cc timeout, session id:%d", pUserSession->m_nKey);
	m_mapSession.Erase(pUserSession);
	DSC_THREAD_TYPE_DEALLOCATE(pUserSession);
}
