#ifndef CC_NODEJS_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765
#define CC_NODEJS_DEMO_SERVICE_PLUGIN_H_43294394329432684328764317432765

#include "dsc/plugin/i_dsc_plugin.h"

class CCcNodeJsDemoServicePlugin : public IDscPlugin
{ 
public:
	CCcNodeJsDemoServicePlugin();

public: 
	ACE_INT32 OnInit(void); 
	
}; 
#endif