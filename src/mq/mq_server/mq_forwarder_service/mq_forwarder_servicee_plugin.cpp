#include "dsc/service/dsc_reactor_service_container.h"
#include "dsc/plugin/dsc_service_factory.h"

#include "mq_forwarder_service/mq_forwarder_service.h"
#include "mq_forwarder_service/mq_forwarder_servicee_plugin.h"

CMqForwarderServicePlugIn::CMqForwarderServicePlugIn()
{
}

ACE_INT32 CMqForwarderServicePlugIn::OnInit(void)
{
	CDscServiceFactory<CMqForwarderService> mqForwarderServiceFactory;

	if (::RegistReactorDscService(&mqForwarderServiceFactory, MQ_ID::EN_MQ_FORWARDER_SERVICE_CONTAINER_TYPE))
	{
		DSC_RUN_LOG_ERROR("mq forwarder service regist failed");

		return -1;
	}
	else
	{
		DSC_RUN_LOG_FINE("mq forwarder service regist successfully");
	}

	return 0;
}

ACE_INT32 CMqForwarderServicePlugIn::OnExit(void)
{

	return 0;
}

extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CMqForwarderServicePlugIn* pMqForwarderServicePlugIn = NULL;

	DSC_NEW(pMqForwarderServicePlugIn, CMqForwarderServicePlugIn);

	return pMqForwarderServicePlugIn;
}
