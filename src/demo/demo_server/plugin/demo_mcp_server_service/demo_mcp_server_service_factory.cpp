#include "demo_mcp_server_service.h"
#include "demo_mcp_server_service_factory.h"

CDemoMcpServerServiceFactory::CDemoMcpServerServiceFactory()
: m_nServiceID(0)
{
}

CDscService* CDemoMcpServerServiceFactory::CreateDscService(void)
{ 
	CDemoMcpServerService* pDemoMcpServerService = NULL;

	DSC_NEW(pDemoMcpServerService, CDemoMcpServerService);

	if(pDemoMcpServerService)
	{ 
		pDemoMcpServerService->SetType(CDemoMcpServerService::EN_SERVICE_TYPE);
		pDemoMcpServerService->SetID(++m_nServiceID);
	} 

	return pDemoMcpServerService;
} 