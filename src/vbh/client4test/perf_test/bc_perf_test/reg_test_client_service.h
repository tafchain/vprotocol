#ifndef PERF_TEST_CLIENT_SERVICE_H_243782347832156213543125
#define PERF_TEST_CLIENT_SERVICE_H_243782347832156213543125

#include "dsc/mem_mng/dsc_stl_type.h"

#include "vbh_sdk_comm/vbh_ca_client_service.h"
#include "vbh_perf_test_comm/vbh_perf_test_comm_msg_def.h"
#include "vbh_test_comm/vbh_test_trade_def.h"

class CBcRegTestService : public CBcCaClientService
{
public:
	enum
	{
		EN_SERVICE_TYPE = VBH_TEST::VBH_REG_TEST_SERVICE_TYPE
	};

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);

protected:
	virtual void OnReady(void);
	virtual void OnAbnormal(void);
	virtual void OnRegistUserRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, CDscString& cryptUserKey);
	virtual void OnUnRegistUserRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey);

private:
	bool m_bReady;
	ACE_UINT64 m_nGetSum = 0;
	ACE_INT32 m_nSetSum = 0;
	ACE_INT32 m_nRegSum = 0;

private:
	using test_map_type = dsc_unordered_map_type(ACE_UINT64, CTestTradeUserInfo);
	test_map_type m_mapUserInfo;
};

#endif
