#include "dsc/dsc_log.h"

#include "common/vbh_comm_msg_def.h"
#include "common/sdk_comm_if.h"
#include "common/send_vbh_msg.h"
#include "client_sdk/client_api.h"


template<typename TYPE>
ACE_INT32 SendSdkServiceMessage(TYPE& t)
{
	CDscMsg::CDscMsgAddr addr;

	addr.SetNodeType(CDscAppManager::m_nNodeType);
	addr.SetNodeID(CDscAppManager::m_nNodeID);
	addr.SetServiceType(VBH::EN_VBH_CLIENT_SERVICE_TYPE);
	addr.SetServiceID(1);

	return VBH_SDK::SendVbhMessage(t, addr);
}

ACE_INT32 VBH_SDK::InitClientSdk(const ACE_INT16 nAppID, IClientSdkMsgCallback* pCallBack)
{
	if (VBH_SDK::InitBlockChainSdk(VBH::EN_VBH_USER_INFO_SDK_APP_TYPE, nAppID))
	{
		return -1;
	}
	
	VBH::CInitBcUserSdk initBcUserSdk;

	initBcUserSdk.m_pCallBack = pCallBack;

	return ::SendSdkServiceMessage(initBcUserSdk);
}

ACE_INT32 VBH_SDK::RegistUser(CRegistUserReq& rRegistUserReq)
{
	if (rRegistUserReq.m_pUserKey && rRegistUserReq.m_nUserKeyLen > 0
		&& rRegistUserReq.m_pUserInfo && rRegistUserReq.m_nUserInfoLen > 0)
	{
		VBH::CRegistUserApiCltReq req;

		req.m_bWaitNotify = rRegistUserReq.m_bWaitNotify;
		req.m_nCcID = rRegistUserReq.m_nCcID;
		req.m_nChannelID = rRegistUserReq.m_nChannelID;
		req.m_userKey.Set(rRegistUserReq.m_pUserKey, rRegistUserReq.m_nUserKeyLen);
		req.m_userInfo.Set(rRegistUserReq.m_pUserInfo, rRegistUserReq.m_nUserInfoLen);

		return ::SendSdkServiceMessage(req);
	}
	else
	{
		return -1;
	}
}

ACE_INT32 VBH_SDK::UnRegistUser(CUnRegistUserReq& rUnRegistUserReq)
{
	//VBH::CUnRegistUserReq req;

	//req.m_nChannelID = rUnRegistUserReq.m_nChannelID;
	//req.m_userKey.Set(rUnRegistUserReq.m_pUserKey, rUnRegistUserReq.m_nUserKeyLen);

	//return ::SendSdkServiceMessage(req);
	return -1;
}

ACE_INT32 VBH_SDK::GetUserInfo(CGetUserInfoReq& rGetUserInfoReq)
{
	if (rGetUserInfoReq.m_pUserKey && rGetUserInfoReq.m_nUserKeyLen > 0 
		&& rGetUserInfoReq.m_pCryptUserKey && rGetUserInfoReq.m_nCryptUserKeyLen > 0)
	{
		VBH::CQueryUserInfoApiCltReq req;

		req.m_nChannelID = rGetUserInfoReq.m_nChannelID;
		req.m_userKey.Set(rGetUserInfoReq.m_pUserKey, rGetUserInfoReq.m_nUserKeyLen);
		req.m_cryptUserKey.Set(rGetUserInfoReq.m_pCryptUserKey, rGetUserInfoReq.m_nCryptUserKeyLen);

		return ::SendSdkServiceMessage(req);
	}
	else
	{
		return -1;
	}
}

ACE_INT32 VBH_SDK::Propose(CProposeReq& rProposeReq)
{
	if (rProposeReq.m_pUserKey && rProposeReq.m_nUserKeyLen > 0
		&& rProposeReq.m_pProposal && rProposeReq.m_nProposalLen > 0
		&& rProposeReq.m_pCryptUserKey && rProposeReq.m_nCryptUserKeyLen > 0)
	{
		VBH::CProposeApiCltReq req;

		req.m_bWaitNotify = rProposeReq.m_bWaitNotify;
		req.m_nCcID = rProposeReq.m_nCcID;
		req.m_nActionID = rProposeReq.m_nAction;
		req.m_nChannelID = rProposeReq.m_nChannelID;
		req.m_userKey.Set(rProposeReq.m_pUserKey, rProposeReq.m_nUserKeyLen);
		req.m_proposal.Set((char*)rProposeReq.m_pProposal, rProposeReq.m_nProposalLen);
		req.m_cryptUserKey.Set((char*)rProposeReq.m_pCryptUserKey, rProposeReq.m_nCryptUserKeyLen);

		return ::SendSdkServiceMessage(req);
	}
	else
	{
		return -1;
	}
}

ACE_INT32 VBH_SDK::GetTransInfo(CGetTransactionInfoReq& rGetTransInfoReq)
{
	if (rGetTransInfoReq.m_pUserKey && rGetTransInfoReq.m_nUserKeyLen > 0
		&& rGetTransInfoReq.m_pCryptUserKey && rGetTransInfoReq.m_nCryptUserKeyLen > 0
		&& rGetTransInfoReq.m_pTransKey && rGetTransInfoReq.m_nTransKeyLen > 0)
	{
		VBH::CQueryTransInfoApiCltReq req;

		req.m_nChannelID = rGetTransInfoReq.m_nChannelID;
		req.m_userKey.Set(rGetTransInfoReq.m_pUserKey, rGetTransInfoReq.m_nUserKeyLen);
		req.m_cryptUserKey.Set((char*)rGetTransInfoReq.m_pCryptUserKey, rGetTransInfoReq.m_nCryptUserKeyLen);
		req.m_transKey.Set(rGetTransInfoReq.m_pTransKey, rGetTransInfoReq.m_nTransKeyLen);

		return ::SendSdkServiceMessage(req);
	}
	else
	{
		return -1;
	}
}
