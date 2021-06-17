#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include "mq_forwarder/mq_forwarder_appmanager.h"
#include "mq_common/mq_id_def.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{ 
	CMqForwarderAppManager* pMqForwarderAppManager = ::new(std::nothrow) CMqForwarderAppManager;
	if(!pMqForwarderAppManager) 
	{ 
		ACE_OS::printf("failed to new bc appmanager!"); 
		
		return -1; 
	} 

	pMqForwarderAppManager->SetNodeType(MQ_ID::EN_MQ_FORWARDER_NODE_TYPE);
	if( pMqForwarderAppManager->Init(argc, argv) )
	{ 
		ACE_OS::printf("mq forwarder init failed, now exit!\n"); 
		pMqForwarderAppManager->Exit();
		delete pMqForwarderAppManager;

		return -1; 
	} 
	
	ACE_OS::printf("mq forwarder init succeed, running...\n"); 
	pMqForwarderAppManager->Run_Loop(); 
	delete pMqForwarderAppManager;
	ACE_OS::printf("mq forwarder terminated!\n"); 
	
	return 0; 
}