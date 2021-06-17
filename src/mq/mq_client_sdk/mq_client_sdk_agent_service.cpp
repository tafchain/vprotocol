#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/netinet/os_tcp.h"

#include "dsc/configure/dsc_configure.h"

#include "mq_client_sdk/mq_client_sdk_agent_service.h"

CMqClientSdkAgentService::CMqClientSdkAgentService(MQ::CInitMqClientSdkParam& rInitMqClientSdkParam)
: m_mqMsgCallback(rInitMqClientSdkParam.m_pMqMsgCallback)
{
	for (unsigned int i = 0; i < rInitMqClientSdkParam.m_nTopicIDNum; ++i)
	{
		m_regMqTopicReq.m_lstTopicID.push_back(rInitMqClientSdkParam.m_pTopicID[i]);
	}		
	m_regMqTopicReq.m_nClientID = rInitMqClientSdkParam.m_nClientID;
}

ACE_INT32 CMqClientSdkAgentService::OnInit(void)
{
	if (CDscHtsClientService::OnInit())
	{
		DSC_RUN_LOG_ERROR("mq client sdk agent service init failed!");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileString("MQ_SCHEDULER_SERVICE_IP_ADDR", m_strMqSchedulerServiceAddr))
	{
		DSC_RUN_LOG_WARNING("cann't read 'MQ_SCHEDULER_SERVICE_IP_ADDR' configure item value");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("MQ_SCHEDULER_SERVICE_PORT", m_nMqSchedulerServicePort))
	{
		DSC_RUN_LOG_WARNING("cann't read 'MQ_SCHEDULER_SERVICE_PORT' configure item value");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("TCP_NODELAY_FLAG", m_nTcpNodelayFlag))
	{
		DSC_RUN_LOG_WARNING("cann't read 'TCP_NODELAY_FLAG' configure item value");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("SOCKET_BUF", m_nSocketBufSize))
	{
		DSC_RUN_LOG_WARNING("cann't read 'SOCKET_BUF' configure item value");

		return -1;
	}
	else
	{
		if (!m_nSocketBufSize)
		{
			m_nSocketBufSize = 128;
		}
		m_nSocketBufSize *= 1024;
	}

	PROT_COMM::CDscIpAddr schedulerAgentAddr;

	schedulerAgentAddr.SetIpAddr(m_strMqSchedulerServiceAddr);
	schedulerAgentAddr.SetPort(m_nMqSchedulerServicePort);

	m_nSchedulerHandleID = this->AllocHandleID();
	this->DoConnect(schedulerAgentAddr, NULL, m_nSchedulerHandleID);

	return 0;
}

ACE_INT32 CMqClientSdkAgentService::OnExit(void)
{
	m_mqMsgCallback->OnExit();
	
	for (dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::iterator it = m_mapForwarderSession.begin();
		it != m_mapForwarderSession.end(); ++it)
	{
		DSC_THREAD_TYPE_ALLOCATE(it->second);
	}

	return CDscHtsClientService::OnExit();
}

ACE_INT32 CMqClientSdkAgentService::OnHtsMsg(MQ_MSG::CRegMqClientAddrRsp& rRegMqClientAddrRsp, CMcpHandler* pMcpHandler)
{
	PROT_COMM::CDscIpAddr ipAddr;
	dsc_set_type(CForwarderAddrSession)::iterator fas_it;
	CForwarderAddrSession session;

	for (DSC::CDscList<MQ_MSG::CMqForwarderAddr>::iterator it = rRegMqClientAddrRsp.m_lstMqForwarderAddr.begin(); it != rRegMqClientAddrRsp.m_lstMqForwarderAddr.end(); ++it)
	{		
		this->AddMqForwarderAddr(*it);
	}

	if (!m_regMqTopicReq.m_nClientID)
	{
		m_regMqTopicReq.m_nClientID = rRegMqClientAddrRsp.m_nClientID;
	}

	return 0;
}

