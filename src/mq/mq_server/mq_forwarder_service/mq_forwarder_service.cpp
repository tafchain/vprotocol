#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/netinet/os_tcp.h"

#include "dsc/configure/dsc_configure.h"

#include "mq_forwarder_service/mq_forwarder_service.h"

CMqForwarderService::CMqForwarderService()
{
}

ACE_INT32 CMqForwarderService::OnInit(void)
{
	if(CDscHtsServerService::OnInit())
	{
		DSC_RUN_LOG_ERROR("mq Subscriber service init failed!");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileString("MQ_FORWARDER_IP_ADDR", m_strMqForwarderAddr))
	{
		DSC_RUN_LOG_WARNING("cann't read 'MQ_FORWARDER_IP_ADDR' configure item value");

		return -1;
	}

	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("MQ_FORWARDER_PORT", m_nMqForwarderPort))
	{
		DSC_RUN_LOG_WARNING("cann't read 'MQ_FORWARDER_PORT' configure item value");

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

	m_nMqForwarderPort += this->GetID();
	m_pMqForwarderServiceAcceptor = ::new(DSC_THREAD_SIZE_MALLOC(sizeof(CServiceAsynchAcceptor<CMqForwarderService>))) CServiceAsynchAcceptor<CMqForwarderService>(*this);
	if (m_pMqForwarderServiceAcceptor->Open(m_nMqForwarderPort, m_strMqForwarderAddr.c_str()))
	{
		DSC_RUN_LOG_WARNING("open failed, ip addr:%s, port:%d!", m_strMqForwarderAddr.c_str(), m_nMqForwarderPort);

		return -1;
	}
	else
	{
		if (-1 == this->RegistHandler(m_pMqForwarderServiceAcceptor, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL))
		{
			DSC_RUN_LOG_WARNING("regist handler failed, ip addr:%s, port:%d", m_strMqForwarderAddr.c_str(), m_nMqForwarderPort);

			return -1;
		}
	}

	return 0;
}

ACE_INT32 CMqForwarderService::OnExit( void )
{
	MQ_MSG::CDelFowrarderAddrNotify notify;
	CClientIDSession* pClientIDSession;

	for (CDscHashMap<CClientIDSession, EN_HASH_MAP_SIZE>::iterator it = m_mapMqClientIDSession.begin(); it != m_mapMqClientIDSession.end(); ++it)
	{
		pClientIDSession = it.second;
		notify.m_mqForwarderAddr.m_nPort = m_nMqForwarderPort;
		notify.m_mqForwarderAddr.m_strIpAddr = m_strMqForwarderAddr;

		this->SendHtsMsg(notify, pClientIDSession->m_pMcpHandler);
	
		dsc_list_type(CHandlerSessionItem*)& lstHandlerSessionItem = pClientIDSession->m_lstHandlerSessionItem;
		for (dsc_list_type(CHandlerSessionItem*)::iterator hsi_it = lstHandlerSessionItem.begin(); hsi_it != lstHandlerSessionItem.end(); ++hsi_it)
		{
			DSC_THREAD_TYPE_DEALLOCATE(*hsi_it);
		}
		DSC_THREAD_TYPE_DEALLOCATE(pClientIDSession);
	}

	for (CDscHashMap<CTopicSession, EN_HASH_MAP_SIZE>::iterator it = m_mapTopicSession.begin(); it != m_mapTopicSession.end(); ++it)
	{
		DSC_THREAD_TYPE_DEALLOCATE(it.second);
	}

	m_pMqForwarderServiceAcceptor->Close();
	m_pMqForwarderServiceAcceptor->ReleaseServiceHandler();
	m_mapTopicSession.Clear();
	m_mapMqClientIDSession.Clear();
	m_mapHandlerSession.Clear();

	return CDscHtsServerService::OnExit();
}

CMcpServerHandler* CMqForwarderService::AllocMcpHandler(ACE_HANDLE handle)
{
	if (m_nTcpNodelayFlag)
	{
		int nodelay = 1;

		ACE_OS::setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay));
	}

	ACE_OS::setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char *)(&m_nSocketBufSize), sizeof(ACE_INT32));
	ACE_OS::setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (char *)(&m_nSocketBufSize), sizeof(ACE_INT32));

	return CDscHtsServerService::AllocMcpHandler(handle);
}

void CMqForwarderService::OnConnected(const CDscConnectInfo& rNetInfo)
{
	if (rNetInfo.m_remoteConnectInfo.m_nType == MQ_ID::EN_MQ_SCHEDULER_NODE_TYPE)
	{
		CDscMsg::CDscMsgAddr addr;
		MQ_MSG::CRegistMqForwarderServiceReq req;

		addr.SetNodeType(MQ_ID::EN_MQ_SCHEDULER_NODE_TYPE);
		addr.SetNodeID(MQ_ID::EN_MQ_SCHEDULER_NODE_ID);
		addr.SetServiceType(MQ_ID::EN_MQ_SCHEDULER_SERVICE_TYPE);
		addr.SetServiceID(MQ_ID::EN_MQ_SCHEDULER_SERVICE_ID);

		req.m_strAddr = m_strMqForwarderAddr;
		req.m_nPort = m_nMqForwarderPort;

		this->SendDscMessage(req, addr);
	}
}

