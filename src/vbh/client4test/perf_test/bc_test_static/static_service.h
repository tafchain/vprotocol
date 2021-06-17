#ifndef STATIC_SERVICE_H_89678678236786783456567
#define STATIC_SERVICE_H_89678678236786783456567

#include "dsc/dsc_reactor.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"

#include "vbh_perf_test_comm/vbh_perf_test_comm_msg_def.h"

class CBcPerfTestStaticService : public CDscReactor, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_SERVICE_TYPE = VBH_TEST::VBH_PERF_TEST_STATIC_SERVICE_TYPE
	};

public:
	CBcPerfTestStaticService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
	void OnTimer(void);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH_TEST::CSumMsg)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH_TEST::CSumMsg& rGetSum, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

private:
	ACE_UINT32 m_nSum = 0;
	double m_dTotal = 0;
	ACE_UINT32 m_nCount = 0;
	ACE_INT32 m_nRegSum = 0;
};

#endif
