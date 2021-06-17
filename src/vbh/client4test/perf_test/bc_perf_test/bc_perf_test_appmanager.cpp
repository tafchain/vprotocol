#include "dsc/dsc_comm_func.h"

#include "vbh_perf_test/vbh_perf_test_appmanager.h"
#include "vbh_perf_test/reg_test_client_service_plugin.h"
#include "vbh_perf_test/perf_test_client_service_plugin.h"

ACE_INT32 CBcPerfTestAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

	CPerfTestServicePlugIn perfTestServicePlugIn;
	CRegTestServicePlugIn regTestServicePlugIn;

	perfTestServicePlugIn.OnInit();
	regTestServicePlugIn.OnInit();

	return 0;
}

void CBcPerfTestAppManager::OnParam(const int argc, ACE_TCHAR* argv)
{
	if (argc == 'u' || argc == 'U')
	{
		DSC::DscAtoi(argv, m_nTestUserNum);
	}
}

ACE_UINT32 CBcPerfTestAppManager::GetTestUserNum(void) const
{
	return m_nTestUserNum;
}
