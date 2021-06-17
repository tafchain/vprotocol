#include "common/vbh_comm_error_code.h"

#include "vbh_perf_test/reg_test_client_service.h"
#include "vbh_perf_test/vbh_perf_test_appmanager.h"

ACE_INT32 CBcRegTestService::OnInit(void)
{
	if(CBcCaClientService::OnInit())
	{
		DSC_RUN_LOG_ERROR("bc perf test service init failed!");

		return -1;
	}

	return 0;
}

ACE_INT32 CBcRegTestService::OnExit( void )
{
	return CBcCaClientService::OnExit();
}

void CBcRegTestService::OnReady(void)
{
	CTestTradeUserKey testUserKey;

	ACE_UINT64 nIDNumber;
	VBH::CUnRegistUserReq req;
	char* pKeyMsgBuf;
	size_t nKeyMsgBufLen;

	nIDNumber = CDscAppManager::Instance()->GetNodeID();
	nIDNumber <<= 16;
	nIDNumber += this->GetID();
	nIDNumber <<= 16;

	CBcPerfTestAppManager* pBcPerfTestAppManager = dynamic_cast<CBcPerfTestAppManager*>(CDscAppManager::Instance());
	if(pBcPerfTestAppManager)
	{
		DSC_RUN_LOG_INFO("pBcPerfTestAppManager->m_nTestUserNum:%d", pBcPerfTestAppManager->m_nTestUserNum);

		for (ACE_UINT32 i = 0; i < pBcPerfTestAppManager->m_nTestUserNum; ++i)
		{
			testUserKey.IDNumber = nIDNumber + i;
			DSC::Encode(testUserKey, pKeyMsgBuf, nKeyMsgBufLen);
			req.m_userKey.m_nChannelID = CTestTradeUserKey::EN_CHANNEL_ID;
			req.m_userKey.m_key.Set(pKeyMsgBuf, nKeyMsgBufLen);

			if (VBH::EN_OK_TYPE != this->UnRegistUser(req))
			{
				DSC_RUN_LOG_WARNING("send unregist user req error");
			}
			DSC_THREAD_SIZE_FREE(pKeyMsgBuf, nKeyMsgBufLen);
		}
	}
	else
	{
		DSC_RUN_LOG_ERROR("dynamic cast to CBcPerfTestAppManager failed");
	}
}

void CBcRegTestService::OnAbnormal(void)
{

}

void CBcRegTestService::OnRegistUserRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey, CDscString& cryptUserKey)
{
	if (nReturnCode == VBH::EN_REPEAT_USER_ERROR_TYPE || nReturnCode == VBH::EN_OK_TYPE)
	{
		CTestTradeUserKey testUserKey;

		if (DSC::Decode(testUserKey, userKey.m_key.GetBuffer(), userKey.m_key.GetSize()))
		{
			DSC_RUN_LOG_ERROR("decode user req error");
		}
		else
		{
			auto it = m_mapUserInfo.find(testUserKey.IDNumber);			
			if (it == m_mapUserInfo.end())
			{
				DSC_RUN_LOG_ERROR("cann't find user, user id:%", testUserKey.IDNumber);
			}
			else
			{
				VBH_TEST::CTestUserInfoMsg msg;
				CDscMsg::CDscMsgAddr addr(CDscAppManager::m_nNodeType, CDscAppManager::m_nNodeID,
					VBH_TEST::VBH_PERF_TEST_SERVICE_TYPE, DSC::EN_INVALID_ID);

				msg.m_nChannelID = userKey.m_nChannelID;
				msg.m_testBcUser.userKey = testUserKey;
				msg.m_testBcUser.userInfo = it->second;
				msg.m_cryptUserKey.Set(cryptUserKey.data(), cryptUserKey.size());

				if (this->SendDscMessage(msg, addr))
				{
					DSC_RUN_LOG_ERROR("send user info to perf test failed, user id:%", testUserKey.IDNumber);
				}
				m_mapUserInfo.erase(it);
			}
		}
	}
	else
	{
		DSC_RUN_LOG_WARNING("reg user error, error id:%d", nReturnCode);
	}
}

void CBcRegTestService::OnUnRegistUserRsp(const ACE_INT32 nReturnCode, VBH::CUserKey& userKey)
{
	CTestTradeUserKey testUserKey;

	if (DSC::Decode(testUserKey, userKey.m_key.GetBuffer(), userKey.m_key.GetSize()))
	{
		DSC_RUN_LOG_ERROR("decode user req error");
	}
	else
	{
		CTestTradeUserInfo userInfo;
		VBH::CRegistUserReq req;
		char* pInfoMsgBuf;
		size_t nInfoMsgBufLen;

		userInfo.BTC = 0X7FFFFFFF;
		userInfo.ETH = 0X7FFFFFFF;

		DEF_ENCODE(userInfo, pInfoMsgBuf, nInfoMsgBufLen);

		req.m_userKey = userKey;
		req.m_userInfo.Set(pInfoMsgBuf, nInfoMsgBufLen);

		if (VBH::EN_OK_TYPE == this->RegistUser(req))
		{
			m_mapUserInfo.insert(test_map_type::value_type(testUserKey.IDNumber, userInfo));
		}
		else
		{
			DSC_RUN_LOG_ERROR("send regist user req error");
		}
	}
}
