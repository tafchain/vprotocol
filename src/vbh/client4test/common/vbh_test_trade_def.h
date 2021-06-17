#ifndef VBH_TEST_TRADE_DEF_H_97823484132652134
#define VBH_TEST_TRADE_DEF_H_97823484132652134

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/codec/json_codec/json_codec_bind_macros.h"
#include "vbh_test_comm/vbh_test_comm_def.h"

class CTestTradeUserKey
{
public:
	enum
	{
		EN_CHANNEL_ID = VBH_TEST::EN_TEST_TRADE_CHANNEL_ID
	};

public:
	bool operator== (const CTestTradeUserKey& rTestUserKey) const
	{
		return IDNumber == rTestUserKey.IDNumber;
	}

public:
	DSC_BIND_ATTR(IDNumber)
	JSON_BIND_ATTR1(IDNumber)

public:
	ACE_UINT64 IDNumber = 0;
};

class CTestTradeUserInfo
{
public:
	DSC_BIND_ATTR(BTC, ETH)
	JSON_BIND_ATTR2(BTC, ETH)

public:
	ACE_UINT32 BTC = 0;
	ACE_UINT32 ETH = 0;
};

class CTestTradeUser
{
public:
	DSC_BIND_ATTR(userKey, userInfo)
	JSON_BIND_ATTR2(userKey, userInfo)

public:
	CTestTradeUserKey userKey;
	CTestTradeUserInfo userInfo;
};

//用户A转币种n_nCoinType的币n_nCoinNum
class CTestDemoTrade
{
public:
	enum
	{
		EN_BTC = 1,
		EN_ETH = 2,
		EN_ACTION_ID = VBH_TEST::EN_TEST_TRADE_ACTION_ID
	};

public:
	DSC_BIND_ATTR(userKeyA, userKeyB, coinType, coinNum)
	JSON_BIND_ATTR4(userKeyA, userKeyB, coinType, coinNum)

public:
	CTestTradeUserKey userKeyA;
	CTestTradeUserKey userKeyB;
	ACE_UINT32 coinType;
	ACE_UINT32 coinNum;
};

#endif
