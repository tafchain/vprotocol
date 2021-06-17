#ifndef CLIENT_API_H_432874329876128761286
#define CLIENT_API_H_432874329876128761286

#include "client_sdk/client_sdk_export.h"
#include "client_sdk/client_api_msg.h"

//TODO: 客户端提供走到CC的查询接口，包括查询用户和查询transaction
namespace VBH_SDK
{
	class IClientSdkMsgCallback
	{
	public:
		virtual void OnReady(void) = 0;
		virtual void OnAbnormal(void) = 0;
		virtual void OnExit(void) = 0;

	public:
		virtual void OnRegistUserRsp(CRegistUserRsp& rRegistUserRsp) = 0;
		virtual void OnUnRegistUserRsp(CUnRegistUserRsp& rUnRegistUserRsp) = 0;
		virtual void OnLaunchTransRsp(CProposeRsp& rSetUserInfoRsp) = 0;
		virtual void OnGetUserInfoRsp(CGetUserInfoRsp& rGetUserInfoRsp) = 0;
		virtual void OnGetTransInfoRsp(VBH_SDK::CGetTransactionInfoRsp& rGetTransInfoRsp) = 0;
	};

	//以下所有函数： return 0: succeed; return -1 error

	//初始化SDK
	VBH_CLIENT_SDK_EXPORT ACE_INT32 InitClientSdk(const ACE_INT16 nAppID, IClientSdkMsgCallback* pCallBack);

	//注册用户
	VBH_CLIENT_SDK_EXPORT ACE_INT32 RegistUser(CRegistUserReq& rRegistUserReq);

	//注销用户
	VBH_CLIENT_SDK_EXPORT ACE_INT32 UnRegistUser(CUnRegistUserReq& rUnRegistUserReq);

	//发起提案
	VBH_CLIENT_SDK_EXPORT ACE_INT32 Propose(CProposeReq& rSetUserInfoReq);

	//查询用户信息
	VBH_CLIENT_SDK_EXPORT ACE_INT32 GetUserInfo(CGetUserInfoReq& rGetUserInfoReq);

	//查询事务
	VBH_CLIENT_SDK_EXPORT ACE_INT32 GetTransInfo(CGetTransactionInfoReq& rGetTransInfoReq);
}

#endif
