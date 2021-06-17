#include "cc_update_user_json_demo_service/cc_update_user_json_demo_service.h"
#include "cc_update_user_json_demo_service/cc_update_user_json_demo_service_factory.h"

CDscService* CCcUpdateUserJsonDemoServiceFactory::CreateDscService(void)
{ 
	CCcUpdateUserJsonDemoService* pCcDemoService = NULL;

	DSC_NEW(pCcDemoService, CCcUpdateUserJsonDemoService);

	if(pCcDemoService)
	{ 
		pCcDemoService->SetType(CCcUpdateUserJsonDemoService::EN_SERVICE_TYPE);
		pCcDemoService->SetID(++m_nServiceID);
	} 

	return pCcDemoService;
}
