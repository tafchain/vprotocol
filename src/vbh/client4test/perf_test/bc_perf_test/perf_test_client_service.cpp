#include "vbh_perf_test/perf_test_client_service.h"
#include "vbh_cc/cc_comm/cc_comm_id_def.h"

CBcPerfTestService::CBcPerfTestService()
{
}

ACE_INT32 CBcPerfTestService::OnInit(void)
{
	if(CBcClientService::OnInit())
	{
		DSC_RUN_LOG_ERROR("bc perf test service init failed!");

		return -1;
	}

	this->SetDscTimer(this, 1, true);
	m_testSession.m_testUserA.userKey.IDNumber = 0;
	m_testSession.m_testUserB.userKey.IDNumber = 0;

	return 0;
}

ACE_INT32 CBcPerfTestService::OnExit( void )
{
	this->CancelDscTimer(this);

	return CBcClientService::OnExit();
}

void CBcPerfTestService::OnTimer(void)
{
	VBH_TEST::CSumMsg sumMsg;
	CDscMsg::CDscMsgAddr addr(VBH_TEST::VBH_TEST_STATIC_NODE_TYPE, 1,
		VBH_TEST::VBH_PERF_TEST_STATIC_SERVICE_TYPE, 1);

	//ACE_OS::printf("msg sum:%d!\n", m_nGetSum);

	sumMsg.m_nGetSum = m_nGetSum;
	if (this->SendDscMessage(sumMsg))
	{
		DSC_RUN_LOG_INFO("send static msg failed");
	}
	m_nGetSum = 0;
}

void CBcPerfTestService::OnReady(void)
{
}

void CBcPerfTestService::OnAbnormal(void)
{

}

void CBcPerfTestService::OnGetBcUserInfoRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, DSC::CDscBlob& userInfo)
{
}

void CBcPerfTestService::OnLaunchBcTransRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, DSC::CDscBlob& transKey)
{
	if (nReturnCode)
	{
		DSC_RUN_LOG_ERROR("set user failed, error code:%d", nReturnCode);
	}

	CTestTradeUserKey testTradeUserKey;

	if (DSC::Decode(testTradeUserKey, userKey.m_key.GetBuffer(), userKey.m_key.GetSize()))
	{
		DSC_RUN_LOG_ERROR("dcode error");
	}
	else
	{
		auto it = m_mapUserInfo.find(testTradeUserKey.IDNumber);
		if (it == m_mapUserInfo.end())
		{
			DSC_RUN_LOG_ERROR("cann't find user:%lld", testTradeUserKey.IDNumber);
		}
		else
		{
			CTestSession& testSession = it->second;

			if (CDscAppManager::m_nNodeID > 10)
			{
				this->DoTrade(testSession);
				++m_nGetSum;
			}
			else
			{
				VBH::CQueryBcTransInfoApiCltReq req;

				req.m_userKey = userKey;
				req.m_transKey = transKey;
				req.m_cryptUserKey.Set(testSession.m_cryptUserAKey.data(), testSession.m_cryptUserAKey.size());

				this->GetBcTransInfo(req);
			}
		}
	}
}

void CBcPerfTestService::OnDscMsg(VBH_TEST::CTestUserInfoMsg& rTestUserInfoMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	if (m_testSession.m_testUserA.userKey.IDNumber)
	{
		m_testSession.m_testUserB = rTestUserInfoMsg.m_testBcUser;
		m_mapUserInfo.insert(test_map_type::value_type(m_testSession.m_testUserA.userKey.IDNumber, m_testSession));

		this->DoTrade(m_testSession);
		m_testSession.m_testUserA.userKey.IDNumber = 0;
		m_testSession.m_testUserB.userKey.IDNumber = 0;
	}
	else
	{
		m_testSession.m_nUserAChannelID = CTestTradeUserKey::EN_CHANNEL_ID;
		m_testSession.m_testUserA = rTestUserInfoMsg.m_testBcUser;
		m_testSession.m_cryptUserAKey.assign(rTestUserInfoMsg.m_cryptUserKey.GetBuffer(), rTestUserInfoMsg.m_cryptUserKey.GetSize());
	}
}

void CBcPerfTestService::OnGetBcTransInfoRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, DSC::CDscBlob& transKey, DSC::CDscBlob& transInfo)
{
	if (nReturnCode)
	{
		DSC_RUN_LOG_ERROR("set user failed, error code:%d", nReturnCode);
	}

	CTestTradeUserKey testTradeUserKey;

	if (DSC::Decode(testTradeUserKey, userKey.m_key.GetBuffer(), userKey.m_key.GetSize()))
	{
		DSC_RUN_LOG_ERROR("dcode error");
	}
	else
	{
		auto it = m_mapUserInfo.find(testTradeUserKey.IDNumber);
		if (it == m_mapUserInfo.end())
		{
			DSC_RUN_LOG_ERROR("cann't find user:%lld", testTradeUserKey.IDNumber);
		}
		else
		{
			CTestSession& testSession = it->second;
			VBH::CQueryBcTransInfoApiCltReq req;

			req.m_userKey = userKey;
			req.m_transKey = transKey;
			req.m_cryptUserKey.Set(testSession.m_cryptUserAKey.data(), testSession.m_cryptUserAKey.size());

			this->GetBcTransInfo(req);
			++m_nGetSum;
		}
	}
}

void CBcPerfTestService::DoTrade(CTestSession& testSession)
{
	CTestDemoTrade testDemoTrade;
	VBH::CLaunchBcTransApiCltReq req;
	char* pKeyMsgBuf;
	char* pProposalBuf;
	size_t nKeyMsgBufLen;
	size_t nProposalBufLen;
	ACE_INT32 nReturnCode;

	testDemoTrade.userKeyA.IDNumber = testSession.m_testUserA.userKey.IDNumber;
	testDemoTrade.userKeyB.IDNumber = testSession.m_testUserB.userKey.IDNumber;
	testDemoTrade.coinType = CTestDemoTrade::EN_BTC;
	testDemoTrade.coinNum = 1;

	DSC::Encode(testSession.m_testUserA.userKey, pKeyMsgBuf, nKeyMsgBufLen);
	DSC::Encode(testDemoTrade, pProposalBuf, nProposalBufLen);

	req.m_nCcID = CC::EN_CC_DEMO_SERVICE_TYPE;
	req.m_nAction = CTestDemoTrade::EN_ACTION_ID;
	req.m_userKey.m_nChannelID = CTestTradeUserKey::EN_CHANNEL_ID;
	req.m_userKey.m_key.Set(pKeyMsgBuf, nKeyMsgBufLen);
	req.m_cryptUserKey.Set(testSession.m_cryptUserAKey.data(), testSession.m_cryptUserAKey.size());
	req.m_proposal.Set(pProposalBuf, nProposalBufLen);

	nReturnCode = this->LaunchBcTrans(req);
	if (nReturnCode)
	{
		DSC_RUN_LOG_ERROR("perf trade test failed, error code:%d\n", nReturnCode);
	}

	DSC_THREAD_SIZE_FREE(pKeyMsgBuf, nKeyMsgBufLen);
	DSC_THREAD_SIZE_FREE(pProposalBuf, nProposalBufLen);
}