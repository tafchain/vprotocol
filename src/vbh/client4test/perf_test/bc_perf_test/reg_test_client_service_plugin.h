#ifndef REG_TEST_SERVICE_PLUG_IN_H_43278934298734220430423781342789432296
#define REG_TEST_SERVICE_PLUG_IN_H_43278934298734220430423781342789432296

#include "dsc/plugin/i_dsc_plugin.h"

class CRegTestServicePlugIn : public IDscPlugin
{ 
public: 
	ACE_INT32 OnInit(void); 
}; 
#endif