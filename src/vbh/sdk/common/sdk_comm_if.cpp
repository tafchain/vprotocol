#include <atomic>

#include "ace/Init_ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Event.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_stat.h"

#include "dsc/mem_mng/dsc_allocator.h"
#include "dsc/dsc_app_mng.h"
#include "dsc/dsc_log.h"

#include "common/sdk_comm_if.h"
#include "common/sdk_appmanager.h"
#include "common/vbh_comm_error_code.h"

class CInitThreadParam
{
public:
	ACE_INT16 m_nAppType;
	ACE_INT16 m_nAppID;
	ACE_Event m_event;
};
//交易核心客户端线程入口函数
ACE_THR_FUNC_RETURN ThreadFunction(void* para)
{
	InitThreadAllocator();
	CSdkAppManager* pAppManager = NULL;

	DSC_NEW(pAppManager, CSdkAppManager);

	CInitThreadParam* pParam = (CInitThreadParam* )para;

	ACE_TCHAR* argv[3] = {"blockchain_sdk", "-h" };

	pAppManager->SetNodeType(pParam->m_nAppType);
	pAppManager->SetNodeID(pParam->m_nAppID);
	argv[2] = getenv("WORK_ROOT");
	if (!argv[2])
	{
		ACE_OS::printf("Please check the setting of 'WORK_ROOT'\n");
		DSC_DELETE(pAppManager);

		pParam->m_nAppID = -1;
		pParam->m_event.signal();

		return 0;
	}

	if (pAppManager->Init(3, argv))
	{
		ACE_OS::printf("app init failed please check log\n");
		pAppManager->Exit();
		DSC_DELETE(pAppManager);

		pParam->m_nAppID = -1;
		pParam->m_event.signal();

		return 0;
	}

	pParam->m_event.signal();
	pAppManager->Run_Loop();
	
	DSC_DELETE(pAppManager);

	DeleteThreadAllocator();

	return 0;
}

std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
bool m_bInited = false;

