#include "cc_nodejs_demo_service/cc_nodejs_demo_service.h"
#include "cc_nodejs_demo_service/cc_nodejs_demo_service_factory.h"

CDscService* CCcNodeJsDemoServiceFactory::CreateDscService(void)
{ 
	CCcNodeJsDemoService* pCcDemoService = NULL;

	DSC_NEW(pCcDemoService, CCcNodeJsDemoService);

	if(pCcDemoService)
	{ 
		pCcDemoService->SetType(CCcNodeJsDemoService::EN_SERVICE_TYPE);
		pCcDemoService->SetID(++m_nServiceID);
	} 

	return pCcDemoService;
}
