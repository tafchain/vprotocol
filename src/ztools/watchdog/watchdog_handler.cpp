#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Reactor.h"
#include "ace/Configuration_Import_Export.h"

#include "watchdog/watchdog_handler.h"

#define FORWARD_CALL(FunctionCall) \
	if(-1 == FunctionCall) \
{ \
	return -1; \
}

#if !( defined(ACE_WIN32) || defined(ACE_WIN64) )
#define FILE_PATH_SPLIT_STR   	"/"//文件分隔字符串
#else
#define FILE_PATH_SPLIT_STR 	"\\"//文件分隔字符串
#endif

CWatchdogHandler::CWatchdogHandler(ACE_Process_Manager& pm)
: m_pm(pm)
, m_no_sigpipe( (ACE_SignalHandler)SIG_IGN )
{
}

int CWatchdogHandler::handle_exit (ACE_Process* process)
{
	pid_t pit = process->getpid();

	for(std::list<CAppParam>::iterator it = m_lstAppParam.begin(); it != m_lstAppParam.end(); ++it)
	{
		if(pit == it->m_pidApp)
		{
			ACE_OS::sleep(1);

			this->StartProc(*it);
			break;
		}
	}

	return 0;
}

int CWatchdogHandler::Init(int argc, ACE_TCHAR *argv[])
{
	this->ParseArgs(argc, argv);

	FORWARD_CALL( this->ReadConfigureInfo() );
	FORWARD_CALL( this->StartProc() );

	return 0;
}

ACE_INT32 CWatchdogHandler::handle_signal(ACE_INT32 signum, siginfo_t *, ucontext_t *)
{
	ACE_Sig_Set sigs;

	sigs.sig_add (SIGHUP);
	sigs.sig_add (SIGINT);
	sigs.sig_add (SIGABRT);
	sigs.sig_add (SIGALRM);
	sigs.sig_add (SIGUSR1);
	sigs.sig_add (SIGUSR2);
	sigs.sig_add (SIGCLD);
	sigs.sig_add (SIGTERM);
	sigs.sig_add (SIGCHLD);
	//sigs.sig_add (SIGINFO);
	sigs.sig_add (SIGIO);

#ifndef ACE_CONFIG_WIN32_H
#if !defined(_IOS_PLATFORM_) && !defined(_FREEBSD_PLATFORM_)
	sigs.sig_add (SIGPOLL);
#endif
	sigs.sig_add (SIGVTALRM);
	sigs.sig_add (SIGPROF);
#endif

	ACE_Sig_Guard guard (&sigs);

	switch(signum)
	{
	case SIGINT:
	case SIGTERM:
	case SIGUSR2:
		{
			ACE_OS::printf("whatdog exit...\n");
			//退出子进程, 改由stop.sh shell programe杀进程
			//for(std::list<CAppParam>::iterator it = m_lstAppParam.begin(); it != m_lstAppParam.end(); ++it)
			//{
			//	ACE_OS::kill(it->m_pidApp, signum);
			//}

			//ACE_Reactor::instance ()->remove_handler(this, ACE_Event_Handler::ALL_EVENTS_MASK | DONT_CALL);
			//ACE_Reactor::instance ()->end_reactor_event_loop();
		}
		break;
	default:
		{
			break;
		}
	}

	return 0;
}

int CWatchdogHandler::StartProc(CAppParam& rAppParam)
{
	ACE_Process_Options options;
	ACE_TString strTemp;

	strTemp = rAppParam.m_strAppName;
	strTemp += ' ';
	strTemp += rAppParam.m_strAppParam;
	options.command_line( strTemp.c_str() );
	rAppParam.m_pidApp = m_pm.spawn(options, this);

	return 0;
}

int CWatchdogHandler::StartProc(void)
{
	for(std::list<CAppParam>::iterator it = m_lstAppParam.begin(); it != m_lstAppParam.end(); ++it)
	{
		this->StartProc(*it);
	}

	return 0;
}

int CWatchdogHandler::ReadConfigureInfo(void)
{
	ACE_Configuration_Heap config;
	if (config.open () == -1)
	{
		ACE_OS::printf("failed to init configure.\n");

		return -1;
	}

	ACE_Registry_ImpExp config_importer (config);

	ACE_TString strConfigureFile = m_strHome;

	strConfigureFile += FILE_PATH_SPLIT_STR;
	strConfigureFile += "conf";
	strConfigureFile += FILE_PATH_SPLIT_STR;
	strConfigureFile += "watchdog.ini";

	if (config_importer.import_config( strConfigureFile.c_str() ) == -1)
	{
		ACE_OS::printf( "failed to open configure file:%s.\n", strConfigureFile.c_str() );

		return -1;
	}

	FORWARD_CALL( this->GetAppParam(config) );

	return 0;
}

