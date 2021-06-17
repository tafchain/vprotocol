#include "dsc/protocol/long_connect/long_connect_acceptor.h"
#include "dsc/protocol/otp/dsc_server_otp_service.h"
#include "dsc/protocol/otp/dsc_client_otp_service.h"
#include "dsc/dsc_log.h"

#include "p2p_mq/mq_callback_service.h"

CMqCallbackService::CSubMsgAddr::CSubMsgAddr()
: m_addrSet()
, m_it(m_addrSet.end())
{

}

ACE_INT32 CMqCallbackService::CSubMsgAddr::insert(const CDscMsg::CDscMsgAddr& addr)
{
	dsc_set_type(CDscMsg::CDscMsgAddr)::iterator it = m_addrSet.find(addr);
	if (it == m_addrSet.end())
	{
		m_addrSet.insert(addr);
		m_it = m_addrSet.begin();

		return 0;
	}
	else
	{
		return -1;
	}
}

ACE_INT32 CMqCallbackService::CSubMsgAddr::erase(const CDscMsg::CDscMsgAddr& addr)
{
	dsc_set_type(CDscMsg::CDscMsgAddr)::iterator it = m_addrSet.find(addr);
	if (it == m_addrSet.end())
	{
		return -1;
	}
	else
	{
		m_addrSet.erase(it);
		m_it = m_addrSet.begin();

		return 0;
	}
}

bool CMqCallbackService::CSubMsgAddr::empty(void)
{
	return m_addrSet.empty();
}

ACE_INT32 CMqCallbackService::CSubMsgAddr::GetAddr(CDscMsg::CDscMsgAddr& addr)
{
	if (m_addrSet.empty())
	{
		return -1;
	}
	else
	{
		if (m_it == m_addrSet.end())
		{
			m_it = m_addrSet.begin();
		}

		addr = *m_it;
		++m_it;

		return 0;
	}
}

CMqCallbackService::CMqCallbackService(IP2pMqMsgCallback* pP2pMqMsgCallback)
: m_pP2pMqMsgCallback(pP2pMqMsgCallback)
, m_mapTopicRegistry(DSC::DscGetPrimeNumber(1024 * 1024))
{
}

ACE_INT32 CMqCallbackService::OnInit(void)
{
	if(CDscMqThreadTask::OnInit())
	{
		DSC_RUN_LOG_ERROR("s28 scheduler service init failed!");

		return -1;
	}

	DSC_RUN_LOG_FINE("s28 scheduler service init succeed!");

	return 0;
}

ACE_INT32 CMqCallbackService::OnExit(void)
{
	m_pP2pMqMsgCallback->Release();

	return CDscMqThreadTask::OnExit();
}

void CMqCallbackService::OnDscMsg(MQ_ID::CNetworkAddrMsg& rNetworkAddrMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	CLongConnectAcceptor<CDscServerOtpService>* pLongConnectAcceptor = NULL;

	DSC_NEW(pLongConnectAcceptor, CLongConnectAcceptor<CDscServerOtpService>);
	if (pLongConnectAcceptor)
	{
		pLongConnectAcceptor->SetType(DSC::EN_DSC_OTP_ACCEPTOR_TYPE);
		pLongConnectAcceptor->SetID(1);

		if (pLongConnectAcceptor->open(rNetworkAddrMsg.m_listenAddr.m_nPort, rNetworkAddrMsg.m_listenAddr.m_pIpAddr))
		{
			DSC_RUN_LOG_ERROR("listen failed, port:%d, ip addr:%s", rNetworkAddrMsg.m_listenAddr.m_nPort, rNetworkAddrMsg.m_listenAddr.m_pIpAddr.c_str());
		}
	}

	CDscClientOtpService* pClientDscProtService = NULL;
	for (DSC::CDscList<MQ_ID::CConnectAddr>::iterator it = rNetworkAddrMsg.m_lstConnectAddr.begin(); it != rNetworkAddrMsg.m_lstConnectAddr.end(); ++it)
	{
		DSC_NEW(pClientDscProtService, CDscClientOtpService);
		if (pClientDscProtService)
		{
			if (-1 == it->m_localAddr.m_nPort)
			{
				pClientDscProtService->Connect(it->m_remoteAddr.m_pIpAddr.c_str(), it->m_remoteAddr.m_nPort);
			}
			else
			{
				pClientDscProtService->Connect(it->m_remoteAddr.m_pIpAddr.c_str(), it->m_remoteAddr.m_nPort, it->m_localAddr.m_pIpAddr.c_str(), it->m_localAddr.m_nPort);
			}
		}
	}
}

void CMqCallbackService::OnConnected(const CDscConnectInfo& rNetInfo)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetNodeType(rNetInfo.m_remoteConnectInfo.m_nType);
	addr.SetNodeID(rNetInfo.m_remoteConnectInfo.m_nID);
	addr.SetServiceType(CMqCallbackService::EN_SERVICE_TYPE);
	addr.SetServiceID(1);

	dsc_set_type(CDscMsg::CDscMsgAddr)::iterator it = m_setRemoteAddr.find(addr);
	
	if (it == m_setRemoteAddr.end())
	{
		m_setRemoteAddr.insert(addr);

		MQ_ID::CSubMsgID msg;

		msg.m_lstMsgID = m_lstMsgID;

		this->SendDscMessage(msg, addr);
	}
}

