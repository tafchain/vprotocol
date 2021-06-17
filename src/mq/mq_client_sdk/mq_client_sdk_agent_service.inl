#include <typeinfo>  

inline ACE_UINT32 CMqClientSdkAgentService::CForwarderAddrSession::GetIndex(void) const
{
	return m_nIndex;
}

inline void CMqClientSdkAgentService::CForwarderAddrSession::SetIndex(const ACE_UINT32 nIndex)
{
	m_nIndex = nIndex;
}

template<typename TYPE>
void CMqClientSdkAgentService::SendDscMsg(TYPE& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	DSC_INTF_LOG_INFO("forwarde %s msg, topic:%d", typeid(TYPE).name(), rMsg.m_nTopicID);

	CForwarderAddrSession* pSession = m_handlerSessionArray.Get();

	if (pSession)
	{
		if (this->SendHtsMsg(rMsg, pSession->m_pMcpHandler))
		{
			DSC_RUN_LOG_INFO("forwarde %s msg failed, topic:%d", typeid(TYPE).name(), rMsg.m_nTopicID);
		}
	}
}

template<typename TYPE>
void CMqClientSdkAgentService::OnMqMsg(TYPE& rMqMsg)
{
	MQ::CAddrMqMsg mqMsg;

	mqMsg.m_nClientID = rMqMsg.m_nClientID;
	mqMsg.m_nTopicID = rMqMsg.m_nTopicID;
	mqMsg.m_nMsgSize = rMqMsg.m_msgBlob.GetSize();
	mqMsg.m_pMsg = rMqMsg.m_msgBlob.GetBuffer();

	m_mqMsgCallback->OnMqMsg(mqMsg);
}
