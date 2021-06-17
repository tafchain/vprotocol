#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "vbh_test_static/static_service.h"
#include "vbh_test_static/static_service_plugin.h"

ACE_INT32 CStaticServicePlugin::OnInit(void)
{ 
	CBcPerfTestStaticService* pService;

	DSC_NEW(pService, CBcPerfTestStaticService);
	
	return CDscDispatcherCenterDemon::instance()->RegistDscTask(pService, CBcPerfTestStaticService::EN_SERVICE_TYPE, 1);
} 
