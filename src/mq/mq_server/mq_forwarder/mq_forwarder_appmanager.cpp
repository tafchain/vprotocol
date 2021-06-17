#include "dsc/dsc_comm_def.h"

#include "mq_forwarder/mq_forwarder_appmanager.h"

ACE_INT32 CMqForwarderAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

	return 0;
}

ACE_INT32 CMqForwarderAppManager::OnExit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnExit());

	return 0;
}


