inline ACE_UINT32 CMqSchedulerService::AllocClientID(void)
{
	return ++m_nClientID == 0 ? ++m_nClientID : m_nClientID;
}
