#ifndef CC_UPDATE_USER_JSON_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765
#define CC_UPDATE_USER_JSON_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765

#include "dsc/plugin/i_dsc_plugin.h"

class CCcUpdateUserJsonDemoServicePlugin : public IDscPlugin
{ 
public:
	CCcUpdateUserJsonDemoServicePlugin();

public: 
	ACE_INT32 OnInit(void); 
	
}; 
#endif