int CWatchdogHandler::InitSignal(void)
{
	ACE_Reactor::instance ()->register_handler(SIGHUP, this);
	ACE_Reactor::instance ()->register_handler(SIGINT, this);
	//begin xyn26379 04-11-24 mod for NYGD4833
	ACE_Reactor::instance ()->register_handler(SIGABRT, this);
	//end xyn26379 04-11-24 mod for NYGD4833
	//ACE_Reactor::instance ()->GetReactor().register_handler(SIGPIPE, this);
	ACE_Reactor::instance ()->register_handler(SIGALRM, this);
	ACE_Reactor::instance ()->register_handler(SIGUSR1, this);
	ACE_Reactor::instance ()->register_handler(SIGUSR2, this);
#ifndef ACE_CONFIG_WIN32_H
#if !defined(_IOS_PLATFORM_) && !defined(_FREEBSD_PLATFORM_)
	ACE_Reactor::instance ()->register_handler(SIGPOLL, this);
#endif
	ACE_Reactor::instance ()->register_handler(SIGVTALRM, this);
	ACE_Reactor::instance ()->register_handler(SIGPROF, this);
#endif
	//ACE_Reactor::instance ()->register_handler(SIGCLD, this);
	// BEGIN: Added by QiaoShan26188, 2004_6_18   问题单号:NYGD03304
	ACE_Reactor::instance ()->register_handler(SIGTERM, this);
	//ACE_Reactor::instance ()->register_handler(SIGCHLD, this);
	//ACE_Reactor::instance ()->register_handler(SIGINFO, this);
	ACE_Reactor::instance ()->register_handler(SIGIO, this);

	m_no_sigpipe.register_action (SIGPIPE, NULL);

	return 0;
}

int CWatchdogHandler::get_string_value(ACE_Configuration_Heap& config, const ACE_TCHAR* pszSection, const ACE_TCHAR* pszName, ACE_TString& sValue)
{
	ACE_Configuration_Section_Key section;

	if(config.open_section(config.root_section (),
		pszSection, 0, section) == -1)
	{
		ACE_OS::printf( "failed to open inventory section:%s.\n", pszSection);

		return -1;
	}

	if(config.get_string_value(section, pszName, sValue) == -1)
	{
		ACE_OS::printf( "failed to open item, key:%s.\n", pszName);

		return -1;
	}

	return 0;
}

int CWatchdogHandler::GetAppParam(ACE_Configuration_Heap& config)
{
	ACE_TString strName;
	ACE_TString strTemp;
	const ACE_Configuration_Section_Key& Key = config.root_section();
	int nIndex = 0;
	CAppParam appParam;

	while( !config.enumerate_sections(Key, nIndex, strName) )
	{
		if( this->get_string_value(config, strName.c_str(), ACE_TEXT("app_name"), strTemp) )
		{
			ACE_OS::printf( "failed to read app_name item.\n");

			return -1;
		}

		appParam.m_strAppName = m_strHome;
		appParam.m_strAppName += FILE_PATH_SPLIT_STR;
		appParam.m_strAppName += "bin";
		appParam.m_strAppName += FILE_PATH_SPLIT_STR;
		appParam.m_strAppName += strTemp;


		if( this->get_string_value(config, strName.c_str(), ACE_TEXT("app_param"), appParam.m_strAppParam) )
		{
			ACE_OS::printf( "failed to read app_param item.\n");

			return -1;
		}
		m_lstAppParam.push_back(appParam);
		++nIndex;
	}

	return 0;
}

ACE_INT32 CWatchdogHandler::ParseArgs(int argc, ACE_TCHAR *argv[])
{
	m_strHome.clear();

	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("h:H:b:B"), 1);

	int c = get_opt();
	while(-1 != c)
	{
		switch (c)
		{
		case 'h':
		case 'H':
			{
				ACE_TCHAR* parg = get_opt.opt_arg();
				if(parg)
				{
					m_strHome = parg;
				}
				else
				{
					char* pszHomePath = ACE_OS::getenv("WORK_ROOT");
					if (!pszHomePath)
					{
						ACE_OS::printf("Please check the setting of 'WORK_ROOT' or '-h{homepath}' !\n");

						return -1;
					}
					m_strHome = pszHomePath;
				}

				break;
			}
		default:
			{
				ACE_OS::printf("Please check the setting of 'WORK_ROOT' or '-h{homepath}' !\n");
				break;
			}
		}
		c = get_opt();
	}

	return 0;
}
