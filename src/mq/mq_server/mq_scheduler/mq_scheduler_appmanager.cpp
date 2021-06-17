#include "dsc/dsc_comm_def.h"

#include "mq_scheduler/mq_scheduler_appmanager.h"

ACE_INT32 CMqSchedulerAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

	return 0;
}

ACE_INT32 CMqSchedulerAppManager::OnExit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnExit());

	return 0;
}


