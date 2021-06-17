#include "cc_update_user_json_demo_service/cc_update_user_json_demo_service.h"
#include "common/vbh_comm_error_code.h"

CCcUpdateUserJsonDemoService::CSession::CSession(CCcUpdateUserJsonDemoService& rCcDemoService)
	: m_rCcService(rCcDemoService)
{

}

CCcUpdateUserJsonDemoService::CCcUpdateUserJsonDemoService()
{
}

ACE_INT32 CCcUpdateUserJsonDemoService::OnInit(void)
{
	if(CCcJsonService::OnInit())
	{
		DSC_RUN_LOG_ERROR("cc demo service init failed!");

		return -1;
	}

	DSC_RUN_LOG_INFO("cc demo service init succeed!");

	return 0;
}

ACE_INT32 CCcUpdateUserJsonDemoService::OnExit(void)
{
	return CCcJsonService::OnExit();
}

ACE_INT32 CCcUpdateUserJsonDemoService::OnPorposal(const ACE_UINT32 nSessionID, CTestUpdateUser& rTestUpdateUser)
{	
	CSession* pSession = DSC_THREAD_TYPE_NEW(CSession) CSession(*this);

	pSession->m_updateUser = rTestUpdateUser;
	pSession->m_user.userKey = rTestUpdateUser.user.userKey;
	pSession->m_user.userInfo = rTestUpdateUser.user.userInfo;

	if (m_mapSession.Insert(nSessionID, pSession))
	{
		DSC_RUN_LOG_INFO("insert session to map failed, session:%d", nSessionID);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);

		return -1;
	}
	else
	{
		this->SetDscTimer(pSession, EN_SESSION_TIMEOUT_VALUE);
		DSC_FORWARD_CALL(this->GetVbhUser(nSessionID, EN_TEST_CHANNEL_ID, rTestUpdateUser.user.userKey));
	}

	return 0;
}

void CCcUpdateUserJsonDemoService::OnGetVbhUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT32 nSessionID, CSimpleUser& rUser)
{
	CTestUser user;

	DSC_FORWARD_CALL(this->DecodeUser(user.userKey, user.userInfo, rUser));

	CSession* pSession = m_mapSession.Find(nSessionID);

	if (pSession)
	{
		if (pSession->m_user.userKey == user.userKey)
		{			
			this->SetVbhUser(nSessionID, EN_TEST_CHANNEL_ID, pSession->m_user.userKey, pSession->m_user.userInfo);

			this->CancelDscTimer(pSession);
			m_mapSession.Erase(pSession);
			DSC_THREAD_TYPE_DEALLOCATE(pSession);
			this->PorposalRsp(VBH::EN_OK_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("update succeed"));
		}		
	}

	return 0;
}

void CCcUpdateUserJsonDemoService::OnTimeOut(CSession* pUserSession)
{
	m_mapSession.Erase(pUserSession);
	DSC_THREAD_TYPE_DEALLOCATE(pUserSession);

	DSC_RUN_LOG_INFO("cc timeout");
}



