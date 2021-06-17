#include "vbh_perf_test/perf_test_client_service.h"
#include "vbh_perf_test/perf_test_client_service_factory.h"

CPerfTestServiceFactory::CPerfTestServiceFactory()
: m_nServiceID(0)
{
}

CDscService* CPerfTestServiceFactory::CreateDscService(void)
{ 
	CBcPerfTestService* pService = NULL;

	DSC_NEW(pService, CBcPerfTestService);

	if(pService)
	{ 
		pService->SetType(CBcPerfTestService::EN_SERVICE_TYPE);
		pService->SetID(++m_nServiceID);
	} 

	return pService;
} 