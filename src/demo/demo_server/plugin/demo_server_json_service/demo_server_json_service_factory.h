#ifndef DEMO_SERVER_JSON_FACTORY_H_3427842384289024397824798234
#define DEMO_SERVER_JSON_FACTORY_H_3427842384289024397824798234

#include "dsc/protocol/http/dsc_server_http_dispatcher.h"

class CDemoServerJsonFactory : public CServerHttpDispatcher::INestServerHttpDscServiceFactory
{ 
public:
	CDemoServerJsonFactory();

public:
	virtual CDscNestServerHttpService* CreateDscNestServerHttpService(void);

private:
	ACE_UINT16 m_nServiceID;
}; 
#endif