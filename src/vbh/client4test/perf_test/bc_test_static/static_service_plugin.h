#ifndef STATIC_SERVICE_PLUGIN_H_43784379346734
#define STATIC_SERVICE_PLUGIN_H_43784379346734

#include "dsc/plugin/i_dsc_plugin.h"

class CStaticServicePlugin : public IDscPlugin
{ 
public: 
	ACE_INT32 OnInit(void); 
}; 
#endif