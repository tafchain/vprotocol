#ifndef IDC_CLIENT_SERVICE_PLUGIN_H_34075918245125
#define IDC_CLIENT_SERVICE_PLUGIN_H_34075918245125

#include "dsc/plugin/i_dsc_plugin.h"

class CIdcClientServicePlugin : public IDscPlugin
{
public:
	ACE_INT32 OnInit(void);
};
#endif