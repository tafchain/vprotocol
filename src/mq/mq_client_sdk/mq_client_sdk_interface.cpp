#include "ace/Init_ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Event.h"

#ifdef WIN32
#include <Shlwapi.h> 
#else
#include <unistd.h>
#endif

#include "dsc/container/dsc_string.h"
#include "dsc/dispatcher/dsc_dispatcher_center.h"
#include "dsc/dsc_log.h"

#include "mq_client_sdk/mq_client_sdk_appmanager.h"
#include "mq_client_sdk/mq_client_sdk_interface.h"
#include "mq_common/mq_msg_def.h"

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
	MQ::CInitMqClientSdkParam m_initMqClientSdkParam;
	ACE_Event m_event;
};

class CSdkMqMsgCallback : public MQ::IMqMsgCallback
{
public:
	virtual void OnReady(void);
	virtual void OnAbnormal(void);
	virtual void OnMqMsg(MQ::CAddrMqMsg& rMqMsg);
	virtual void OnExit(void);

public:
	on_ready m_pOnReady;
	on_abnormal m_pOnAbnormal;
	on_exit m_pOnExit;
	on_mq_msg m_pOnMqMsg;
};

void CSdkMqMsgCallback::OnReady(void)
{
	m_pOnReady();
}

void CSdkMqMsgCallback::OnAbnormal(void)
{
	m_pOnAbnormal();
}

void CSdkMqMsgCallback::OnMqMsg(MQ::CAddrMqMsg& rMqMsg)
{
	m_pOnMqMsg(rMqMsg.m_nClientID, rMqMsg.m_nTopicID, rMqMsg.m_pMsg, rMqMsg.m_nMsgSize);
}

void CSdkMqMsgCallback::OnExit(void)
{
	m_pOnExit();

	delete this;
}

static ACE_THR_FUNC_RETURN ThreadFunction(void* para)
{
	CThreadParam* pThreadParam = (CThreadParam*)para;
	CMqClientSdkAppManager* pMqClientSdkAppManager;

	DSC_NEW(pMqClientSdkAppManager, CMqClientSdkAppManager);

	ACE_TCHAR* argv[7] = {"mq_client_sdk", "-t", "888", "-n", "1", "-h" };
	CDscString strProgramPath;
	CDscString strNodeID;

	strNodeID = pThreadParam->m_initMqClientSdkParam.m_nClientID;
	argv[4] = (ACE_TCHAR*)(strNodeID.c_str());

	::GetProgramDir(strProgramPath);

	int iIndex = strProgramPath.find_r(DSC_FILE_PATH_SPLIT_CHAR);
	strProgramPath.TrimRight(strProgramPath.size() - iIndex);
	if (-1 == iIndex)
	{
		pThreadParam->m_event.signal();

		return 0;
	}

	strProgramPath.TrimRight(strProgramPath.size() - iIndex);
	iIndex = strProgramPath.find_r(DSC_FILE_PATH_SPLIT_CHAR);
	if (-1 == iIndex)
	{
		pThreadParam->m_event.signal();

		return 0;
	}

	strProgramPath.TrimRight(strProgramPath.size() - iIndex);//提供OCX注册路径的上层路径
	argv[6] = (ACE_TCHAR*)(strProgramPath.c_str());


	if (pMqClientSdkAppManager->Init(7, argv))
	{
		pMqClientSdkAppManager->Exit();
		DSC_DELETE(pMqClientSdkAppManager);
		pThreadParam->m_event.signal();

		return 0;
	}
	else
	{
		pMqClientSdkAppManager->InitService(pThreadParam->m_initMqClientSdkParam);
		pThreadParam->m_event.signal();
		pMqClientSdkAppManager->Run_Loop();
		DSC_DELETE(pMqClientSdkAppManager);

		return 0;
	}
}

int InitMqClientSdk(const unsigned int nClientID, unsigned int* pTopicID, const unsigned int nTopicIDNum,
	on_ready pOnReady, on_abnormal pOnAbnormal, on_exit pOnExit, on_mq_msg pOnMqMsg)
{
	CThreadParam threadParam;

	threadParam.m_initMqClientSdkParam.m_nClientID = nClientID;
	threadParam.m_initMqClientSdkParam.m_pMqMsgCallback = new CSdkMqMsgCallback;
	threadParam.m_initMqClientSdkParam.m_pTopicID = pTopicID;
	threadParam.m_initMqClientSdkParam. m_nTopicIDNum = nTopicIDNum;

	ACE::init();

	if (-1 != ACE_Thread_Manager::instance()->spawn(ThreadFunction, &threadParam, THR_NEW_LWP | THR_DETACHED))
	{
		threadParam.m_event.wait();

		return 0;
	}

	return -1;
}

int ExitMqClientSdk(void)
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
CDscMsg* GetDscMsg(TYPE& t, const CDscMsg::CDscMsgAddr& rAddress, const ACE_UINT16 nMessageType)
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
ACE_INT32 SendDscMessage(TYPE& t, const ACE_UINT16 nMessageType, const CDscMsg::CDscMsgAddr& rAddress)
{
	CDscMsg* pDscMsg = GetDscMsg(t, rAddress, nMessageType);
	if (pDscMsg)
	{
		if (::SendDscMessage(pDscMsg))
		{
			DSC_RUN_LOG_WARNING("send dsc message failed, msg type:%d", pDscMsg->GetMsgType());
			DSC::FreeDscMsg(pDscMsg);

			return -1;
		}
		else
		{
			return 0;
		}
	}

	return -1;
}

template<typename TYPE>
ACE_INT32 SendDscMessage(TYPE& t, const CDscMsg::CDscMsgAddr& rAddress)
{
	return ::SendDscMessage(t, TYPE::EN_MSG_ID, rAddress);
}

template<typename TYPE>
ACE_INT32 SendMqMessage(TYPE& t)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetServiceType(MQ_ID::EN_MQ_CLIENT_SDK_AGENT_SERVICE_TYPE);
	addr.SetServiceID(1);

	return ::SendDscMessage(t, TYPE::EN_MSG_ID, addr);
}

template<typename IF_TYPE, typename MSG_TYPE>
int PubMqMsgImpl(IF_TYPE& rReq, MSG_TYPE& rMsg)
{
	rMsg.m_nTopicID = rReq.m_nTopicID;
	rMsg.m_msgBlob.SetBuffer(rReq.m_pMsg);
	rMsg.m_msgBlob.SetSize(rReq.m_nMsgSize);

	return ::SendMqMessage(rMsg);
}

int PubTopicPollMqMsg(const unsigned int nTopicID, const char* pMsg, const unsigned int nMsgSize)
{
	MQ_MSG::CPubMqTopicMsg msg;

	msg.m_nTopicID = nTopicID;
	msg.m_msgBlob.SetBuffer(pMsg);
	msg.m_msgBlob.GetSize(nMsgSize);
	
	return ::SendMqMessage(rMsg);
}

int MQ::PubTopicBroadcastMqMsg(CTopicMqMsg& rTopicMqMsg)
{
	MQ_MSG::CPubMqBroadcastMsg msg;

	return PubMqMsgImpl(rTopicMqMsg, msg);
}

int MQ::PubAddrMqMsg(CAddrMqMsg& rAddrMqMsg)
{
	MQ_MSG::CPubMqAddrMsg msg;

	msg.m_nClientID = rAddrMqMsg.m_nClientID;

	return PubMqMsgImpl(rAddrMqMsg, msg);
}
