#ifndef VBH_PERF_TEST_APPMANAGER_H_4322978438904231932198754089234097821308359
#define VBH_PERF_TEST_APPMANAGER_H_4322978438904231932198754089234097821308359

#include "dsc/dsc_app_mng.h"

class CBcPerfTestAppManager : public CDscAppManager
{
public:
	ACE_UINT32 GetTestUserNum(void) const;

protected:
	virtual ACE_INT32 OnInit(void);
	virtual void OnParam(const int argc, ACE_TCHAR* argv);

public:
	ACE_UINT32 m_nTestUserNum = 0;
};

#endif
