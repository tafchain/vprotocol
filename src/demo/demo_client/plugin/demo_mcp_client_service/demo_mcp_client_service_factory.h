#ifndef DEMO_MCP_CLIENT_SERVICE_FACTORY_H_548709540890878957985789
#define DEMO_MCP_CLIENT_SERVICE_FACTORY_H_548709540890878957985789

#include "dsc/service/dsc_service_container.h"

class CPerfTestServiceFactory : public IDscServiceFactory
{ 
public:
	CPerfTestServiceFactory();

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif