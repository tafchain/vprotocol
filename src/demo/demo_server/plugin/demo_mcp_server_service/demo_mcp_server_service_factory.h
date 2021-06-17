#ifndef DEMO_MCP_SERVER_SERVICE_FACTORY_H_45546465865435456465465465465
#define DEMO_MCP_SERVER_SERVICE_FACTORY_H_45546465865435456465465465465

#include "dsc/service/dsc_service_container.h"

class CDemoMcpServerServiceFactory : public IDscServiceFactory
{ 
public:
	CDemoMcpServerServiceFactory();

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif