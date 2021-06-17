#ifndef PROC_HANDLER_H_48954954978423763289743208923498732497873214
#define PROC_HANDLER_H_48954954978423763289743208923498732497873214

#include <list>

#include "ace/Configuration.h"
#include "ace/Signal.h"
#include "ace/SString.h"
#include "ace/Process_Manager.h"

class CWatchdogHandler: public ACE_Event_Handler
{
private:
	class CAppParam
	{
	public:
		CAppParam()
		: m_pidApp(0)
		{
		}

	public:
		ACE_TString m_strAppName;
		ACE_TString	m_strAppParam;
		pid_t		m_pidApp;
	};

public:
	CWatchdogHandler(ACE_Process_Manager& pm);

public:
	int Init(int argc, ACE_TCHAR *argv[]);

public:
	virtual int handle_exit (ACE_Process* process);
	virtual ACE_INT32 handle_signal (ACE_INT32 signum, siginfo_t * = 0, ucontext_t * = 0);

private:
	ACE_INT32 ParseArgs(int argc, ACE_TCHAR *argv[]);
	int StartProc(void);
	int StartProc(CAppParam& rAppParam);
	int ReadConfigureInfo(void);
	int InitSignal(void);
	int get_string_value(ACE_Configuration_Heap& config, const ACE_TCHAR* pszSection, const ACE_TCHAR* pszName, ACE_TString& sValue);
	int GetAppParam(ACE_Configuration_Heap& config);

private:
	std::list<CAppParam> m_lstAppParam;
	//CAppParam	m_InventoryParam;
	//CAppParam	m_GateWayParam;

	ACE_TString m_strHome;
	ACE_Process_Manager& m_pm;
	ACE_Sig_Action m_no_sigpipe;
};
#endif
