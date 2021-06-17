#ifndef MQ_FORWARDER_APPMANAGER_H_589598732987234798243978
#define MQ_FORWARDER_APPMANAGER_H_589598732987234798243978

#include "dsc/dsc_app_mng.h"

class CMqForwarderAppManager : public CDscAppManager
{

protected:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
};

#endif
