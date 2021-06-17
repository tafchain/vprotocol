#ifndef MQ_COMM_MSG_DEF_H_34897234673216823168236523157623156
#define MQ_COMM_MSG_DEF_H_34897234673216823168236523157623156

#include "dsc/codec/dsc_codec/dsc_codec.h"

#include "mq_common/mq_id_def.h"

namespace MQ_MSG
{
	class CRegistMqForwarderServiceReq
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_REGIST_MQ_FORWARDER_SERVICE_MSG
		};

	public:
		DSC_BIND_ATTR(m_nPort, m_strAddr)

	public:
		ACE_INT16  m_nPort;
		CDscString m_strAddr;
	};

	class CRegMqClientAddrReq
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_REGIST_MQ_CLIENT_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nClientID)

	public:
		ACE_UINT32 m_nClientID;
	};

	class CMqForwarderAddr
	{
	public:
		DSC_BIND_ATTR(m_nPort, m_strIpAddr)

	public:
		ACE_INT32  m_nPort;
		CDscString m_strIpAddr;
	};

	class CRegMqClientAddrRsp
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_REGIST_MQ_CLIENT_RSP_MSG
		};

	public:
		DSC_BIND_ATTR(m_nClientID, m_lstMqForwarderAddr)

	public:
		ACE_UINT32 m_nClientID;
		DSC::CDscList<MQ_MSG::CMqForwarderAddr> m_lstMqForwarderAddr;
	};

	class CAddFowrarderAddrNotify
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_ADD_FOWRARDER_ADDR_NOTIFY_MSG
		};

	public:
		DSC_BIND_ATTR(m_mqForwarderAddr)

	public:
		MQ_MSG::CMqForwarderAddr m_mqForwarderAddr;
	};

	class CDelFowrarderAddrNotify
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_DEL_FOWRARDER_ADDR_NOTIFY_MSG
		};

	public:
		DSC_BIND_ATTR(m_mqForwarderAddr)

	public:
		MQ_MSG::CMqForwarderAddr m_mqForwarderAddr;
	};

	class CRegMqTopicReq
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_REGIST_MQ_TOPIC_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nClientID, m_lstTopicID)

	public:
		ACE_UINT32 m_nClientID;
		DSC::CDscList<ACE_UINT32> m_lstTopicID;
	};

	class CPubMqTopicMsg
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_PUB_MQ_TOPIC_MSG
		};

	public:
		DSC_BIND_ATTR(m_nTopicID, m_msgBlob)

	public:
		ACE_UINT32 m_nTopicID;
		DSC::CDscBlob m_msgBlob;
	};

	class CPubMqAddrMsg
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_PUB_MQ_ADDR_MSG
		};

	public:
		DSC_BIND_ATTR(m_nClientID, m_nTopicID, m_msgBlob)

	public:
		ACE_UINT32 m_nClientID;
		ACE_UINT32 m_nTopicID;
		DSC::CDscBlob m_msgBlob;
	};

	class CPubMqBroadcastMsg
	{
	public:
		enum
		{
			EN_MSG_ID = MQ_ID::EN_PUB_MQ_BROADCAST_MSG
		};

	public:
		DSC_BIND_ATTR(m_nClientID, m_nTopicID, m_msgBlob)

	public:
		ACE_UINT32 m_nClientID;
		ACE_UINT32 m_nTopicID;
		DSC::CDscBlob m_msgBlob;
	};
}

#endif
