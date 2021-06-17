template<typename MQ_MSG_TYPE, typename MQ_MSG_PROC>
MQ::CMqMsgProc<MQ_MSG_TYPE, MQ_MSG_PROC>::CMqMsgProc(MQ_MSG_PROC& rMqMsgProc)
: m_rMqMsgProc(rMqMsgProc)
{
}

template<typename MQ_MSG_TYPE, typename MQ_MSG_PROC>
int MQ::CMqMsgProc<MQ_MSG_TYPE, MQ_MSG_PROC>::OnMqMsg(MQ::CAddrMqMsg& rMqMsg)
{
	MQ_MSG_TYPE msg;

	if (DSC::Decode(msg, rMqMsg.m_pMsg, rMqMsg.m_nMsgSize))
	{
		return -1;
	}
	else
	{
		m_rMqMsgProc.OnMqMsg(rMqMsg.m_nClientID, msg);

		return 0;
	}
}

template<typename MQ_MSG_TYPE, typename MQ_MSG_PROC>
MQ::IMqMsgProc* MQ::CreateMqMsgProc(MQ_MSG_TYPE& rMsg, MQ_MSG_PROC& m_rMqMsgProc)
{
	return new(std::nothrow) MQ::CMqMsgProc<MQ_MSG_TYPE, MQ_MSG_PROC>(m_rMqMsgProc);
}
