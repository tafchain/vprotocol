#ifndef IDC_SERVER_SERVICE_PLUGIN_H_23469023468
#define IDC_SERVER_SERVICE_PLUGIN_H_23469023468

#include "dsc/plugin/i_dsc_plugin.h"
#include "dsc/service/dsc_service_container.h"

class CIDomServerServicePlugin : public IDscPlugin
{
public:
	CIDomServerServicePlugin();

public:
	ACE_INT32 OnInit(void);

};



class CIDomServerServiceFactory : public IDscServiceFactory
{
public:
	CIDomServerServiceFactory();

public:
	virtual CDscService* CreateDscService(void);

public:
	CDscString m_strAddr;
	ACE_INT32 m_nPort;
	ACE_INT32 m_nShareTime;

private:
	ACE_UINT16 m_nServiceID;
};
#endif