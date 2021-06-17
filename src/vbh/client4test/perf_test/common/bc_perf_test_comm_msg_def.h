#ifndef COMM_MSG_DEF_H_4328792347624323468842382346
#define COMM_MSG_DEF_H_4328792347624323468842382346

#include "dsc/codec/dsc_codec/dsc_codec.h"

#include "common/vbh_comm_msg_def.h"
#include "vbh_perf_test_comm/vbh_perf_test_comm_id_def.h"
#include "vbh_test/vbh_test_comm/vbh_test_trade_def.h"

namespace VBH_TEST
{
	class CSumMsg
	{
	public:
		enum
		{
			EN_MSG_ID = VBH_TEST::EN_SUM_MSG
		};

	public:
		DSC_BIND_ATTR(m_nGetSum);

	public:
		ACE_UINT32 m_nGetSum;
	};

	class CTestUserInfoMsg
	{
	public:
		enum
		{
			EN_MSG_ID = VBH_TEST::EN_USER_INFO_MSG
		};

	public:
		DSC_BIND_ATTR(m_nChannelID, m_testBcUser, m_cryptUserKey);

	public:
		ACE_UINT32 m_nChannelID;
		CTestTradeUser m_testBcUser;
		DSC::CDscBlob m_cryptUserKey;
	};
}

#endif