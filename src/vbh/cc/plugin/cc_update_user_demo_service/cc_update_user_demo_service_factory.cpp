#include "cc_update_user_demo_service/cc_update_user_demo_service.h"
#include "cc_update_user_demo_service/cc_update_user_demo_service_factory.h"

CDscService* CCcUpdateUserDemoServiceFactory::CreateDscService(void)
{ 
	CCcUpdateUserDemoService* pCcDemoService = NULL;

	DSC_NEW(pCcDemoService, CCcUpdateUserDemoService);

	if(pCcDemoService)
	{ 
		pCcDemoService->SetType(CCcUpdateUserDemoService::EN_SERVICE_TYPE);
		pCcDemoService->SetID(++m_nServiceID);
	} 

	return pCcDemoService;
}