ACE_INT32 VBH_SDK::InitBlockChainSdk(const ACE_INT16 nAppType, const ACE_INT16 nAppID)
{
	while (m_lock.test_and_set(std::memory_order_acquire));

	if(m_bInited)
	{//重复调用
		m_lock.clear(std::memory_order_release);
		
		return 0;
	}
	else
	{
		m_bInited = true;
		m_lock.clear(std::memory_order_release);
	}

	InitThreadAllocator();
	ACE::init();

	CInitThreadParam param;

	param.m_nAppType = nAppType;
	param.m_nAppID = nAppID;

	//创建交易核心客户端线程
	if (-1 != ACE_Thread_Manager::instance()->spawn(ThreadFunction, &param, THR_NEW_LWP | THR_DETACHED))
	{
		param.m_event.wait();
		if (param.m_nAppID == nAppID)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

ACE_INT32 VBH_SDK::FnitBlockChainSdk(void)
{
	CDscAppManager::Instance()->SetAppExited();

	return 0;
}

const char* VBH_SDK::GetBcSdkErrorString(const ACE_INT32 nErrorCode)
{
	return VBH::GetErrorString(nErrorCode);
}

ACE_INT32 VBH_SDK::WriteCryptKeyToFile(const char* pstrPathname, const char* pCryptKey, const size_t nCryptKeyLen)
{
	ACE_HANDLE hFileHandle = ACE_OS::open(pstrPathname, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC);

	if (ACE_INVALID_HANDLE == hFileHandle)
	{
		ACE_INT32 nLastError = ACE_OS::last_error();

		DSC_RUN_LOG_ERROR("cann't open file:%s, last error code:%d, last error reason:%s.", pstrPathname, nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}

	if (-1 == ACE_OS::write(hFileHandle, DSC_STRING_TYPE_PARAM(DEF_DEGIN_VBH_CORE_KEY)))
	{
		ACE_INT32 nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("write crypt file failed, file name:%s, last error code:%d, last error reason:%s.", 
			DEF_DEGIN_VBH_CORE_KEY, nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(hFileHandle);

		return -1;
	}

	if (-1 == ACE_OS::write(hFileHandle, pCryptKey, nCryptKeyLen))
	{
		ACE_INT32 nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("write crypt file failed, file name:%s, last error code:%d, last error reason:%s.",
			DEF_DEGIN_VBH_CORE_KEY, nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(hFileHandle);

		return -1;
	}

	if (-1 == ACE_OS::write(hFileHandle, DSC_STRING_TYPE_PARAM(DEF_END_VBH_CORE_KEY)))
	{
		ACE_INT32 nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("write crypt file failed, file name:%s, last error code:%d, last error reason:%s.",
			DEF_DEGIN_VBH_CORE_KEY, nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(hFileHandle);

		return -1;
	}

	ACE_OS::close(hFileHandle);

	return 0;
}

ACE_INT32 VBH_SDK::ReadyCryptKeyToFile(const char* pstrPathname, char*& pCryptKey, size_t& nCryptKeyLen)
{
	ACE_stat fileStat;
	ACE_INT32 nFileState = ACE_OS::stat(pstrPathname, &fileStat);

	if (-1 == nFileState)
	{
		DSC_RUN_LOG_ERROR("crypt file:%s not exist", pstrPathname);

		return -1;
	}

	nCryptKeyLen = fileStat.st_size;

	if (nCryptKeyLen <= DSC_STRING_LENGTH(DEF_DEGIN_VBH_CORE_KEY) + DSC_STRING_LENGTH(DEF_END_VBH_CORE_KEY))
	{
		DSC_RUN_LOG_ERROR("crypt file:%s invalide", pstrPathname);

		return -1;
	}
	
	ACE_HANDLE hFileHandle = ACE_OS::open(pstrPathname, O_RDONLY | O_BINARY);

	if (ACE_INVALID_HANDLE == hFileHandle)
	{
		ACE_INT32 nLastError = ACE_OS::last_error();

		DSC_RUN_LOG_ERROR("cann't open file:%s, last error code:%d, last error reason:%s.", pstrPathname, nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}

	pCryptKey = (char*)malloc(nCryptKeyLen);

	if (!pCryptKey)
	{
		ACE_OS::close(hFileHandle);

		return -1;
	}

	if (-1 == ACE_OS::read(hFileHandle, pCryptKey, nCryptKeyLen))
	{
		ACE_INT32 nLastError = ACE_OS::last_error();

		DSC_RUN_LOG_ERROR("read file:%s head failed, last error code:%d, last error reason:%s.", pstrPathname, nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(hFileHandle);
		free(pCryptKey);

		return -1;
	}

	if (::memcmp(pCryptKey, DEF_DEGIN_VBH_CORE_KEY, DSC_STRING_LENGTH(DEF_DEGIN_VBH_CORE_KEY)) ||
		::memcmp(pCryptKey + nCryptKeyLen - DSC_STRING_LENGTH(DEF_END_VBH_CORE_KEY), DEF_END_VBH_CORE_KEY, DSC_STRING_LENGTH(DEF_END_VBH_CORE_KEY)))
	{
		DSC_RUN_LOG_ERROR("crypt file:%s invalide", pstrPathname);
		ACE_OS::close(hFileHandle);
		free(pCryptKey);

		return -1;
	}

	nCryptKeyLen -= DSC_STRING_LENGTH(DEF_DEGIN_VBH_CORE_KEY) + DSC_STRING_LENGTH(DEF_END_VBH_CORE_KEY);
	ACE_OS::memmove(pCryptKey, pCryptKey + DSC_STRING_LENGTH(DEF_DEGIN_VBH_CORE_KEY), nCryptKeyLen);

	ACE_OS::close(hFileHandle);

	return 0;
}
