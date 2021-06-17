#include "demo_server_service.h"
#include "demo_server_service_factory.h"

CDemoServerServiceFactory::CDemoServerServiceFactory()
: m_nServiceID(0)
{
}

CDscService* CDemoServerServiceFactory::CreateDscService(void)
{ 
	//CDemoServerService* pDemoServerService = NULL;

	//DSC_NEW(pDemoServerService, CDemoServerService);

	//if(pDemoServerService)
	//{ 
	//	pDemoServerService->SetType(CDemoServerService::EN_SERVICE_TYPE);
	//	pDemoServerService->SetID(++m_nServiceID);
	//} 

	//return pDemoServerService;
	return NULL;
} 