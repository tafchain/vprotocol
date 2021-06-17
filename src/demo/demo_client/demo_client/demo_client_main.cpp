#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include "dsc/mem_mng/dsc_allocator.h"

#include "demo_client/demo_client_appmanager.h"
#include "common/comm_id_def.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) 
{ 
	CDemoClientAppManager* pAppManager = NULL; 

	DSC_NEW(pAppManager, CDemoClientAppManager);
	if(!pAppManager) 
	{ 
		ACE_OS::printf("failed to new demo clent appmanager!"); 
		
		return -1; 
	} 

	pAppManager->SetNodeType(NS_DEMO_ID_DEF::DEMO_CLIENT_NODE_TYPE);
	if( pAppManager->Init(argc, argv) )
	{ 
		ACE_OS::printf("demo clent init failed, now exit!\n"); 
		pAppManager->Exit();
		delete pAppManager;

		return -1; 
	} 
	
	ACE_OS::printf("demo clent init succeed, running...\n"); 
	pAppManager->Run_Loop(); 
	delete pAppManager;
	ACE_OS::printf("demo clent terminated!\n"); 
	
	return 0; 
}