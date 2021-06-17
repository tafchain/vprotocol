#include "cc_demo_service/cc_demo_service.h"
#include "cc_demo_service/cc_demo_service_factory.h"

CDscService* CCcDemoServiceFactory::CreateDscService(void)
{ 
	CCcDemoService* pCcDemoService = NULL;

	DSC_NEW(pCcDemoService, CCcDemoService);

	if(pCcDemoService)
	{ 
		pCcDemoService->SetType(CCcDemoService::EN_SERVICE_TYPE);
		pCcDemoService->SetID(++m_nServiceID);
	} 

	return pCcDemoService;
}
