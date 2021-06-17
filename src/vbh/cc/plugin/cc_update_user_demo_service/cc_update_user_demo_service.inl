inline void CCcUpdateUserDemoService::CSession::OnTimer(void)
{
	m_rCcService.OnTimeOut(this);
}

