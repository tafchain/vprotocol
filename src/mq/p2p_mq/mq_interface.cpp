#ifdef WIN32
#include <Shlwapi.h> 
#else
#include <unistd.h>
#endif

#include "ace/Init_ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Event.h"

#include "dsc/container/dsc_string.h"
#include "dsc/dispatcher/dsc_dispatcher_center.h"
#include "dsc/dsc_log.h"

#include "p2p_mq/mq_interface.h"
#include "p2p_mq/mq_app_mng.h"
#include "p2p_mq/mq_comm_def.h"

static void GetProgramDir(CDscString& rstrProgramDir)
{
#ifdef WIN32
	// 获取程序安装目录 strPath
	char szShortPathName[MAX_PATH];
	char szLongPathName[MAX_PATH];
	char* lpPart[MAX_PATH] = { NULL };

	::GetModuleFileNameA(NULL, szShortPathName, MAX_PATH);
	::PathRemoveFileSpecA(szShortPathName);
	::GetFullPathName(szShortPathName, MAX_PATH, szLongPathName, lpPart);
	rstrProgramDir = szLongPathName;
	rstrProgramDir += "\\";
#else
	char link[128];
	char path[256];

	sprintf(link, "/proc/%d/exe", getpid());
	int i = readlink(link, path, sizeof(path));
	rstrProgramDir.assign(path, i);
#endif
}

class CThreadParam
{
public:
	CThreadParam(CInitMsgParam& rParam)
		: m_param(rParam)
	{

	}

public:
	CInitMsgParam& m_param;
	ACE_Event m_event;
};

static void SendNetworkInfo2CallbackService(CInitMsgParam& rParam);

static ACE_THR_FUNC_RETURN ThreadFunction(void* para)
{
	CThreadParam* pThreadParam = (CThreadParam*)para;
	CMqAppMng* pMqAppMng = NULL;

	DSC_NEW(pMqAppMng, CMqAppMng);

	CDscString strAppType;
	CDscString strAppID;

	strAppType = pThreadParam->m_param.m_nAppType;
	strAppID = pThreadParam->m_param.m_nAppID;

	ACE_TCHAR* argv[7] = {"p2p_mq", "-t", strAppType.data(), "-n", strAppID.data(), "-h" };
	CDscString strProgramPath;
	
	::GetProgramDir(strProgramPath);

	int iIndex = strProgramPath.find_r(DSC_FILE_PATH_SPLIT_CHAR);
	if (-1 == iIndex)
	{
		pThreadParam->m_event.signal();

		return NULL;
	}

	strProgramPath.TrimRight(strProgramPath.size() - iIndex);

	iIndex = strProgramPath.find_r(DSC_FILE_PATH_SPLIT_CHAR);
	if (-1 == iIndex)
	{
		pThreadParam->m_event.signal();

		return NULL;
	}

	strProgramPath.TrimRight(strProgramPath.size() - iIndex);
	argv[6] = (ACE_TCHAR*)(strProgramPath.c_str());


	if (pMqAppMng->Init(7, argv))
	{
		pMqAppMng->Exit();
		DSC_DELETE(pMqAppMng);
		pThreadParam->m_event.signal();

		return NULL;
	}
	else
	{
		pMqAppMng->InitService(pThreadParam->m_param.m_pP2pMqMsgCallback);
		::SendNetworkInfo2CallbackService(pThreadParam->m_param);
		pThreadParam->m_event.signal();
		pMqAppMng->Run_Loop();
		DSC_DELETE(pMqAppMng);

		return 0;
	}
}

int InitP2pMqMsgSdk(CInitMsgParam& rParam)
{
	CThreadParam threadParam(rParam);

	ACE::init();

	if (-1 != ACE_Thread_Manager::instance()->spawn(ThreadFunction, &threadParam, THR_NEW_LWP | THR_DETACHED))
	{
		threadParam.m_event.wait();

		return 0;
	}

	return -1;
}

int ExitP2pMqMsgSdk(void)
{
	CDscAppManager::Instance()->SetAppExited();
	ACE::fini();

	return 0;
}

ACE_INT32 SendDscMessage(CDscMsg* pDscMsg)
{
	ACE_ASSERT(pDscMsg);

	CDscMsg::CDscMsgAddr& rDesMsgAddr = pDscMsg->GetDesMsgAddr();

	if ((rDesMsgAddr.GetNodeType() == DSC::EN_INVALID_TYPE) || (rDesMsgAddr.GetServiceType() == DSC::EN_INVALID_TYPE))
	{
		return CDscDispatcherCenterDemon::instance()->SendDscMessageByMsgType(pDscMsg);
	}
	else
	{
		if ((rDesMsgAddr.GetNodeID() == DSC::EN_INVALID_ID) || (rDesMsgAddr.GetServiceID() == DSC::EN_INVALID_ID))
		{
			return CDscDispatcherCenterDemon::instance()->SendDscMessageByTaskType(pDscMsg);
		}
		else
		{
			return CDscDispatcherCenterDemon::instance()->SendDscMessageByAddr(pDscMsg);
		}
	}
}

