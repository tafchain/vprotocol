#include "dsc/configure/dsc_configure.h"

#include "mq_scheduler_service/mq_scheduler_service.h"

bool CMqSchedulerService::CForwarderAddrSession::operator< (const CForwarderAddrSession& rForwarderAddrSession) const
{
	if (m_nPort == rForwarderAddrSession.m_nPort)
	{
		return m_strAddr < rForwarderAddrSession.m_strAddr;
	}

	return m_nPort < rForwarderAddrSession.m_nPort;
}

CMqSchedulerService::CMqSchedulerService()
: m_nClientID(0)
{
}

ACE_INT32 CMqSchedulerService::OnInit(void)
{
	if(CDscHtsServerService::OnInit())
	{
		DSC_RUN_LOG_ERROR("mq scheduler service init failed!");

		return -1;
	}

	CDscString strMqchedulerService;
	ACE_INT32  nMqSchedulerServicePort = 0;

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileString("MQ_SCHEDULER_SERVICE_IP_ADDR", strMqchedulerService))
	{
		DSC_RUN_LOG_WARNING("cann't read 'MQ_SCHEDULER_SERVICE_IP_ADDR' configure item value");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("MQ_SCHEDULER_SERVICE_PORT", nMqSchedulerServicePort))
	{
		DSC_RUN_LOG_WARNING("cann't read 'MQ_SCHEDULER_SERVICE_PORT' configure item value");

		return -1;
	}

	m_pMqSchedulerServiceAcceptor = ::new(DSC_THREAD_SIZE_MALLOC(sizeof(CServiceAsynchAcceptor<CMqSchedulerService>))) CServiceAsynchAcceptor<CMqSchedulerService>(*this);
	if (m_pMqSchedulerServiceAcceptor->Open(nMqSchedulerServicePort, strMqchedulerService.c_str()))
	{
		DSC_RUN_LOG_WARNING("open failed, ip addr:%s, port:%d!", strMqchedulerService.c_str(), nMqSchedulerServicePort);

		return -1;
	}
	else
	{
		if (-1 == this->RegistHandler(m_pMqSchedulerServiceAcceptor, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL))
		{
			DSC_RUN_LOG_WARNING("regist handler failed, ip addr:%s, port:%d", strMqchedulerService.c_str(), nMqSchedulerServicePort);

			return -1;
		}
	}

	return 0;
}

ACE_INT32 CMqSchedulerService::OnExit(void)
{
	m_pMqSchedulerServiceAcceptor->Close();
	m_pMqSchedulerServiceAcceptor->ReleaseServiceHandler();

	return CDscHtsServerService::OnExit();
}

void CMqSchedulerService::OnDscMsg(MQ_MSG::CRegistMqForwarderServiceReq& rRegistMqForwarderServiceReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	CForwarderAddrSession forwarderAddr;

	forwarderAddr.m_nNodeType = rSrcMsgAddr.GetNodeType();
	forwarderAddr.m_nNodeID = rSrcMsgAddr.GetNodeID();
	forwarderAddr.m_nPort = rRegistMqForwarderServiceReq.m_nPort;
	forwarderAddr.m_strAddr = rRegistMqForwarderServiceReq.m_strAddr;

	dsc_set_type(CForwarderAddrSession)::iterator it = m_setForwarderAddr.find(forwarderAddr);
	
	if (it == m_setForwarderAddr.end())
	{
		MQ_MSG::CAddFowrarderAddrNotify notify;

		notify.m_mqForwarderAddr.m_nPort = forwarderAddr.m_nPort;
		notify.m_mqForwarderAddr.m_strIpAddr = forwarderAddr.m_strAddr;

		for (dsc_set_type(ACE_UINT32)::iterator it = m_setMqClientHandlerID.begin(); it != m_setMqClientHandlerID.end(); ++it)
		{
			this->SendHtsMsg(notify, *it);
		}

		m_setForwarderAddr.insert(forwarderAddr);
	}
}

ACE_INT32 CMqSchedulerService::OnHtsMsg(MQ_MSG::CRegMqClientAddrReq& rRegMqClientAddrReq, const ACE_UINT32 nHandleID)
{
	dsc_set_type(ACE_UINT32)::iterator it = m_setMqClientHandlerID.find(nHandleID);
	
	if (it == m_setMqClientHandlerID.end())
	{
		MQ_MSG::CMqForwarderAddr addr;
		MQ_MSG::CRegMqClientAddrRsp rsp;

		m_setMqClientHandlerID.insert(nHandleID);

		if (rRegMqClientAddrReq.m_nClientID)
		{
			rsp.m_nClientID = rRegMqClientAddrReq.m_nClientID;
		}
		else
		{
			rsp.m_nClientID = this->AllocClientID();
		}

		for (dsc_set_type(CForwarderAddrSession)::iterator fwa_it = m_setForwarderAddr.begin(); fwa_it != m_setForwarderAddr.end(); ++fwa_it)
		{
			addr.m_nPort = fwa_it->m_nPort;
			addr.m_strIpAddr = fwa_it->m_strAddr;

			rsp.m_lstMqForwarderAddr.push_back(addr);
		}

		this->SendHtsMsg(rsp, nHandleID);
	}

	return 0;
}


void CMqSchedulerService::OnNetworkError(CMcpHandler* pMcpHandler)
{
	m_setMqClientHandlerID.erase(pMcpHandler->GetHandleID());

	CDscHtsServerService::OnNetworkError(pMcpHandler);
}

void CMqSchedulerService::OnDisConnected(const CDscConnectInfo& rNetInfo)
{
	//去除通知有forarder发
	//MQ_MSG::CDelFowrarderAddrNotify notify;

	for (dsc_set_type(CForwarderAddrSession)::iterator it = m_setForwarderAddr.begin(); it != m_setForwarderAddr.end();)
	{
		if (it->m_nNodeType == rNetInfo.m_remoteConnectInfo.m_nType && it->m_nNodeID == rNetInfo.m_remoteConnectInfo.m_nID)
		{
			//notify.m_mqForwarderAddr.m_nPort = it->m_nPort;
			//notify.m_mqForwarderAddr.m_strIpAddr = it->m_strAddr;

			//for (dsc_set_type(ACE_UINT32)::iterator it = m_setMqClientHandlerID.begin(); it != m_setMqClientHandlerID.end(); ++it)
			//{
			//	this->SendHtsMsg(notify, *it);
			//}

			it = m_setForwarderAddr.erase(it);
		}
		else
		{
			++it;
		}
	}
}
