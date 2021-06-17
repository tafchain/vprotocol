inline void CCcDemoService::CSession::OnTimer(void)
{
	m_rCcService.OnTimeOut(this);
}

