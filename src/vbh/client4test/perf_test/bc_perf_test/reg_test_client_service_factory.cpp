#include "vbh_perf_test/reg_test_client_service.h"
#include "vbh_perf_test/reg_test_client_service_factory.h"

CRegTestServiceFactory::CRegTestServiceFactory()
: m_nServiceID(0)
{
}

CDscService* CRegTestServiceFactory::CreateDscService(void)
{ 
	CBcRegTestService* pService = NULL;

	DSC_NEW(pService, CBcRegTestService);

	if(pService)
	{ 
		pService->SetType(CBcRegTestService::EN_SERVICE_TYPE);
		pService->SetID(++m_nServiceID);
	} 

	return pService;
} 