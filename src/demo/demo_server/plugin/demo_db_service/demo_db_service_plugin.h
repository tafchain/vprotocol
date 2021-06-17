#ifndef DEMO_DB_SERVICE_PLUG_IN_H_45546465865435456465465465465
#define DEMO_DB_SERVICE_PLUG_IN_H_45546465865435456465465465465

#include "dsc/plugin/i_dsc_plugin.h"

class CDemoDbServicePlugIn : public IDscPlugin
{ 
public: 
	ACE_INT32 OnInit(void); 
}; 
#endif