#ifndef CLIENT_API_H_432874329876128761286
#define CLIENT_API_H_432874329876128761286

#include "client_sdk/client_sdk_export.h"
#include "client_sdk/client_api_msg.h"

//TODO: �ͻ����ṩ�ߵ�CC�Ĳ�ѯ�ӿڣ�������ѯ�û��Ͳ�ѯtransaction
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

	//�������к����� return 0: succeed; return -1 error

	//��ʼ��SDK
	VBH_CLIENT_SDK_EXPORT ACE_INT32 InitClientSdk(const ACE_INT16 nAppID, IClientSdkMsgCallback* pCallBack);

	//ע���û�
	VBH_CLIENT_SDK_EXPORT ACE_INT32 RegistUser(CRegistUserReq& rRegistUserReq);

	//ע���û�
	VBH_CLIENT_SDK_EXPORT ACE_INT32 UnRegistUser(CUnRegistUserReq& rUnRegistUserReq);

	//�����᰸
	VBH_CLIENT_SDK_EXPORT ACE_INT32 Propose(CProposeReq& rSetUserInfoReq);

	//��ѯ�û���Ϣ
	VBH_CLIENT_SDK_EXPORT ACE_INT32 GetUserInfo(CGetUserInfoReq& rGetUserInfoReq);

	//��ѯ����
	VBH_CLIENT_SDK_EXPORT ACE_INT32 GetTransInfo(CGetTransactionInfoReq& rGetTransInfoReq);
}

#endif
