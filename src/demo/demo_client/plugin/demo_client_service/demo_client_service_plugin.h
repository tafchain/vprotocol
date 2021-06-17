#ifndef DEMO_CLIENT_SERVICE_PLUG_IN_H_43278934298734220430423781342789432296
#define DEMO_CLIENT_SERVICE_PLUG_IN_H_43278934298734220430423781342789432296

#include "dsc/plugin/i_dsc_plugin.h"

class CDemoClientServicePlugIn : public IDscPlugin
{ 
public: 
	ACE_INT32 OnInit(void); 
}; 
#endif