#ifndef MQ_FORWARDER_SERVICE_PLUGIN_H_4320784238634215623417823497342562315623465
#define MQ_FORWARDER_SERVICE_PLUGIN_H_4320784238634215623417823497342562315623465

#include "dsc/plugin/i_dsc_plugin.h"

class CMqForwarderServicePlugIn : public IDscPlugin
{
public:
	CMqForwarderServicePlugIn();

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);
};
#endif