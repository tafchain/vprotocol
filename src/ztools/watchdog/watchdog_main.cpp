#include "ace/Process_Manager.h"
#include "ace/Object_Manager.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_stdio.h"

#include "watchdog/watchdog_handler.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
	ACE_Process_Manager pm( 128, ACE_Reactor::instance() );
	CWatchdogHandler* pHandler = new CWatchdogHandler(pm);

	if( pHandler->Init(argc, argv) )
	{
		delete pHandler;
		ACE_OS::printf( "init failed.\n");

		return -1;
	}

	ACE_Reactor::instance ()->run_reactor_event_loop ();

	pm.close();
	ACE_Object_Manager::instance()->fini();

	delete pHandler;

	return 0;
}

