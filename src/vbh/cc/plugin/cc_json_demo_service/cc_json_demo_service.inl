inline void CCcJsonDemoService::CSession::OnTimer(void)
{
	m_rCcService.OnTimeOut(this);
}

