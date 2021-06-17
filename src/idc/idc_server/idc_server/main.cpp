#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include <stack>
#include <functional>

#include "dsc/mem_mng/dsc_allocator.h"
#include "dsc/dsc_log.h"

#include "idc/idc_common/idc_common_id_def.h"


#include "dsc/dsc_app_mng.h"

class CIDomServerAppManager : public CDscAppManager
{

protected:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
};

ACE_INT32 CIDomServerAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit());

	return 0;
}

ACE_INT32 CIDomServerAppManager::OnExit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnExit());

	return 0;
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	CIDomServerAppManager* pAppManager = new CIDomServerAppManager;
	if (!pAppManager)
	{
		ACE_OS::printf("failed to new idom server appmanager!");

		return -1;
	}

	pAppManager->SetNodeType(IDC::EN_IDC_SERVER_TYPE);
	if (pAppManager->Init(argc, argv))
	{
		ACE_OS::printf("idom server init failed, now exit!\n");
		pAppManager->Exit();
		delete pAppManager;

		return -1;
	}

	ACE_OS::printf("idom server init succeed, running...\n");
	pAppManager->Run_Loop();
	delete pAppManager;
	ACE_OS::printf("idom server terminated!\n");

	return 0;
}