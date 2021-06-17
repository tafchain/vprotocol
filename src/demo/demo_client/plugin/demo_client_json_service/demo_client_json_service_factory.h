#ifndef DEMO_CLIENT_JSON_FACTORY_H_4237843284230893420842304378
#define DEMO_CLIENT_JSON_FACTORY_H_4237843284230893420842304378

#include "dsc/service/dsc_service_container.h"

class CDemoClientJsonFactory : public IDscServiceFactory
{ 
public:
	CDemoClientJsonFactory();

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif