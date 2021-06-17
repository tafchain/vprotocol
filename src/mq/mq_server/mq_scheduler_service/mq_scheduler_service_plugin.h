#ifndef MQ_SCHEDULER_AGENT_PLUGIN_H_32789234872438623672376564
#define MQ_SCHEDULER_AGENT_PLUGIN_H_32789234872438623672376564

#include "dsc/plugin/i_dsc_plugin.h"

class CMqSchedulerServicePlugIn : public IDscPlugin
{
public:
	CMqSchedulerServicePlugIn();

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);
};
#endif