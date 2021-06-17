#ifndef DEMO_CLIENT_SERVICE_H_80950985480734280923498056908895487923418079578035807423780
#define DEMO_CLIENT_SERVICE_H_80950985480734280923498056908895487923418079578035807423780

#include "dsc/dsc_reactor.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoClientService : public CDscReactor, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_CLIENT_SERVICE_TYPE
	};

public:
	CDemoClientService();

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(NS_DEMO_MSG_DEF::CDemoRspMsg)
	END_BIND_DSC_MESSAGE

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);
	void OnTimer(void);

public:
	void OnDscMsg(NS_DEMO_MSG_DEF::CDemoRspMsg& rDemoRspMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

protected:
	virtual void OnConnected(const CDscConnectInfo& rNetInfo);
	virtual void OnPeerRegMsgType(const CDscConnectInfo& rNetInfo);

private:
	void InitDemoReqMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& reqMsg);

private:
	ACE_UINT64 m_nSummary;
};

#endif
