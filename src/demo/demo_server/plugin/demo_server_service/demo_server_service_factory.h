#ifndef DEMO_SERVER_SERVICE_FACTORY_H_45546465865435456465465465465
#define DEMO_SERVER_SERVICE_FACTORY_H_45546465865435456465465465465

#include "dsc/service/dsc_service_container.h"

class CDemoServerServiceFactory : public IDscServiceFactory
{ 
public:
	CDemoServerServiceFactory();

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif