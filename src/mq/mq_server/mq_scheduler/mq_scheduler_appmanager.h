#ifndef MQ_SCHEDULER_APPMANAGER_H_57895368975468723487
#define MQ_SCHEDULER_APPMANAGER_H_57895368975468723487

#include "dsc/dsc_app_mng.h"

class CMqSchedulerAppManager : public CDscAppManager
{

protected:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
};

#endif
