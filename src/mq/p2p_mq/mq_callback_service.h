#ifndef S28_CLIENT_SDK_CALLBACK_SERVICE_H_3428234809457982346721368721865678328216823
#define S28_CLIENT_SDK_CALLBACK_SERVICE_H_3428234809457982346721368721865678328216823

#include "dsc/mem_mng/dsc_stl_type.h"
#include "dsc/dsc_mq_thread_task.h"

#include "p2p_mq/mq_comm_def.h"
#include "p2p_mq/mq_if_def.h"

class CMqCallbackService : public CDscMqThreadTask
{
private:
	class CSubMsgAddr
	{
	public:
		CSubMsgAddr();

	public:
		ACE_INT32 insert(const CDscMsg::CDscMsgAddr& addr);
		ACE_INT32 erase(const CDscMsg::CDscMsgAddr& addr);
		bool empty(void);
		ACE_INT32 GetAddr(CDscMsg::CDscMsgAddr& addr);

	public:
		dsc_set_type(CDscMsg::CDscMsgAddr) m_addrSet;
		dsc_set_type(CDscMsg::CDscMsgAddr)::iterator m_it;
	};

public:
	enum
	{
		EN_SERVICE_TYPE = MQ_ID::EN_MQ_SERVICE_TYPE,
	};

public:
	CMqCallbackService(IP2pMqMsgCallback* pP2pMqMsgCallback);

protected:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
	virtual void OnConnected(const CDscConnectInfo& rNetInfo);
	virtual void OnDisConnected(const CDscConnectInfo& rNetInfo);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(MQ_ID::CNetworkAddrMsg)
	DSC_BIND_MESSAGE(MQ_ID::CSubMsgID2CallbackService)
	DSC_BIND_MESSAGE(MQ_ID::CSubMsgID)
	DSC_BIND_MESSAGE(MQ_ID::CPubMsg2CallbackService)
	DSC_BIND_MESSAGE(MQ_ID::CPubMsg)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(MQ_ID::CNetworkAddrMsg& rNetworkAddrMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(MQ_ID::CSubMsgID2CallbackService& rSubMsgID, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(MQ_ID::CSubMsgID& rSubMsgID, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(MQ_ID::CPubMsg2CallbackService& rPubMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(MQ_ID::CPubMsg& rPubMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

private:
	using topic_registry_type = dsc_unordered_map_type(ACE_INT32, CSubMsgAddr*);

private:
	IP2pMqMsgCallback* m_pP2pMqMsgCallback;
	dsc_set_type(CDscMsg::CDscMsgAddr) m_setRemoteAddr;//保存与对端连接的地址
	topic_registry_type m_mapTopicRegistry;//topic登记处
	DSC::CDscList<ACE_INT32> m_lstMsgID;
};

#endif
