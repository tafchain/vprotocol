#ifndef CC_UPDATE_USER_JSON_DEMO_SERVICE_FACTORY_H_34297342987324586321821386231
#define CC_UPDATE_USER_JSON_DEMO_SERVICE_FACTORY_H_34297342987324586321821386231

#include "dsc/service/i_dsc_service_container.h"

class CCcUpdateUserJsonDemoServiceFactory : public IDscServiceFactory
{ 
public:
	virtual CDscService* CreateDscService(void);

private:
	ACE_UINT16 m_nServiceID = 0;
}; 
#endif
