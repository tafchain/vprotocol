#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <time.h>

#include "vbh_ca_client_sdk/vbh_ca_client_api.h"
#include "vbh_client_sdk/vbh_client_api.h"
#include "vbh_test_comm/vbh_test_trade_def.h"
#include "vbh_sdk_comm/vbh_sdk_comm_if.h"
#include "bc/vbh_cc/cc_comm/cc_comm_id_def.h"
#include "common/vbh_comm_error_code.h"

class CTestRegUserMsgCallback : public VBH_SDK::IRegUserMsgCallback
{
public:
	virtual void OnReady(void);
	virtual void OnAbnormal(void) {};
	virtual void OnExit(void) {};

public:
	virtual void OnRegistUserRsp(VBH_SDK::CRegistUserRsp& rRegistUserRsp);
	virtual void OnUnRegistUserRsp(VBH_SDK::CUnRegistUserRsp& rUnRegistUserRsp);
};

class CTestOpUserMsgCallback : public VBH_SDK::IBcUserInfoMsgCallback
{
public:
	virtual void OnReady(void);
	virtual void OnAbnormal(void);
	virtual void OnExit(void);

public:
	virtual void OnGetBcUserInfoRsp(VBH_SDK::CGetBcUserInfoRsp& rGetUserInfoRsp);
	virtual void OnLaunchBcTransRsp(VBH_SDK::CSetBcUserInfoRsp& rSetUserInfoRsp);
	virtual void OnGetBcTransInfoRsp(VBH_SDK::CGetBcTransactionInfoRsp& rGetTransInfoRsp);
};

ACE_UINT64 g_nBaseIDNumber;
ACE_UINT32 g_nUserNum;
ACE_UINT32 g_nRegistRspNum = 0;

CDscString* g_userKey;
CDscString* g_cryptUserKey;
unsigned int g_msgNum = 0;
bool g_bExit = false;

int main(int argc, char* argv[])
{
	int testID;
	if (argc < 3)
	{
		printf("no base num and user num\n");
	}

	testID = atoi(argv[1]);

	g_nBaseIDNumber = testID;
	g_nBaseIDNumber = g_nBaseIDNumber << 16;

	g_nUserNum = atoi(argv[2]) * 2;
	g_cryptUserKey = new CDscString[g_nUserNum];
	g_userKey = new CDscString[g_nUserNum];

	CTestRegUserMsgCallback testRegUserMsgMsgCallback;
	CTestOpUserMsgCallback testOpUserInfoProcCallback;

	VBH_SDK::InitBcCaClientSdk(testID, &testRegUserMsgMsgCallback);
	VBH_SDK::InitBcUserInfoSdk(testID, &testOpUserInfoProcCallback);

	for (;;)
	{
		printf("msg num:%d\n", g_msgNum);
		g_msgNum = 0;
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif

		if (g_bExit)
		{
			break;
		}
	}

	delete[] g_cryptUserKey;

	return 0;
}

void CTestRegUserMsgCallback::OnReady(void)
{
	CTestTradeUserKey userKey;
	VBH_SDK::CUnRegistUserReq req;

	for (uint32_t i = 0; i < g_nUserNum; ++i)
	{
		userKey.IDNumber = g_nBaseIDNumber + i;
		req.m_nChannelID = CTestTradeUserKey::EN_CHANNEL_ID;
		DEF_ENCODE(userKey, req.m_pUserKey, req.m_nUserKeyLen);

		if (VBH_SDK::UnRegBcUser(req))
		{
			printf("un regist user req failed\n");
		}
	}
}

void CTestRegUserMsgCallback::OnUnRegistUserRsp(VBH_SDK::CUnRegistUserRsp& rUnRegistUserRsp)
{
	CTestTradeUserKey userKey;
	CTestTradeUserInfo userInfo;

	if (DSC::Decode(userKey, rUnRegistUserRsp.m_pUserKey, rUnRegistUserRsp.m_nUserKeyLen))
	{
		printf("OnUnRegistUserRsp(), decode user key error\n");
	}
	else
	{
		VBH_SDK::CRegistUserReq req;

		req.m_nChannelID = CTestTradeUserKey::EN_CHANNEL_ID;

		req.m_pUserKey = rUnRegistUserRsp.m_pUserKey;
		req.m_nUserKeyLen = rUnRegistUserRsp.m_nUserKeyLen;

		userInfo.BTC = 0X7FFFFFFF;
		userInfo.ETH = 0X7FFFFFFF;

		DEF_ENCODE(userInfo, req.m_pUserUserInfo, req.m_nUserUserInfoLen);

		if (VBH_SDK::RegBcUser(req))
		{
			printf("regist user req failed\n");
		}
	}
}

void DoTrade(char* pUserKey, size_t nUserKeyLen, char* pCryptUserKey, size_t nCryptUserKeyLen)
{
	CTestTradeUserKey userKey;

	if (DSC::Decode(userKey, pUserKey, nUserKeyLen))
	{
		printf("OnRegistUserRsp(), decode user key error\n");
	}
	else
	{
		VBH_SDK::CLaunchBcTransReq req;

		req.m_nChannelID = CTestTradeUserKey::EN_CHANNEL_ID;
		req.m_nCcID = CC::EN_CC_DEMO_SERVICE_TYPE;
		req.m_nAction = CTestDemoTrade::EN_ACTION_ID;
		req.m_pUserKey = pUserKey;
		req.m_nUserKeyLen = nUserKeyLen;

		req.m_pCryptUserKey = pCryptUserKey;
		req.m_nCryptUserKeyLen = nCryptUserKeyLen;

		CTestDemoTrade trade;

		trade.userKeyA.IDNumber = userKey.IDNumber;
		trade.userKeyB.IDNumber = userKey.IDNumber + 1;
		trade.coinType = CTestDemoTrade::EN_BTC;
		trade.coinNum = 1;

		DEF_ENCODE(trade, req.m_pProposal, req.m_nProposalLen);

		if (VBH_SDK::LaunchBcTrans(req))
		{
			printf("set user req failed\n");
		}
	}
}

void CTestRegUserMsgCallback::OnRegistUserRsp(VBH_SDK::CRegistUserRsp& rRegistUserRsp)
{
	if (rRegistUserRsp.m_nReturnCode)
	{
		printf("OnRegistUserRsp() report error:%d\n", rRegistUserRsp.m_nReturnCode);
	}
	else
	{
		++g_nRegistRspNum;

		CTestTradeUserKey userKey;

		if (DSC::Decode(userKey, rRegistUserRsp.m_pUserKey, rRegistUserRsp.m_nUserKeyLen))
		{
			printf("OnRegistUserRsp(), decode user key error\n");
		}
		else
		{
			g_userKey[userKey.IDNumber - g_nBaseIDNumber].assign(rRegistUserRsp.m_pUserKey, rRegistUserRsp.m_nUserKeyLen);
			g_cryptUserKey[userKey.IDNumber - g_nBaseIDNumber].assign(rRegistUserRsp.m_pCryptUserKey, rRegistUserRsp.m_nCryptUserKeyLen);

			if (g_nRegistRspNum == g_nUserNum)
			{
				const ACE_UINT32 n = g_nUserNum;
				for (ACE_UINT32 i = 0; i < n; i += 2)
				{
					DoTrade(g_userKey[i].data(), g_userKey[i].size(), g_cryptUserKey[i].data(), g_cryptUserKey[i].size());
				}
			}
		}
	}
}

void CTestOpUserMsgCallback::OnGetBcUserInfoRsp(VBH_SDK::CGetBcUserInfoRsp& rGetUserInfoRsp)
{
	//if (rGetUserInfoRsp.m_nReturnCode)
	//{
	//	printf("OnGetBcUserInfoRsp() report error\n");
	//}
	//else
	//{
	//	CTestTradeUserKey userKey;

	//	if (DSC::Decode(userKey, rGetUserInfoRsp.m_pUserKey, rGetUserInfoRsp.m_nUserKeyLen))
	//	{
	//		printf("OnRegistUserRsp(), decode user key error\n");
	//	}
	//	else
	//	{
	//		VBH_SDK::CSetBcUserInfoReq req;

	//		CDscString& strCrypt = g_cryptUserKey[userKey.m_nIDNumber - g_nBaseIDNumber];
	//		req.m_nChannelID = CTestTradeUserKey::EN_CHANNEL_ID;
	//		req.m_nCcID = CC::EN_CC_DEMO_SERVICE_TYPE;
	//		req.m_pUserKey = rGetUserInfoRsp.m_pUserKey;
	//		req.m_nUserKeyLen = rGetUserInfoRsp.m_nUserKeyLen;

	//		req.m_pCryptUserKey = strCrypt.data();
	//		req.m_nCryptUserKeyLen = strCrypt.size();

	//		CTestTradeUserInfo userInfo;

	//		userInfo.m_sex = 1;
	//		userInfo.m_nAge = 54;
	//		userInfo.m_strUserName = "张三";
	//		userInfo.m_strAddress = "阳关道";
	//		userInfo.n_nPhoneNumber = userKey.m_nIDNumber;

	//		DEF_ENCODE(userInfo, req.m_pProposal, req.m_nProposalLen);
	//		++g_msgNum;
	//		if (VBH_SDK::SetBcUserInfo(req))
	//		{
	//			printf("set user req failed\n");
	//		}
	//	}
	//}
}

void CTestOpUserMsgCallback::OnLaunchBcTransRsp(VBH_SDK::CSetBcUserInfoRsp& rSetUserInfoRsp)
{
	if (rSetUserInfoRsp.m_nReturnCode)
	{
		printf("OnSetBcUserInfoRsp() report error:%d\n", rSetUserInfoRsp.m_nReturnCode);
	}
	else
	{
	}

	CTestTradeUserKey userKey;

	if (DSC::Decode(userKey, rSetUserInfoRsp.m_pUserKey, rSetUserInfoRsp.m_nUserKeyLen))
	{
		printf("OnRegistUserRsp(), decode user key error\n");
	}
	else
	{
		CDscString& strCrypt = g_cryptUserKey[userKey.IDNumber - g_nBaseIDNumber];

		DoTrade(rSetUserInfoRsp.m_pUserKey, rSetUserInfoRsp.m_nUserKeyLen, strCrypt.data(), strCrypt.size());

		++g_msgNum;
	}
}

void CTestOpUserMsgCallback::OnGetBcTransInfoRsp(VBH_SDK::CGetBcTransactionInfoRsp& rGetTransInfoRsp)
{
	//printf("get transaction succeed.\n");
}

void CTestOpUserMsgCallback::OnAbnormal(void)
{
	printf("CTestRegUserMsgCallback::OnAbnormal(void)\n");
}

void CTestOpUserMsgCallback::OnReady(void)
{
}


void CTestOpUserMsgCallback::OnExit(void)
{
	g_bExit = true;
}
