#include "ace/Object_Manager.h"
#include "ace/Reactor.h"
#include "ace/Process_Manager.h"
#include "ace/Thread_Manager.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

#include "mq/p2p_mq/mq_interface.h"

ACE_UINT32 g_nTradeNum = 0;
class CP2pMqMsgCallback : public IP2pMqMsgCallback
{
public:
	virtual void OnMessage(CP2pMqAddrMsg& msg);
	virtual void Release(void);
};

ACE_THR_FUNC_RETURN ThreadFunction(void* para);
ACE_INT32 GetAppID(ACE_INT16& nLocalAppID, ACE_INT16& nRemoteAppID, int argc, ACE_TCHAR *argv[]);
ACE_INT16 nLocalAppID;
ACE_INT16 nRemoteAppID;
#define DEF_BASE_PORT 11978

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
	CInitMsgParam param;

	::GetAppID(nLocalAppID, nRemoteAppID, argc, argv);
	if (-1 == nLocalAppID || -1 == nRemoteAppID)
	{
		ACE_OS::printf("app id cann't be -1\n");

		return -1;
	}

	param.m_pP2pMqMsgCallback = new CP2pMqMsgCallback;
	param.m_nAppType = 777;
	param.m_nAppID = nLocalAppID;
	param.m_listenAddr.m_nPort = DEF_BASE_PORT + param.m_nAppID;
	param.m_listenAddr.m_pIpAddr = "127.0.0.1";

	CP2pMqConnectAddr addr;

	addr.m_remoteAddr.m_nPort = DEF_BASE_PORT + nRemoteAppID;
	addr.m_remoteAddr.m_pIpAddr = "127.0.0.1";
	param.m_lstConnectAddr.push_back(addr);

	InitP2pMqMsgSdk(param);

	std::list<int> lstTopic;

	lstTopic.push_back(24978 + nLocalAppID);
	::SubP2pMqMsgID(lstTopic);
	
	ACE_Process_Manager pm( 128, ACE_Reactor::instance() );

	ACE_Thread_Manager::instance()->spawn(ThreadFunction, NULL, THR_NEW_LWP | THR_DETACHED);
	ACE_Reactor::instance ()->run_reactor_event_loop ();

	pm.close();
	ACE_Object_Manager::instance()->fini();

	return 0;
}


ACE_THR_FUNC_RETURN ThreadFunction(void* para)
{
	ACE_OS::sleep(4);

	char buf[16] = "123456789";
	CP2pMqMsg msg;

	msg.m_nTopicID = 24978 + nRemoteAppID;
	msg.m_pMsg = buf;
	msg.m_nMsgSize = 10;

	::PubPollP2pMqMsg(msg);

	for (;;)
	{
		ACE_OS::printf("trade num: %d\n", g_nTradeNum);
		g_nTradeNum = 0;
		ACE_OS::sleep(1);
	}
	
	return 0;
}

void CP2pMqMsgCallback::OnMessage(CP2pMqAddrMsg& msg)
{
	++g_nTradeNum;

	char buf[16] = "123456789";
	CP2pMqMsg pubMsg;

	pubMsg.m_nTopicID = 24978 + nRemoteAppID;
	pubMsg.m_pMsg = buf;
	pubMsg.m_nMsgSize = 10;

	::PubPollP2pMqMsg(pubMsg);

}

void CP2pMqMsgCallback::Release(void)
{
	delete this;
}

ACE_INT32 GetAppID(ACE_INT16& nLocalAppID, ACE_INT16& nRemoteAppID, int argc, ACE_TCHAR *argv[])
{
	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("h:H:n:N:t:T"), 1);
	int c = get_opt();

	nLocalAppID = -1;
	nRemoteAppID = -1;

	while (-1 != c)
	{
		switch (c)
		{
		case 'h':
		case 'H':
		{// 设置模块ID
			ACE_TCHAR* parg = get_opt.opt_arg();
			if (parg)
			{
				nLocalAppID = ACE_OS::atoi(parg);
			}
			else
			{
				ACE_OS::printf("Please check '-h local app it' !\n");

				return -1;
			}


			break;
		}
		case 'n':
		case 'N':
		{// 设置模块ID
			ACE_TCHAR* parg = get_opt.opt_arg();
			if (parg)
			{
				nRemoteAppID = ACE_OS::atoi(parg);
			}
			else
			{
				ACE_OS::printf("Please check '-n remote app it' !\n");

				return -1;
			}

			break;
		}
		default:
		{// 其它参数, 默认不处理
			break;
		}
		}
		c = get_opt();
	}

	return 0;
}