void CMqCallbackService::OnDisConnected(const CDscConnectInfo& rNetInfo)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetNodeType(rNetInfo.m_remoteConnectInfo.m_nType);
	addr.SetNodeID(rNetInfo.m_remoteConnectInfo.m_nID);
	addr.SetServiceType(CMqCallbackService::EN_SERVICE_TYPE);
	addr.SetServiceID(1);

	dsc_set_type(CDscMsg::CDscMsgAddr)::iterator it = m_setRemoteAddr.find(addr);

	if (it != m_setRemoteAddr.end())
	{
		m_setRemoteAddr.erase(it);
	
		CSubMsgAddr* pSubMsgAddr;
		for (topic_registry_type::iterator it = m_mapTopicRegistry.begin(); it != m_mapTopicRegistry.end();)
		{
			pSubMsgAddr = it->second;

			pSubMsgAddr->erase(addr);
			if (pSubMsgAddr->empty())
			{
				DSC_THREAD_TYPE_DEALLOCATE(pSubMsgAddr);
				it = m_mapTopicRegistry.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void CMqCallbackService::OnDscMsg(MQ_ID::CSubMsgID2CallbackService& rSubMsgID, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	m_lstMsgID = rSubMsgID.m_lstMsgID;

	MQ_ID::CSubMsgID msg;

	msg.m_lstMsgID = rSubMsgID.m_lstMsgID;

	for (dsc_set_type(CDscMsg::CDscMsgAddr)::iterator it = m_setRemoteAddr.begin(); it != m_setRemoteAddr.end(); ++it)
	{
		this->SendDscMessage(msg, *it);
	}
}

void CMqCallbackService::OnDscMsg(MQ_ID::CSubMsgID& rSubMsgID, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	topic_registry_type::iterator reg_it;
	CSubMsgAddr* pSubMsgAddr;

	for (DSC::CDscList<ACE_INT32>::iterator it = rSubMsgID.m_lstMsgID.begin(); it != rSubMsgID.m_lstMsgID.end(); ++it)
	{
		reg_it = m_mapTopicRegistry.find(*it);
		if (reg_it == m_mapTopicRegistry.end())
		{
			DSC_THREAD_TYPE_ALLOCATE(pSubMsgAddr);
			m_mapTopicRegistry.insert(topic_registry_type::value_type(*it, pSubMsgAddr));
			pSubMsgAddr->insert(rSrcMsgAddr);
		}
		else
		{
			reg_it->second->insert(rSrcMsgAddr);
		}
	}
}

void CMqCallbackService::OnDscMsg(MQ_ID::CPubMsg2CallbackService& rPubMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	topic_registry_type::iterator it = m_mapTopicRegistry.find(rPubMsg.m_nTopicID);

	if (it == m_mapTopicRegistry.end())
	{
		DSC_RUN_LOG_INFO("cann't find topic:%d", rPubMsg.m_nTopicID);
	}
	else
	{
		MQ_ID::CPubMsg msg;
		CSubMsgAddr* pSubMsgAddr = it->second;
		CDscMsg::CDscMsgAddr addr;

		msg.m_nTopicID = rPubMsg.m_nTopicID;
		msg.m_msgBlob.SetBuffer(rPubMsg.m_msgBlob.GetBuffer());
		msg.m_msgBlob.SetSize(rPubMsg.m_msgBlob.GetSize());

		if (rPubMsg.m_nPubMsgType == MQ_ID::CPubMsg2CallbackService::EN_POLL_TYPE)
		{
			pSubMsgAddr->GetAddr(addr);
			this->SendDscMessage(msg, addr);
		}
		else
		{
			for (dsc_set_type(CDscMsg::CDscMsgAddr)::iterator addr_it = pSubMsgAddr->m_addrSet.begin(); addr_it != pSubMsgAddr->m_addrSet.end(); ++addr_it)
			{
				this->SendDscMessage(msg, *addr_it);
			}
		}

		msg.m_msgBlob.SetBuffer(NULL);
		msg.m_msgBlob.SetSize(0);
	}

	rPubMsg.m_msgBlob.SetBuffer(NULL);
	rPubMsg.m_msgBlob.SetSize(0);
}

void CMqCallbackService::OnDscMsg(MQ_ID::CPubMsg& rPubMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	CP2pMqAddrMsg msg;

	msg.m_nAppType = rSrcMsgAddr.GetNodeType();
	msg.m_nAppID = rSrcMsgAddr.GetNodeID();
	msg.m_msg.m_nTopicID = rPubMsg.m_nTopicID;
	msg.m_msg.m_pMsg = rPubMsg.m_msgBlob.GetBuffer();
	msg.m_msg.m_nMsgSize = rPubMsg.m_msgBlob.GetSize();

	m_pP2pMqMsgCallback->OnMessage(msg);

	rPubMsg.m_msgBlob.SetBuffer(NULL);
	rPubMsg.m_msgBlob.SetSize(0);
}
