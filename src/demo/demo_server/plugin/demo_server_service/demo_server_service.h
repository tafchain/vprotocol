#ifndef DEMO_SERVER_SERVICE_H_439824397821349423963456458574326943296342856342856
#define DEMO_SERVER_SERVICE_H_439824397821349423963456458574326943296342856342856

#include "dsc/dsc_reactor.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoServerService : public CDscReactor
{
private:
	class CTestTimer : public CDscServiceTimerHandler
	{
	public:
		CTestTimer(CDemoServerService& rDemoServerService);

	public:
		virtual void OnTimer(void);
		CDemoServerService& m_rDemoServerService;
	};

public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_SERVER_SERVICE_TYPE
	};

public:
	CDemoServerService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

	void OnTimeOut(CTestTimer* pTestTimer);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(NS_DEMO_MSG_DEF::CDemoReqMsg)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& rDemoReqMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

//private:
//	CTestTimer m_TestTimer1;
//	CTestTimer m_TestTimer2;
//	CTestTimer m_TestTimer3;
//	CTestTimer m_TestTimer4;
//	CTestTimer m_TestTimer5;
//	CTestTimer m_TestTimer6;
//	CTestTimer m_TestTimer7;
//	CTestTimer m_TestTimer8;
//	CTestTimer m_TestTimer9;
//	CTestTimer m_TestTimer10;
};

#endif