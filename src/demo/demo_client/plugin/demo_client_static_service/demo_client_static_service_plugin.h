#ifndef TRADE_SERVER_SERVICE_PLUGIN_H_789247896856768
#define TRADE_SERVER_SERVICE_PLUGIN_H_789247896856768

#include "dsc/plugin/i_dsc_plugin.h"

class CDemoClientStaticServicePlugin : public IDscPlugin
{ 
public: 
	ACE_INT32 OnInit(void); 
}; 
#endif