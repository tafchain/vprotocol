#include "dsc/service/dsc_reactor_service_container.h"
#include "dsc/plugin/dsc_service_factory.h"

#include "mq_scheduler_service/mq_scheduler_service.h"
#include "mq_scheduler_service/mq_scheduler_service_plugin.h"

CMqSchedulerServicePlugIn::CMqSchedulerServicePlugIn()
{
}

ACE_INT32 CMqSchedulerServicePlugIn::OnInit(void)
{
	CDscServiceFactory<CMqSchedulerService> mqSchedulerServiceFactory;

	ACE_INT32 nRet = ::RegistOneReactorDscService(&mqSchedulerServiceFactory);

	if (nRet)
	{
		DSC_RUN_LOG_ERROR("mq scheduler service service regist failed");
	}
	else
	{
		DSC_RUN_LOG_FINE("mq scheduler service service regist successfully");
	}

	return nRet;
}

ACE_INT32 CMqSchedulerServicePlugIn::OnExit(void)
{

	return 0;
}

extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CMqSchedulerServicePlugIn* pMqSchedulerServicePlugIn = NULL;

	DSC_NEW(pMqSchedulerServicePlugIn, CMqSchedulerServicePlugIn);

	return pMqSchedulerServicePlugIn;
}