ACE_INT32 CMqClientSdkAgentService::OnHtsMsg(MQ_MSG::CPubMqTopicMsg& rPubMqTopicMsg, CMcpHandler* pMcpHandler)
{
	//DSC_INTF_LOG_INFO("forwarde topic msg, topic:%d", rPubMqTopicMsg.m_nTopicID);

	this->OnMqMsg(rPubMqTopicMsg);

	return 0;
}

ACE_INT32 CMqClientSdkAgentService::OnHtsMsg(MQ_MSG::CPubMqAddrMsg& rPubMqAddrMsg, CMcpHandler* pMcpHandler)
{
	//DSC_INTF_LOG_INFO("forwarde addr msg, topic:%d", rPubMqAddrMsg.m_nTopicID);

	this->OnMqMsg(rPubMqAddrMsg);

	return 0;
}

void CMqClientSdkAgentService::OnDscMsg(MQ_MSG::CPubMqTopicMsg& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	rMsg.m_nTopicID = m_regMqTopicReq.m_nClientID;

	this->SendDscMsg(rMsg, rSrcMsgAddr);
}

void CMqClientSdkAgentService::OnDscMsg(MQ_MSG::CPubMqAddrMsg& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	this->SendDscMsg(rMsg, rSrcMsgAddr);
}

void CMqClientSdkAgentService::OnDscMsg(MQ_MSG::CPubMqBroadcastMsg& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	rMsg.m_nClientID = m_regMqTopicReq.m_nClientID;
	this->SendDscMsg(rMsg, rSrcMsgAddr);
}

CMcpClientHandler* CMqClientSdkAgentService::AllocMcpHandler(ACE_HANDLE handle, const ACE_UINT32 nHandleID,
	const ACE_INT16 nMaxLostHB, PROT_COMM::CDscIpAddr& rRemoteAddr, PROT_COMM::CDscIpAddr& localAddr)
{
	if (m_nTcpNodelayFlag)
	{
		int nodelay = 1;

		ACE_OS::setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)& nodelay, sizeof(nodelay));
	}

	ACE_OS::setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char*)(&m_nSocketBufSize), sizeof(ACE_INT32));
	ACE_OS::setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (char*)(&m_nSocketBufSize), sizeof(ACE_INT32));

	return ::new(DSC_THREAD_MALLOC(sizeof(CMcpClientHandler))) CMcpClientHandler(*this, handle, nHandleID, rRemoteAddr, localAddr);
}

ACE_INT32 CMqClientSdkAgentService::OnHtsMsg(MQ_MSG::CAddFowrarderAddrNotify& rAddFowrarderAddrNotify, CMcpHandler* pMcpHandler)
{
	this->AddMqForwarderAddr(rAddFowrarderAddrNotify.m_mqForwarderAddr);

	return 0;
}

ACE_INT32 CMqClientSdkAgentService::OnHtsMsg(MQ_MSG::CDelFowrarderAddrNotify& rDelFowrarderAddrNotify, CMcpHandler* pMcpHandler)
{
	PROT_COMM::CDscIpAddr ipAddr;

	ipAddr.SetPort(rDelFowrarderAddrNotify.m_mqForwarderAddr.m_nPort);
	ipAddr.SetIpAddr(rDelFowrarderAddrNotify.m_mqForwarderAddr.m_strIpAddr);

	dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::iterator it = m_mapForwarderSession.find(ipAddr);

	if (it != m_mapForwarderSession.end())
	{
		CForwarderAddrSession* pSession = it->second;

		m_handlerSessionArray.Delete(pSession);
		if (pSession->m_pMcpHandler)
		{
			this->DisConnect(pSession->m_pMcpHandler->GetHandleID());
		}
		else
		{
			if (pSession->m_nHandleID)
			{
				this->CancelConnect(pSession->m_nHandleID);
			}
		}
		m_mapForwarderSession.erase(it);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);
	}

	return 0;
}

