#include "cc_json_demo_service/cc_json_demo_service.h"
#include "cc_json_demo_service/cc_json_demo_service_factory.h"

CDscService* CCcJsonDemoServiceFactory::CreateDscService(void)
{ 
	CCcJsonDemoService* pCcDemoService = NULL;

	DSC_NEW(pCcDemoService, CCcJsonDemoService);

	if(pCcDemoService)
	{ 
		pCcDemoService->SetType(CCcJsonDemoService::EN_SERVICE_TYPE);
		pCcDemoService->SetID(++m_nServiceID);
	} 

	return pCcDemoService;
}
