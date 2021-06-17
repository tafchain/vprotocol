#include "cc_demo_service/cc_demo_service.h"
#include "common/vbh_comm_error_code.h"

CCcDemoService::CCcDemoService()
{
}

CCcDemoService::CSession::CSession(CCcDemoService& rCcDemoService)
: m_rCcService(rCcDemoService)
{

}

ACE_INT32 CCcDemoService::OnInit(void)
{
	if(CCcObjService::OnInit())
	{
		DSC_RUN_LOG_ERROR("cc demo service init failed!");

		return -1;
	}

	DSC_RUN_LOG_INFO("cc demo service init succeed!");

	return 0;
}

ACE_INT32 CCcDemoService::OnExit(void)
{
	return CCcObjService::OnExit();
}

ACE_INT32 CCcDemoService::OnPorposal(const ACE_UINT32 nSessionID, CTestDemoTrade& rTestDemoTrade)
{	
	CSession* pSession = DSC_THREAD_TYPE_NEW(CSession) CSession(*this);

	pSession->m_demoTrade = rTestDemoTrade;
	pSession->m_userA.userKey = rTestDemoTrade.userKeyA;
	pSession->m_userB.userKey = rTestDemoTrade.userKeyB;

	if (m_mapSession.Insert(nSessionID, pSession))
	{
		DSC_RUN_LOG_INFO("insert session to map failed, session:%d", nSessionID);
		DSC_THREAD_TYPE_DEALLOCATE(pSession);

		return - 1;
	}
	else
	{
		this->SetDscTimer(pSession, EN_SESSION_TIMEOUT_VALUE);
		DSC_FORWARD_CALL(this->GetVbhUser(nSessionID, CTestTradeUserKey::EN_CHANNEL_ID, rTestDemoTrade.userKeyA));
		DSC_FORWARD_CALL(this->GetVbhUser(nSessionID, CTestTradeUserKey::EN_CHANNEL_ID, rTestDemoTrade.userKeyB));
	}

	return 0;
}

void CCcDemoService::OnGetVbhUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT32 nSessionID, CSimpleUser& rUser)
{
	CSession* pSession = m_mapSession.Find(nSessionID);

	if (pSession)
	{
		if(nReturnCode == VBH::EN_OK_TYPE)
		{
			CTestTradeUser user;

			if (this->DecodeUser(user.userKey, user.userInfo, rUser))
			{
				this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed, decode erorr"));

				this->CancelDscTimer(pSession);
				m_mapSession.Erase(nSessionID);
				DSC_THREAD_TYPE_DEALLOCATE(pSession);

				return -1;
			}
			else
			{
				if (pSession->m_userA.userKey == user.userKey)
				{
					++pSession->m_nRspNum;
					pSession->m_userA.userInfo = user.userInfo;
				}
				else if (pSession->m_userB.userKey == user.userKey)
				{
					++pSession->m_nRspNum;
					pSession->m_userB.userInfo = user.userInfo;
				}
				else
				{
					DSC_RUN_LOG_INFO("user key error");
					this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed, decode erorr"));

					this->CancelDscTimer(pSession);
					m_mapSession.Erase(nSessionID);
					DSC_THREAD_TYPE_DEALLOCATE(pSession);

					return -1;
				}

				if (2 == pSession->m_nRspNum)
				{
					switch (pSession->m_demoTrade.coinType)
					{
					case CTestDemoTrade::EN_BTC:
					{
						pSession->m_userA.userInfo.BTC -= pSession->m_demoTrade.coinNum;
						pSession->m_userB.userInfo.BTC += pSession->m_demoTrade.coinNum;
					}
					break;
					case CTestDemoTrade::EN_ETH:
					{
						pSession->m_userA.userInfo.ETH -= pSession->m_demoTrade.coinNum;
						pSession->m_userB.userInfo.ETH += pSession->m_demoTrade.coinNum;
					}
					break;
					default:
					{
						DSC_RUN_LOG_INFO("coin type error:%d", pSession->m_demoTrade.coinType);
						this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed, coin type erorr"));

						this->CancelDscTimer(pSession);
						m_mapSession.Erase(nSessionID);
						DSC_THREAD_TYPE_DEALLOCATE(pSession);

						return -1;
					}
					}

					this->SetVbhUser(nSessionID, CTestTradeUserKey::EN_CHANNEL_ID, pSession->m_userA.userKey, pSession->m_userA.userInfo);
					this->SetVbhUser(nSessionID, CTestTradeUserKey::EN_CHANNEL_ID, pSession->m_userB.userKey, pSession->m_userB.userInfo);

					this->PorposalRsp(VBH::EN_OK_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade succeed"));
					this->CancelDscTimer(pSession);
					m_mapSession.Erase(nSessionID);
					DSC_THREAD_TYPE_DEALLOCATE(pSession);
				}
				else
				{
					//¼ÌÐøµÈ
				}
			}
		}
		else
		{
			this->PorposalRsp(VBH::EN_CC_REPORT_ERROR_TYPE, nSessionID, DSC_STRING_TYPE_PARAM("trade failed"));

			this->CancelDscTimer(pSession);
			m_mapSession.Erase(nSessionID);
			DSC_THREAD_TYPE_DEALLOCATE(pSession);

			return -1;
		}
	}
	else
	{
		DSC_RUN_LOG_INFO("cann't find session:%d", nSessionID);

		return -1;
	}

	return 0;
}

void CCcDemoService::OnTimeOut(CSession* pUserSession)
{
	DSC_RUN_LOG_INFO("cc timeout, session id:%d", pUserSession->m_nKey);
	m_mapSession.Erase(pUserSession);
	DSC_THREAD_TYPE_DEALLOCATE(pUserSession);
}
