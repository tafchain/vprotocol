#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include "mq_scheduler/mq_scheduler_appmanager.h"
#include "mq_common/mq_id_def.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{ 
	CMqSchedulerAppManager* pMqSchedulerAppManager = ::new(std::nothrow) CMqSchedulerAppManager;
	if(!pMqSchedulerAppManager) 
	{ 
		ACE_OS::printf("failed to new bc appmanager!"); 
		
		return -1; 
	} 

	pMqSchedulerAppManager->SetNodeType(MQ_ID::EN_MQ_SCHEDULER_NODE_TYPE);
	if( pMqSchedulerAppManager->Init(argc, argv) )
	{ 
		ACE_OS::printf("mq scheduler init failed, now exit!\n"); 
		pMqSchedulerAppManager->Exit();
		delete pMqSchedulerAppManager;

		return -1; 
	} 
	
	ACE_OS::printf("mq scheduler init succeed, running...\n"); 
	pMqSchedulerAppManager->Run_Loop(); 
	delete pMqSchedulerAppManager;
	ACE_OS::printf("mq scheduler terminated!\n"); 
	
	return 0; 
}
