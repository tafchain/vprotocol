#ifndef REG_TEST_SERVICE_FACTORY_H_548709540890878957985789
#define REG_TEST_SERVICE_FACTORY_H_548709540890878957985789

#include "dsc/service/i_dsc_service_container.h"

class CRegTestServiceFactory : public IDscServiceFactory
{ 
public:
	CRegTestServiceFactory();

public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif