#include "demo_client_service.h"
#include "demo_client_service_factory.h"

CDemoClientServiceFactory::CDemoClientServiceFactory()
: m_nServiceID(0)
{
}

CDscService* CDemoClientServiceFactory::CreateDscService(void)
{ 
//	CDemoClientService* pDemoClientService = NULL;
//
//	DSC_NEW(pDemoClientService, CDemoClientService);
//
//	if(pDemoClientService)
//	{ 
//		pDemoClientService->SetType(CDemoClientService::EN_SERVICE_TYPE);
//		pDemoClientService->SetID(++m_nServiceID);
//	} 
//
//	return pDemoClientService;
	return NULL;
} 