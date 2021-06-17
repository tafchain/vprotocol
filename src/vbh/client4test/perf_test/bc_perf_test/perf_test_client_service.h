#ifndef PERF_TEST_CLIENT_SERVICE_H_243782347832156213543125
#define PERF_TEST_CLIENT_SERVICE_H_243782347832156213543125

#include "dsc/service_timer/dsc_service_timer_handler.h"
#include "vbh_sdk_comm/vbh_client_service.h"
#include "vbh_perf_test_comm/vbh_perf_test_comm_msg_def.h"
#include "vbh_test_comm/vbh_test_trade_def.h"

class CBcPerfTestService : public CBcClientService, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_HASH_MAP_SIZE = 1 << 16,
		EN_SERVICE_TYPE = VBH_TEST::VBH_PERF_TEST_SERVICE_TYPE
	};

	class CTestSession
	{
	public:
		ACE_UINT32 m_nUserAChannelID;
		CTestTradeUser m_testUserA;
		CTestTradeUser m_testUserB;
		CDscString m_cryptUserAKey;
	};

public:
	CBcPerfTestService();

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);
	void OnTimer(void);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH_TEST::CTestUserInfoMsg)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH_TEST::CTestUserInfoMsg& rTestUserInfoMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

protected:
	virtual void OnReady(void);
	virtual void OnAbnormal(void);
	virtual void OnGetBcUserInfoRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, DSC::CDscBlob& userInfo);
	virtual void OnLaunchBcTransRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, DSC::CDscBlob& transKey);
	virtual void OnGetBcTransInfoRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, DSC::CDscBlob& transKey, DSC::CDscBlob& transInfo);

private:
	void DoTrade(CTestSession& testSession);

private:
	bool m_bReady;
	ACE_UINT32 m_nGetSum = 0;
	ACE_INT32 m_nSetSum = 0;

private:
	using test_map_type = dsc_unordered_map_type(ACE_UINT64, CTestSession);
	test_map_type m_mapUserInfo;
	CTestSession m_testSession;
};

#endif
