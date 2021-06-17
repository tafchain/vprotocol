#include "demo_client_json_service.h"
#include "demo_client_json_service_factory.h"

CDemoClientJsonFactory::CDemoClientJsonFactory()
: m_nServiceID(0)
{
}

CDscService* CDemoClientJsonFactory::CreateDscService(void)
{ 
	CDemoClientJsonService* pDemoClientJsonService = nullptr;

	DSC_NEW(pDemoClientJsonService, CDemoClientJsonService);

	if(pDemoClientJsonService)
	{ 
		pDemoClientJsonService->SetType(CDemoClientJsonService::EN_SERVICE_TYPE);
		pDemoClientJsonService->SetID(++m_nServiceID);
	} 

	return pDemoClientJsonService;
} 