void CMqForwarderService::OnNetworkError(CMcpHandler* pMcpHandler)
{
	if (m_bDurindBroadcastMsg)
	{
		this->UnRegistHandler(pMcpHandler, ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL);
		ACE_OS::closesocket(pMcpHandler->get_handle());
		pMcpHandler->set_handle(ACE_INVALID_HANDLE);
		m_stackMcpHandler.push(pMcpHandler);
	}
	else
	{
		CHandlerSession* pHandlerSession = m_mapHandlerSession.Find(pMcpHandler->GetHandleID());
		if (pHandlerSession)
		{
			CHandlerSessionItem* pHandlerSessionItem;
			CTopicSession* pTopicSession;

			m_mapMqClientIDSession.Erase(pHandlerSession->m_nClientID);
			dsc_list_type(CHandlerSessionItem*)& lstHandlerSessionItem = pHandlerSession->m_lstHandlerSessionItem;

			for (DSC::CDscList<CHandlerSessionItem*>::iterator hsi_it = lstHandlerSessionItem.begin(); hsi_it != lstHandlerSessionItem.end(); ++hsi_it)
			{
				pHandlerSessionItem = *hsi_it;

				pTopicSession = pHandlerSessionItem->m_pTopicSession;
				pTopicSession->m_HandlerSessionArray.Delete(pHandlerSessionItem);
				if (pTopicSession->m_HandlerSessionArray.IsEmpty())
				{
					m_mapTopicSession.Erase(pTopicSession->m_nTopicID);
					DSC_THREAD_TYPE_DEALLOCATE(pTopicSession);
				}
				DSC_THREAD_TYPE_DEALLOCATE(pHandlerSessionItem);
			}

			DSC_THREAD_TYPE_DEALLOCATE(pHandlerSession);
			m_mapHandlerSession.Erase(pMcpHandler->GetHandleID());
		}

		CDscHtsServerService::OnNetworkError(pMcpHandler);
	}
}

//算法策略，重复注册请求，后者将前者冲掉，不支持增量注册
ACE_INT32 CMqForwarderService::OnHtsMsg(MQ_MSG::CRegMqTopicReq& rRegMqTopicReq, CMcpHandler* pMcpHandler)
{
	//1、清理,若有重复client ID，踢掉以前的
	CClientIDSession* pClientIDSession = m_mapMqClientIDSession.Find(rRegMqTopicReq.m_nClientID);
	CTopicSession* pTopicSession;
	CHandlerSessionItem* pHandlerSessionItem;

	if (pClientIDSession)
	{
		dsc_list_type(CHandlerSessionItem*)& lstHandlerSessionItem = pClientIDSession->m_lstHandlerSessionItem;

		for (dsc_list_type(CHandlerSessionItem*)::iterator it = lstHandlerSessionItem.begin(); it != lstHandlerSessionItem.end(); ++it)
		{
			pHandlerSessionItem = *it;
			pTopicSession = pHandlerSessionItem->m_pTopicSession;

			pTopicSession->m_HandlerSessionArray.Delete(pHandlerSessionItem);
			if (pTopicSession->m_HandlerSessionArray.IsEmpty())
			{
				m_mapTopicSession.Erase(pTopicSession->m_nTopicID);
				DSC_THREAD_TYPE_DEALLOCATE(pTopicSession);
			}
			DSC_THREAD_TYPE_DEALLOCATE(pHandlerSessionItem);
		}
		
		const ACE_UINT32 nTempHandleID = pClientIDSession->m_pMcpHandler->GetHandleID();

		DSC_RUN_LOG_WARNING("repeat reg topic it:%d\n", rRegMqTopicReq.m_nClientID);
		m_mapHandlerSession.Erase(nTempHandleID);
		m_mapMqClientIDSession.Erase(rRegMqTopicReq.m_nClientID);
		this->DisConnect(nTempHandleID);

		DSC_THREAD_TYPE_DEALLOCATE(pClientIDSession);
	}

	//1、注册
	ACE_UINT32 nTopicID;
	DSC::CDscList<ACE_UINT32>& lstTopicID = rRegMqTopicReq.m_lstTopicID;

	DSC_THREAD_TYPE_ALLOCATE(pClientIDSession);

	pClientIDSession->m_nClientID = rRegMqTopicReq.m_nClientID;
	pClientIDSession->m_pMcpHandler = pMcpHandler;
	m_mapMqClientIDSession.Insert(rRegMqTopicReq.m_nClientID, pClientIDSession);
	m_mapHandlerSession.Insert(pMcpHandler->GetHandleID(), pClientIDSession);

	for (DSC::CDscList<ACE_UINT32>::iterator lst_it = lstTopicID.begin(); lst_it != lstTopicID.end(); ++lst_it)
	{
		nTopicID = *lst_it;

		pTopicSession = m_mapTopicSession.Find(nTopicID);
		if (!pTopicSession)
		{
			DSC_THREAD_TYPE_ALLOCATE(pTopicSession);
			pTopicSession->m_nTopicID = nTopicID;
			m_mapTopicSession.Insert(nTopicID, pTopicSession);
		}

		DSC_THREAD_TYPE_ALLOCATE(pHandlerSessionItem);
		pHandlerSessionItem->m_nClientID = rRegMqTopicReq.m_nClientID;
		pHandlerSessionItem->m_pTopicSession = pTopicSession;
		pHandlerSessionItem->m_pMcpHandler = pMcpHandler;
		pTopicSession->m_HandlerSessionArray.Insert(pHandlerSessionItem);
		pClientIDSession->m_lstHandlerSessionItem.push_back(pHandlerSessionItem);
	}

	return 0;
}

