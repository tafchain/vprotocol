#ifndef MQ_CLIENT_SDK_AGENT_SERVICE_FACTORY_H_45546465865435456465465465465
#define MQ_CLIENT_SDK_AGENT_SERVICE_FACTORY_H_45546465865435456465465465465

#include "dsc/service/i_dsc_service_container.h"

class CMqClientSdkAgentServiceFactory : public IDscServiceFactory
{ 
public:
	CMqClientSdkAgentServiceFactory(MQ::CInitMqClientSdkParam& rInitMqClientSdkParam);

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_INT16 m_nServiceID;
	MQ::CInitMqClientSdkParam& m_initMqClientSdkParam;
};
#endif