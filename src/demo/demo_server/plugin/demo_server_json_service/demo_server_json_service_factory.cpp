#include "demo_server_json_service.h"
#include "demo_server_json_service_factory.h"

CDemoServerJsonFactory::CDemoServerJsonFactory()
: m_nServiceID(0)
{
}

CDscNestServerHttpService* CDemoServerJsonFactory::CreateDscNestServerHttpService(void)
{ 
	CDemoServerJsonService* pDemoServerJsonService = nullptr;

	DSC_NEW(pDemoServerJsonService, CDemoServerJsonService);

	if(pDemoServerJsonService)
	{ 
		pDemoServerJsonService->SetType(CDemoServerJsonService::EN_SERVICE_TYPE);
		pDemoServerJsonService->SetID(++m_nServiceID);
	} 

	return pDemoServerJsonService;
} 