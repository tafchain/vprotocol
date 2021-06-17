#ifndef CC_UPDATE_USER_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765
#define CC_UPDATE_USER_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765

#include "dsc/plugin/i_dsc_plugin.h"

class CCcUpdateUserDemoServicePlugin : public IDscPlugin
{ 
public:
	CCcUpdateUserDemoServicePlugin();

public: 
	ACE_INT32 OnInit(void); 
	
}; 
#endif