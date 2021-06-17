
inline void CCcService::CRegistUserSession::OnTimer(void)
{
	m_rCcService.OnTimeOut(this);
}

inline void CCcService::CProcessProposalSession::OnTimer(void)
{
	m_rCcService.OnTimeOut(this);
}

inline ACE_UINT64 CCcService::CombineUserCCSessionID(const ACE_UINT32 nSessionID, const ACE_UINT32 nSeesionType)
{
	ACE_UINT64 nCcSessinID = nSeesionType;

	nCcSessinID <<= 32;
	nCcSessinID += nSessionID;

	return nCcSessinID;
}

inline void CCcService::SplitUserCCSessionID(ACE_UINT32& nSessionID, ACE_UINT32& nSessionType, const ACE_UINT64 nCcSessinID)
{
	nSessionID = nCcSessinID & 0xFFFFFFFF;

	nSessionType = nCcSessinID >> 32;
}

