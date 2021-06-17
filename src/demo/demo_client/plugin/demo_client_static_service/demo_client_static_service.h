#ifndef DEMO_CLIENT_STATIC_SERVICE_H_89678678236786783456567
#define DEMO_CLIENT_STATIC_SERVICE_H_89678678236786783456567

#include "dsc/dsc_reactor.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoClientStaticService : public CDscReactor, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_CLIENT_STATIC_SERVICE_TYPE
	};

public:
	CDemoClientStaticService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
	void OnTimer(void);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(NS_DEMO_MSG_DEF::COptSumMsg)
	DSC_BIND_MESSAGE(NS_DEMO_MSG_DEF::CJsonSumMsg)
	DSC_BIND_MESSAGE(NS_DEMO_MSG_DEF::CMcpSumMsg)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(NS_DEMO_MSG_DEF::COptSumMsg& rOptSumMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(NS_DEMO_MSG_DEF::CJsonSumMsg& rJsonSumMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(NS_DEMO_MSG_DEF::CMcpSumMsg& rMcpSumMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

private:
	ACE_UINT64 m_nOptSummary;
	ACE_INT32 m_nJsonSummary;
	ACE_INT32 m_nMcpSummary;
};

#endif