template<typename TYPE>
CDscMsg* GetDscMsg(TYPE& t, const CDscMsg::CDscMsgAddr& rAddress, const ACE_INT32 nMessageType)
{
	CDscEncodeState codeStateControl;

	DSC::GetSize(t, codeStateControl);

#ifdef DONNT_USING_MEMORY_POOL
	CDscMsg* pDscMsg = DSC::NewDscMsg(codeStateControl.GetSize());
#else
	CDscMsg* pDscMsg = CDscMsgAllocatorDemon::instance()->allocate(codeStateControl.GetSize());
#endif
	if (pDscMsg)
	{
		codeStateControl.SetBuffer(pDscMsg->GetMessageBodyPtr());

		t.Bind(codeStateControl);

		pDscMsg->SetSrcNodeType(CDscAppManager::Instance()->GetNodeType());
		pDscMsg->SetSrcNodeID(CDscAppManager::Instance()->GetNodeID());

		pDscMsg->SetDesMsgAddr(rAddress);
		pDscMsg->SetMsgType(nMessageType);
	}

	return pDscMsg;
}

template<typename TYPE>
ACE_INT32 SendDscMessage(TYPE& t, const CDscMsg::CDscMsgAddr& rAddress)
{
	CDscMsg* pDscMsg = ::GetDscMsg(t, rAddress, TYPE::EN_MSG_ID);
	if (pDscMsg)
	{
		if (::SendDscMessage(pDscMsg))
		{
			DSC_RUN_LOG_WARNING("send dsc message failed, msg type:%d", pDscMsg->GetMsgType());
#ifdef DONNT_USING_MEMORY_POOL
			DSC::FreeDscMsg(pDscMsg);
#else
			CDscMsgAllocatorDemon::instance()->deallocate(pDscMsg);
#endif

			return -1;
		}
		else
		{
			return 0;
		}
	}

	return -1;
}

int SubP2pMqMsgID(std::list<int>& lstTopic)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetServiceType(MQ_ID::EN_MQ_SERVICE_TYPE);
	addr.SetServiceID(1);

	MQ_ID::CSubMsgID2CallbackService subMsgID;

	for (std::list<int>::iterator it = lstTopic.begin(); it != lstTopic.end(); ++it)
	{
		subMsgID.m_lstMsgID.push_back(*it);
	}

	return ::SendDscMessage(subMsgID, addr);
}

void SendNetworkInfo2CallbackService(CInitMsgParam& rParam)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetServiceType(MQ_ID::EN_MQ_SERVICE_TYPE);
	addr.SetServiceID(1);

	MQ_ID::CNetworkAddrMsg msg;

	msg.m_listenAddr.m_nPort = rParam.m_listenAddr.m_nPort;
	msg.m_listenAddr.m_pIpAddr.assign(rParam.m_listenAddr.m_pIpAddr.c_str(), rParam.m_listenAddr.m_pIpAddr.size());

	MQ_ID::CConnectAddr cntAddr;

	for (std::list<CP2pMqConnectAddr>::iterator it = rParam.m_lstConnectAddr.begin(); it != rParam.m_lstConnectAddr.end(); ++it)
	{
		cntAddr.m_localAddr.m_nPort = it->m_localAddr.m_nPort;
		cntAddr.m_localAddr.m_pIpAddr.assign(it->m_localAddr.m_pIpAddr.c_str(), it->m_localAddr.m_pIpAddr.size());

		cntAddr.m_remoteAddr.m_nPort = it->m_remoteAddr.m_nPort;
		cntAddr.m_remoteAddr.m_pIpAddr.assign(it->m_remoteAddr.m_pIpAddr.c_str(), it->m_remoteAddr.m_pIpAddr.size());

		msg.m_lstConnectAddr.push_back(cntAddr);
	}

	::SendDscMessage(msg, addr);
}

int PubP2pMqAddrMsg(CP2pMqAddrMsg& rMsg)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetNodeType(rMsg.m_nAppType);
	addr.SetNodeID(rMsg.m_nAppID);
	addr.SetServiceType(MQ_ID::EN_MQ_SERVICE_TYPE);
	addr.SetServiceID(1);

	MQ_ID::CPubMsg pubMsg;

	pubMsg.m_nTopicID = rMsg.m_msg.m_nTopicID;
	pubMsg.m_msgBlob.SetBuffer((char* )rMsg.m_msg.m_pMsg);
	pubMsg.m_msgBlob.SetSize(rMsg.m_msg.m_nMsgSize);

	ACE_INT32 nRet = ::SendDscMessage(pubMsg, addr);

	pubMsg.m_msgBlob.SetBuffer(NULL);
	pubMsg.m_msgBlob.SetSize(0);

	return nRet;
}

static int PubP2pMqMsgImpl(CP2pMqMsg& rMsg, const bool bPoll)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetServiceType(MQ_ID::EN_MQ_SERVICE_TYPE);
	addr.SetServiceID(1);

	MQ_ID::CPubMsg2CallbackService pubMsg;

	pubMsg.m_nPubMsgType = bPoll ==true ? MQ_ID::CPubMsg2CallbackService::EN_POLL_TYPE : MQ_ID::CPubMsg2CallbackService::EN_BROADCAST_TYPE;
	pubMsg.m_nTopicID = rMsg.m_nTopicID;
	pubMsg.m_msgBlob.SetBuffer((char*)rMsg.m_pMsg);
	pubMsg.m_msgBlob.SetSize(rMsg.m_nMsgSize);

	ACE_INT32 nRet = ::SendDscMessage(pubMsg, addr);

	pubMsg.m_msgBlob.SetBuffer(NULL);
	pubMsg.m_msgBlob.SetSize(0);

	return nRet;
}

int PubBroadcastP2pMqMsg(CP2pMqMsg& rMsg)
{
	return PubP2pMqMsgImpl(rMsg, false);
}

int PubPollP2pMqMsg(CP2pMqMsg& rMsg)
{
	return PubP2pMqMsgImpl(rMsg, true);
}