void CMqClientSdkAgentService::AddMqForwarderAddr(MQ_MSG::CMqForwarderAddr& addr)
{
	PROT_COMM::CDscIpAddr ipAddr;

	ipAddr.SetPort(addr.m_nPort);
	ipAddr.SetIpAddr(addr.m_strIpAddr);

	dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::iterator it = m_mapForwarderSession.find(ipAddr);

	if (it == m_mapForwarderSession.end())
	{
		CForwarderAddrSession* pSession = DSC_THREAD_TYPE_NEW(CForwarderAddrSession) CForwarderAddrSession;

		pSession->m_nPort = addr.m_nPort;
		pSession->m_strAddr = addr.m_strIpAddr;

		m_mapForwarderSession.insert(dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::value_type(ipAddr, pSession));

		//新收到的forwarder地址发链接请求
		this->DoConnect(ipAddr, NULL, this->AllocHandleID());
	}
	else
	{
	}
}

void CMqClientSdkAgentService::OnNetworkError(CMcpHandler* pMcpHandler)
{
	CMcpClientHandler* pMcpClientHandler = dynamic_cast<CMcpClientHandler*>(pMcpHandler);
	dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::iterator it = m_mapForwarderSession.find(pMcpClientHandler->GetRemoteAddr());

	if (it != m_mapForwarderSession.end())
	{
		CForwarderAddrSession* pSession = it->second;

		pSession->m_pMcpHandler = NULL;
		m_handlerSessionArray.Delete(pSession);
		if (m_handlerSessionArray.IsEmpty())
		{
			m_mqMsgCallback->OnAbnormal();
		}
	}

	CDscHtsClientService::OnNetworkError(pMcpHandler);
}

ACE_INT32 CMqClientSdkAgentService::OnConnectedNodify(CMcpClientHandler* pMcpClientHandler)
{
	PROT_COMM::CDscIpAddr& rRemoteAddr = pMcpClientHandler->GetRemoteAddr();

	if (rRemoteAddr.GetIpAddr() == m_strMqSchedulerServiceAddr && rRemoteAddr.GetPort() == m_nMqSchedulerServicePort)
	{
		MQ_MSG::CRegMqClientAddrReq regMqClientAddrReq;

		regMqClientAddrReq.m_nClientID = m_regMqTopicReq.m_nClientID;

		this->SendHtsMsg(regMqClientAddrReq, rRemoteAddr);
	}
	else
	{
		CForwarderAddrSession* pSession = NULL;
		dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::iterator it = m_mapForwarderSession.find(rRemoteAddr);
		if (it == m_mapForwarderSession.end())
		{
			ACE_ASSERT(false);
			DSC_THREAD_TYPE_ALLOCATE(pSession);

			pSession->m_nPort = rRemoteAddr.GetPort();
			pSession->m_strAddr = rRemoteAddr.GetIpAddr();
			pSession->m_pMcpHandler = pMcpClientHandler;
			pSession->m_nHandleID = pMcpClientHandler->GetHandleID();

			m_mapForwarderSession.insert(dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*)::value_type(rRemoteAddr, pSession));
		}
		else
		{
			pSession = it->second;
			pSession->m_pMcpHandler = pMcpClientHandler;
			pSession->m_nHandleID = pMcpClientHandler->GetHandleID();
		}
		m_handlerSessionArray.Insert(pSession);

		CMqClientSdkAgentService::CForwarderAddrSession fwAddr;

		fwAddr.m_nPort = rRemoteAddr.GetPort();
		fwAddr.m_strAddr = rRemoteAddr.GetIpAddr();

		this->SendHtsMsg(m_regMqTopicReq, pMcpClientHandler->GetHandleID());

		if (m_handlerSessionArray.Size() == 1)
		{
			m_mqMsgCallback->OnReady();
		}
	}

	return 0;
}
