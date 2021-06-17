#ifndef DEMO_CLIENT_SERVICE_FACTORY_H_548709540890878957985789
#define DEMO_CLIENT_SERVICE_FACTORY_H_548709540890878957985789

#include "dsc/service/dsc_service_container.h"

class CDemoClientServiceFactory : public IDscServiceFactory
{ 
public:
	CDemoClientServiceFactory();

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif