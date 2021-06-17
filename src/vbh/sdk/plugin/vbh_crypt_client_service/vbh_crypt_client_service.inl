inline void CVbhCryptClientService::CRegistUserSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhCryptClientService::CRegistUserSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}

inline void CVbhCryptClientService::CQueryUserSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhCryptClientService::CQueryUserSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}

inline void CVbhCryptClientService::CProposeSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhCryptClientService::CProposeSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}

inline void CVbhCryptClientService::CQueryTransactionSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhCryptClientService::CQueryTransactionSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}


