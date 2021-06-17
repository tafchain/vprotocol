#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include "dsc/mem_mng/dsc_allocator.h"

#include "vbh_test_static/vbh_test_static_appmanager.h"
#include "vbh_perf_test_comm/vbh_perf_test_comm_id_def.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) 
{ 
	CBcTestStaticAppManager* pAppManager = NULL; 

	DSC_NEW(pAppManager, CBcTestStaticAppManager);
	if(!pAppManager) 
	{ 
		ACE_OS::printf("failed to new bc perf test appmanager!"); 
		
		return -1; 
	} 

	pAppManager->SetNodeType(VBH_TEST::VBH_TEST_STATIC_NODE_TYPE);
	if( pAppManager->Init(argc, argv) )
	{ 
		ACE_OS::printf("bc perf test init failed, now exit!\n"); 
		pAppManager->Exit();
		delete pAppManager;

		return -1; 
	} 
	
	ACE_OS::printf("bc perf test init succeed, running...\n"); 
	pAppManager->Run_Loop(); 
	delete pAppManager;
	ACE_OS::printf("bc perf test terminated!\n"); 
	
	return 0; 
}