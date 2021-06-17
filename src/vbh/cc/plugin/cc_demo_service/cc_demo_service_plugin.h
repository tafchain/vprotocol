#ifndef CC_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765
#define CC_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765

#include "dsc/plugin/i_dsc_plugin.h"

class CCcDemoServicePlugin : public IDscPlugin
{ 
public:
	CCcDemoServicePlugin();

public: 
	ACE_INT32 OnInit(void); 
	
}; 
#endif