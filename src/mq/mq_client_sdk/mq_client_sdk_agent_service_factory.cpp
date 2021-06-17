#include "mq_client_sdk/mq_client_sdk_agent_service.h"
#include "mq_client_sdk/mq_client_sdk_agent_service_factory.h"

CMqClientSdkAgentServiceFactory::CMqClientSdkAgentServiceFactory(MQ::CInitMqClientSdkParam& rInitMqClientSdkParam)
: m_nServiceID(0)
, m_initMqClientSdkParam(rInitMqClientSdkParam)
{
}

CDscService* CMqClientSdkAgentServiceFactory::CreateDscService(void)
{ 
	CMqClientSdkAgentService* pMqClientSdkAgentService = NULL;

	DSC_NEW(pMqClientSdkAgentService, CMqClientSdkAgentService(m_initMqClientSdkParam));

	if(pMqClientSdkAgentService)
	{ 
		pMqClientSdkAgentService->SetType(CMqClientSdkAgentService::EN_SERVICE_TYPE);
		pMqClientSdkAgentService->SetID(++m_nServiceID);
	} 

	return pMqClientSdkAgentService;
} 