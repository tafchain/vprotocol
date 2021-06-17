#include "dsc/dsc_log.h"

#include "vbh_test_static/static_service.h"

CBcPerfTestStaticService::CBcPerfTestStaticService()
{
}

ACE_INT32 CBcPerfTestStaticService::OnInit(void)
{
	if (CDscReactor::OnInit())
	{
		DSC_RUN_LOG_ERROR("perf static service init failed!");

		return -1;
	}

	this->SetDscTimer(this, 1, true);

	return 0;
}

ACE_INT32 CBcPerfTestStaticService::OnExit(void)
{
	this->CancelDscTimer(this);

	return CDscReactor::OnExit();
}

void CBcPerfTestStaticService::OnTimer(void)
{
	//ACE_OS::printf("get msg sum:%d, set msg sum:%d, reg msg sum:%d, total sum:%d!\n", m_nGetSum, m_nSetSum, m_nRegSum, m_nGetSum + m_nSetSum + m_nRegSum);
	double dAvg = 0;

	if (m_nSum)
	{
		m_dTotal += m_nSum;
		++m_nCount;
	}

	if (m_nCount)
	{
		dAvg = m_dTotal / m_nCount;
	}

	if (dAvg > 1)
	{
		ACE_OS::printf("cur sec msg sum:%d, avg:%0.1f!\n", m_nSum, dAvg);
	}

	m_nSum = 0;
}

void CBcPerfTestStaticService::OnDscMsg(VBH_TEST::CSumMsg& rGetSum, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	m_nSum += rGetSum.m_nGetSum;
}