ACE_INT32 CMqForwarderService::OnHtsMsg(MQ_MSG::CPubMqTopicMsg& rPubMqTopicMsg, CMcpHandler* pMcpHandler)
{
	//DSC_INTF_LOG_INFO("forwarde topic msg, topic:%d", rPubMqTopicMsg.m_nTopicID);

	CTopicSession* pTopicSession = m_mapTopicSession.Find(rPubMqTopicMsg.m_nTopicID);
	if (DSC_LIKELY(pTopicSession))
	{
		CHandlerSessionItem* pHandlerSessionItem = pTopicSession->m_HandlerSessionArray.Get();
		if (pHandlerSessionItem)
		{
			if (this->SendHtsMsg(rPubMqTopicMsg, pHandlerSessionItem->m_pMcpHandler))
			{
				DSC_RUN_LOG_INFO("forwarde topic msg failed, topic:%d", rPubMqTopicMsg.m_nTopicID);
			}
		}
		else
		{
			DSC_RUN_LOG_INFO("cann't find mq client by topic:%d", rPubMqTopicMsg.m_nTopicID);
		}
	}
	else
	{
		DSC_RUN_LOG_INFO("cann't find topic:%d", rPubMqTopicMsg.m_nTopicID);
	}

	return 0;
}

ACE_INT32 CMqForwarderService::OnHtsMsg(MQ_MSG::CPubMqAddrMsg& rPubMqAddrMsg, CMcpHandler* pMcpHandler)
{
	//DSC_INTF_LOG_INFO("forwarde addr msg, topic:%d", rPubMqAddrMsg.m_nTopicID);

	CClientIDSession* pClientIDSession = m_mapMqClientIDSession.Find(rPubMqAddrMsg.m_nClientID);
	if (DSC_LIKELY(pClientIDSession))
	{
		if (this->SendHtsMsg(rPubMqAddrMsg, pClientIDSession->m_pMcpHandler))
		{
			DSC_RUN_LOG_INFO("forwarde topic msg failed, topic:%d", rPubMqAddrMsg.m_nTopicID);
		}
	}
	else
	{
		DSC_RUN_LOG_INFO("cann't find topic:%d", rPubMqAddrMsg.m_nTopicID);
	}

	return 0;
}

ACE_INT32 CMqForwarderService::OnHtsMsg(MQ_MSG::CPubMqBroadcastMsg& rPubMqBroadcastMsg, CMcpHandler* pMcpHandler)
{
	//DSC_INTF_LOG_INFO("forwarde broadcast msg, topic:%d", rPubMqBroadcastMsg.m_nTopicID);

	CTopicSession* pTopicSession = m_mapTopicSession.Find(rPubMqBroadcastMsg.m_nTopicID);
	if (DSC_LIKELY(pTopicSession))
	{
		MQ_MSG::CPubMqTopicMsg pubMqTopicMsg;
		CTypeArray<CHandlerSessionItem>& rHandlerSessionArray = pTopicSession->m_HandlerSessionArray;

		pubMqTopicMsg.m_nClientID = rPubMqBroadcastMsg.m_nClientID;
		pubMqTopicMsg.m_nTopicID = rPubMqBroadcastMsg.m_nTopicID;
		pubMqTopicMsg.m_msgBlob = rPubMqBroadcastMsg.m_msgBlob;

		CHandlerSessionItem* pHandlerSessionItem = rHandlerSessionArray.Begin();
		m_bDurindBroadcastMsg = true;

		while (pHandlerSessionItem)
		{
			if (this->SendHtsMsg(pubMqTopicMsg, pHandlerSessionItem->m_pMcpHandler))
			{
				DSC_RUN_LOG_INFO("forwarde topic msg failed, topic:%d", pubMqTopicMsg.m_nTopicID);
			}

			pHandlerSessionItem = rHandlerSessionArray.Next(pHandlerSessionItem);
		}
		m_bDurindBroadcastMsg = false;
		
		CMcpHandler* pstackMcpHandler = m_stackMcpHandler.top();
		
		while (pstackMcpHandler)
		{
			this->OnNetworkError(pstackMcpHandler);
			pstackMcpHandler = m_stackMcpHandler.top();
			m_stackMcpHandler.pop();
		}
	}
	else
	{
		DSC_RUN_LOG_INFO("cann't find topic:%d", rPubMqBroadcastMsg.m_nTopicID);
	}

	return 0;
}
