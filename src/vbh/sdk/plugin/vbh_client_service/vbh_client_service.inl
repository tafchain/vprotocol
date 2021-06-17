inline void CVbhClientService::CRegistUserSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhClientService::CRegistUserSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}

inline void CVbhClientService::CRegistUserWaitNotitySession::OnTimer(void)
{

}

inline void CVbhClientService::CRegistUserWaitNotitySession::OnNetError(void)
{

}

inline void CVbhClientService::CQueryUserSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhClientService::CQueryUserSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}

inline void CVbhClientService::CProposeSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhClientService::CProposeSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}

inline void CVbhClientService::CProposeWaitNotifySession::OnTimer(void)
{

}

inline void CVbhClientService::CProposeWaitNotifySession::OnNetError(void)
{

}

inline void CVbhClientService::CQueryTransactionSession::OnTimer(void)
{
	m_rClientBaseService.OnTimeOut(this);
}

inline void CVbhClientService::CQueryTransactionSession::OnNetError(void)
{
	m_rClientBaseService.OnNetError(this);
}


template <typename SESSION_TYPE>
inline void CVbhClientService::OnReleaseMultiHandleSession(SESSION_TYPE* pSession)
{
	//从所有的背书 handle-session中删除本 -session
	CHandleSession* pHandleSession;
	for (ACE_UINT32 idx = 0; idx < pSession->m_arrHandleSession.Size(); ++idx)
	{
		pHandleSession = pSession->m_arrHandleSession[idx];
		pHandleSession->m_arrSession.Erase(pSession);
	}
	DSC_THREAD_TYPE_DELETE(pSession);
}

template <typename SESSION_TYPE>
void CVbhClientService::OnReleaseSingleHanleSession(SESSION_TYPE* pSession)
{
	pSession->m_pHandleSession->m_arrSession.Erase(pSession);
	DSC_THREAD_TYPE_DEALLOCATE(pSession);
}

