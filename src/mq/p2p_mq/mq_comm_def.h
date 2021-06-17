#ifndef S28_CLIENT_SDK_COMMON_MSG_DEF_H_34873421809234168213234165
#define S28_CLIENT_SDK_COMMON_MSG_DEF_H_34873421809234168213234165

#include "dsc/codec/dsc_codec/dsc_codec.h"

#include "p2p_mq/mq_comm_def.h"

namespace MQ_ID
{
	enum
	{
		EN_MQ_SERVICE_TYPE = DSC::EN_MIN_USER_TASK_TYPE + 1
	};

	enum
	{
		EN_NETWORK_ADDR_MSG = DSC::EN_MIN_USER_MSG + 1,
		EN_SUB_MSG_ID_2_CALLBACK_SERVICE_MSG,
		EN_SUB_MSG_ID_MSG,
		EN_PUB_MSG_2_CALLBACK_SERVICE_MSG,
		EN_PUB_MSG
	};

	class CNetAddr
	{
	public:
		DSC_BIND_ATTR(m_nPort, m_pIpAddr)

	public:
		ACE_INT16 m_nPort;
		CDscString m_pIpAddr;
	};

	class CConnectAddr
	{
	public:
		CConnectAddr()
		{
			m_localAddr.m_nPort = -1;
		}

	public:
		DSC_BIND_ATTR(m_localAddr, m_remoteAddr)

	public:
		MQ_ID::CNetAddr m_localAddr;
		MQ_ID::CNetAddr m_remoteAddr;
	};

	class CNetworkAddrMsg
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_NETWORK_ADDR_MSG
		};

	public:
		DSC_BIND_ATTR(m_listenAddr, m_lstConnectAddr)

	public:
		MQ_ID::CNetAddr m_listenAddr;
		DSC::CDscList<MQ_ID::CConnectAddr> m_lstConnectAddr;
	};

	class CSubMsgID2CallbackService
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_SUB_MSG_ID_2_CALLBACK_SERVICE_MSG
		};

	public:
		DSC_BIND_ATTR(m_lstMsgID)

	public:
		DSC::CDscList<ACE_INT32> m_lstMsgID;
	};

	class CSubMsgID
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_SUB_MSG_ID_MSG
		};

	public:
		DSC_BIND_ATTR(m_lstMsgID)

	public:
		DSC::CDscList<ACE_INT32> m_lstMsgID;
	};

	class CPubMsg2CallbackService
	{
	public:
		enum EN_PUB_MSG_TYPE
		{
			EN_BROADCAST_TYPE,
			EN_POLL_TYPE
		};

	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_PUB_MSG_2_CALLBACK_SERVICE_MSG
		};

	public:
		DSC_BIND_ATTR(m_nTopicID, m_msgBlob)

	public:
		ACE_INT32 m_nPubMsgType;
		ACE_INT32 m_nTopicID;
		DSC::CDscBlob m_msgBlob;
	};

	class CPubMsg
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_PUB_MSG
		};

	public:
		DSC_BIND_ATTR(m_nTopicID, m_msgBlob)

	public:
		ACE_INT32 m_nTopicID;
		DSC::CDscBlob m_msgBlob;
	};
}

#endif
