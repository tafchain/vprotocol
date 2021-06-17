#include "demo_mcp_client_service.h"
#include "demo_mcp_client_service_factory.h"

CPerfTestServiceFactory::CPerfTestServiceFactory()
: m_nServiceID(0)
{
}

CDscService* CPerfTestServiceFactory::CreateDscService(void)
{ 
	CDemoMcpClientService* pDemoClientService = NULL;

	DSC_NEW(pDemoClientService, CDemoMcpClientService);

	if(pDemoClientService)
	{ 
		pDemoClientService->SetType(CDemoMcpClientService::EN_SERVICE_TYPE);
		pDemoClientService->SetID(++m_nServiceID);
	} 

	return pDemoClientService;
} 