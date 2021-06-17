#ifndef CC_JSON_DEMO_SERVICE_H_3428723472314231213863217
#define CC_JSON_DEMO_SERVICE_H_3428723472314231213863217

#include "dsc/mem_mng/dsc_stl_type.h"

#include "common/cc_json_service.h"
#include "common/cc_comm_id_def.h"
#include "bc/vbh_test/vbh_test_comm/vbh_test_trade_def.h"

class PLUGIN_EXPORT CCcJsonDemoService : public CCcJsonService
{
public:
	enum
	{
		EN_HASH_MAP_BITES = 16,
		EN_SESSION_TIMEOUT_VALUE = 60,
		EN_SERVICE_TYPE = CC::EN_CC_JSON_DEMO_SERVICE_TYPE
	};

	class CSession : public CDscServiceTimerHandler
	{
	public:
		CSession(CCcJsonDemoService& rCcDemoService);

		virtual void OnTimer(void);

	public:
		CTestTradeUser m_userA;
		CTestTradeUser m_userB;
		CTestDemoTrade m_demoTrade;
		ACE_INT32 m_nRspNum = 0;

	public:
		ACE_UINT32 m_nKey = 0;
		CSession* m_pPrev = NULL;
		CSession* m_pNext = NULL;

	protected:
		CCcJsonDemoService& m_rCcService;
	};

public:
	CCcJsonDemoService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

public:
	void OnTimeOut(CSession* pUserSession);

public:
	BEGIN_BIND_JSON_PROPOSAL_PROC
	BIND_JSON_PROPOSAL_PROC(CTestDemoTrade)
	END_BIND_JSON_PROPOSAL_PROC

protected:
	virtual ACE_INT32 OnPorposal(const ACE_UINT32 nSessionID, CTestDemoTrade& rTestDemoTrade);
	virtual void OnGetVbhUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT32 nSessionID, CSimpleUser& rUser);

private:
	using session_type = CBareHashMap<ACE_UINT32, CSession, EN_HASH_MAP_BITES>;

	session_type m_mapSession;
};

#include "cc_json_demo_service/cc_json_demo_service.inl"

#endif
