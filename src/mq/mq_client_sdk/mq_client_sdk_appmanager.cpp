#include "dsc/service/dsc_reactor_service_container.h"
#include "dsc/plugin/dsc_service_factory.h"

#include "mq_client_sdk/mq_client_sdk_appmanager.h"
#include "mq_client_sdk/mq_client_sdk_agent_service.h"
#include "mq_client_sdk/mq_client_sdk_agent_service_factory.h"

ACE_INT32 CMqClientSdkAppManager::OnInit()
{
	if (CDscAppManager::OnInit())
	{
		return -1;
	}

	return 0;
}

ACE_INT32 CMqClientSdkAppManager::OnExit(void)
{

	return CDscAppManager::OnExit();
}

ACE_INT32 CMqClientSdkAppManager::InitService(MQ::CInitMqClientSdkParam& rInitMqClientSdkParam)
{
	CMqClientSdkAgentServiceFactory mqClientSdkAgentServiceFactory(rInitMqClientSdkParam);

	ACE_INT32 nRet = ::RegistOneReactorDscService(&mqClientSdkAgentServiceFactory);

	if (nRet)
	{
		DSC_RUN_LOG_ERROR("mq client agent service regist failed");

		return -1;
	}
	else
	{
		DSC_RUN_LOG_FINE("mq client agent service regist successfully");

		return 0;
	}
}
