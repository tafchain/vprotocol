#include "idc_client.h"
#include "idc_client_service/idc_client_service.h"
#include "idc_common/idc_common_msg.h"

#include "vbh/common/vbh_comm_id_def.h"
#include "vbh/sdk/common/send_vbh_msg.h"
#include "vbh/common/vbh_encrypt_lib.h"

#include "ace/Init_ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Event.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_stat.h"

#include "dsc/mem_mng/dsc_allocator.h"
#include "dsc/dsc_app_mng.h"
#include "dsc/dsc_log.h"
#include "dsc/plugin/dsc_service_factory.h"
#include "dsc/service/dsc_service_container.h"

template<typename TYPE>
ACE_INT32 SendIdcServiceMessage(TYPE& t)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetNodeType(CDscAppManager::m_nNodeType);
	addr.SetNodeID(CDscAppManager::m_nNodeID);
	addr.SetServiceType(IDC::EN_ID_CHAIN_APP_TYPE);
	addr.SetServiceID(1);

	return VBH_SDK::SendVbhMessage(t, addr);
}

namespace IDC_SDK
{


	template <typename T>
	int TransMessage(int reqID, T& msg)
	{
		char* info = NULL;
		size_t infoLen = 0;

		DSC::Encode(msg, info, infoLen);
		if (!info)
		{
			return -1;
		}

		IDC::CIdcCommonApiCltReq req;

		req.m_nReqID = reqID;
		req.m_nType = T::EN_MSG_ID;
		req.m_info.Set(info, infoLen);

		auto ret = SendIdcServiceMessage(req);

		DSC_THREAD_SIZE_FREE(info, infoLen);

		return ret;
	}

	
	class CIdcSdkAppManager : public CDscAppManager
	{

	protected:
		virtual ACE_INT32 OnInit(void)
		{
			VBH::InitOpenSsl();
			DSC_FORWARD_CALL(CDscAppManager::OnInit());
			CDscServiceFactory<IDC_SDK::CIdcClientService> serviceFactory;
			ACE_INT32 nRet = ::RegistOneReactorDscService(&serviceFactory);
			return nRet;
		}
		virtual ACE_INT32 OnExit(void)
		{
			DSC_FORWARD_CALL(CDscAppManager::OnExit());
			return 0;
		}
	};

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
		CIdcSdkAppManager* pAppManager = NULL;

		DSC_NEW(pAppManager, CIdcSdkAppManager);

		CInitThreadParam* pParam = (CInitThreadParam*)para;

		ACE_TCHAR* argv[3] = { const_cast<char*>("blockchain_sdk"), const_cast<char*>("-h" )};

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


	///////////////////////////////////////////////////////////////////// CIdcClient
	CIdcClient::CIdcClient()
	{
	}

	CIdcClient::~CIdcClient()
	{
	}

int CIdcClient::Init(CIdcMsgCallback* callbacks)
{
	while (m_lock.test_and_set(std::memory_order_acquire));

	if (m_bInited)
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

	param.m_nAppType = IDC::EN_ID_CHAIN_APP_TYPE;
	param.m_nAppID = 0;

	//创建交易核心客户端线程
	if (-1 != ACE_Thread_Manager::instance()->spawn(ThreadFunction, &param, THR_NEW_LWP | THR_DETACHED))
	{
		param.m_event.wait();
		if (param.m_nAppID == 0)
		{
		}
		else
		{
			return -1;
		}
	}

	m_callbacks = new CIdcMsgCallback(*this);

	IDC::CInitClientSdk req;
	req.m_pCallBack = m_callbacks;

	::SendIdcServiceMessage(req);
	return 0;
}

int CIdcClient::RegisterUser(int nReqID, unsigned long long IDNum, const char* userInfo, size_t userInfoLen)
{
	IDC::CIdcRegisterUserCltSrvReq regiterUser;

	regiterUser.m_nId = IDNum;
	regiterUser.m_info.Set(const_cast<char*>(userInfo), userInfoLen);

	return TransMessage(nReqID, regiterUser);
}

int CIdcClient::Login(int reqID, unsigned long long IDNum, const char* key, size_t keyLen)
{
	IDC::CIdcLoginCltSrvReq req;
	
	req.m_nId = IDNum;
	req.m_cryptoKey.Set(const_cast<char*>(key), keyLen);

	return TransMessage(reqID, req);
}

int CIdcClient::QueryUserInfo(int reqID)
{
	IDC::CIdcQueryUserInfoCltSrvReq req;
	
	req.m_nSessionId = m_nSessionId;
	req.m_nToken = m_nToken;

	return TransMessage(reqID, req);
}

int CIdcClient::UpdateUserInfo(int reqID, const char* userInfo, size_t userInfoLen)
{
	IDC::CIdcUpdateUserInfoCltSrvReq req;

	req.m_nSessionId = m_nSessionId;
	req.m_nToken = m_nToken;
	req.m_info.Set(const_cast<char*>(userInfo), userInfoLen);

	return TransMessage(reqID, req);
}

void CIdcClient::RefreshToken()
{
	IDC::CIdcUpdateTokenCltSrvReq req;

	req.m_nSessionId = m_nSessionId;
	req.m_nToken = m_nToken;

	TransMessage(-1, req);
}

void CIdcClient::OnGetSessionId(ACE_UINT32 nSession)
{
	m_nSessionId = nSession;
}

void CIdcClient::OnRefreshToken(ACE_UINT64 nToken)
{
	m_nToken = nToken;
}


} // namespace IDC_SDK


