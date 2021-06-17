#include "client_sdk/client_api.h"
#include "client_c_sdk/client_c_api.h"

class CWrapperCallback : public VBH_SDK::IClientSdkMsgCallback
{
public:
	virtual void OnReady(void);
	virtual void OnAbnormal(void);
	virtual void OnExit(void);

public:
	virtual void OnRegistUserRsp(VBH_SDK::CRegistUserRsp& rRegistUserRsp) override;
	virtual void OnUnRegistUserRsp(VBH_SDK::CUnRegistUserRsp& rUnRegistUserRsp) override;
	virtual void OnLaunchTransRsp(VBH_SDK::CProposeRsp& rSetUserInfoRsp) override;
	virtual void OnGetUserInfoRsp(VBH_SDK::CGetUserInfoRsp& rGetUserInfoRsp) override;
	virtual void OnGetTransInfoRsp(VBH_SDK::CGetTransactionInfoRsp& rGetTransInfoRsp) override;

public:
	on_vbh_ready m_pOnReady;
	on_vbh_abnormal m_pOnAbnormal;
	on_vbh_exit m_pOnExit;

	on_regist_vbh_user_rsp m_pOnRegistUserRsp;
	on_unregist_vbh_user_rsp m_pOnUnRegistUserRsp;
	on_query_vbh_user_rsp m_pOnQueryUserRsp;
	on_vbh_propose_rsp m_pOnProposeRsp;
	on_query_vbh_trans_rsp m_pOnGetBcTransRsp;
};

int init_vbh_c_sdk(const short int nAppID, 
	on_vbh_ready pOnReady, 
	on_vbh_abnormal pOnAbnormal, 
	on_vbh_exit pOnExit, 
	on_regist_vbh_user_rsp pOnRegistUserRsp,
	on_unregist_vbh_user_rsp pOnUnRegistUserRsp,
	on_query_vbh_user_rsp pOnQueryUserRsp, 
	on_vbh_propose_rsp pOnProposeRsp, 
	on_query_vbh_trans_rsp pOnGetBcTransRsp)
{
	if (pOnReady && pOnAbnormal && pOnExit && pOnRegistUserRsp && pOnUnRegistUserRsp && pOnQueryUserRsp && pOnProposeRsp && pOnGetBcTransRsp)
	{
		CWrapperCallback* pWrapperCallback = new CWrapperCallback;

		pWrapperCallback->m_pOnReady = pOnReady;
		pWrapperCallback->m_pOnAbnormal = pOnAbnormal;
		pWrapperCallback->m_pOnExit = pOnExit;

		pWrapperCallback->m_pOnRegistUserRsp = pOnRegistUserRsp;
		pWrapperCallback->m_pOnUnRegistUserRsp = pOnUnRegistUserRsp;
		pWrapperCallback->m_pOnQueryUserRsp = pOnQueryUserRsp;
		pWrapperCallback->m_pOnProposeRsp = pOnProposeRsp;
		pWrapperCallback->m_pOnGetBcTransRsp = pOnGetBcTransRsp;

		return InitClientSdk(nAppID, pWrapperCallback);
	}
	else
	{
		return -1;
	}
}


int reg_vbh_user(const unsigned int nChannelID, 
	char* pUserKey, const unsigned int nUserKeyLen,
	char* pUserInfo, const unsigned int nUserInfoLen)
{
	if (pUserKey && nUserKeyLen > 0 
		&& pUserInfo && nUserInfoLen > 0)
	{
		VBH_SDK::CRegistUserReq req;

		req.m_nChannelID = nChannelID;
		req.m_pUserKey = pUserKey;
		req.m_nUserKeyLen = nUserKeyLen;
		req.m_pUserInfo = pUserInfo;
		req.m_nUserInfoLen = nUserInfoLen;

		return VBH_SDK::RegistUser(req);
	}
	else
	{
		return -1;
	}
}

int unreg_vbh_user(const unsigned int nChannelID, char* pUserKey, const unsigned int nUserKeyLen)
{
	if (pUserKey && nUserKeyLen > 0)
	{
		VBH_SDK::CUnRegistUserReq req;

		req.m_nChannelID = nChannelID;
		req.m_pUserKey = pUserKey;
		req.m_nUserKeyLen = nUserKeyLen;

		return VBH_SDK::UnRegistUser(req);
	}
	else
	{
		return -1;
	}
}

int query_vbh_user(const unsigned int nChannelID, 
	char* pUserKey, const unsigned int nUserKeyLen, 
	char* pCryptUserKey, const unsigned int nCryptUserKeyLen)
{
	if (pUserKey && nUserKeyLen > 0
		&& pCryptUserKey && nCryptUserKeyLen > 0)
	{
		VBH_SDK::CGetUserInfoReq req;

		req.m_nChannelID = nChannelID;
		req.m_pUserKey = pUserKey;
		req.m_nUserKeyLen = nUserKeyLen;

		req.m_pCryptUserKey = pCryptUserKey;
		req.m_nCryptUserKeyLen = nCryptUserKeyLen;

		return VBH_SDK::GetUserInfo(req);
	}
	else
	{
		return -1;
	}
}

int vbh_propose(const short int nCcID, const unsigned int nAction, const unsigned int nChannelID, 
	char* pUserKey, const unsigned int nUserKeyLen,
	char* pProposal, const unsigned int nProposalLen, 
	char* pCryptUserKey, const unsigned int nCryptUserKeyLen)
{
	if (pUserKey && nUserKeyLen > 0
		&& pProposal && nProposalLen > 0
		&& pCryptUserKey && nCryptUserKeyLen > 0)
	{
		VBH_SDK::CProposeReq req;

		req.m_nCcID = nCcID;
		req.m_nAction = nAction;
		req.m_nChannelID = nChannelID;

		req.m_pUserKey = pUserKey;
		req.m_nUserKeyLen = nUserKeyLen;

		req.m_pProposal = pProposal;
		req.m_nProposalLen = nProposalLen;

		req.m_pCryptUserKey = pCryptUserKey;
		req.m_nCryptUserKeyLen = nCryptUserKeyLen;

		return VBH_SDK::Propose(req);
	}
	else
	{
		return -1;
	}
}

int query_vbh_trans(int nChannelID, 
	char* pUserKey, const unsigned int nUserKeyLen, 
	char* pCryptUserKey, int nCryptUserKeyLen, 
	char* pTransKey, int nTransKeyLen)
{
	if (pUserKey && nUserKeyLen > 0
		&& pCryptUserKey && nCryptUserKeyLen > 0
		&& pTransKey && nTransKeyLen > 0)
	{
		VBH_SDK::CGetTransactionInfoReq req;

		req.m_nChannelID = nChannelID;
		req.m_pUserKey = pUserKey;
		req.m_nUserKeyLen = nUserKeyLen;

		req.m_pCryptUserKey = pCryptUserKey;
		req.m_nCryptUserKeyLen = nCryptUserKeyLen;

		req.m_pTransKey = pTransKey;
		req.m_nTransKeyLen = nTransKeyLen;

		return VBH_SDK::GetTransInfo(req);
	}
	else
	{
		return -1;
	}
}


void CWrapperCallback::OnReady(void)
{
	this->m_pOnReady();
}

void CWrapperCallback::OnAbnormal(void)
{
	this->m_pOnAbnormal();
}

void CWrapperCallback::OnExit(void)
{
	this->m_pOnExit();
	delete this;
}

void CWrapperCallback::OnRegistUserRsp(VBH_SDK::CRegistUserRsp& rRegistUserRsp)
{
	this->m_pOnRegistUserRsp(rRegistUserRsp.m_nReturnCode, rRegistUserRsp.m_nChannelID,
		rRegistUserRsp.m_pUserKey, rRegistUserRsp.m_nUserKeyLen,
		rRegistUserRsp.m_pCryptUserKey, rRegistUserRsp.m_nCryptUserKeyLen);
}

void CWrapperCallback::OnUnRegistUserRsp(VBH_SDK::CUnRegistUserRsp& rUnRegistUserRsp)
{
	this->m_pOnUnRegistUserRsp(rUnRegistUserRsp.m_nReturnCode, rUnRegistUserRsp.m_nChannelID,
		rUnRegistUserRsp.m_pUserKey, rUnRegistUserRsp.m_nUserKeyLen);
}

void CWrapperCallback::OnGetUserInfoRsp(VBH_SDK::CGetUserInfoRsp& rGetUserInfoRsp)
{
	this->m_pOnQueryUserRsp(rGetUserInfoRsp.m_nReturnCode, rGetUserInfoRsp.m_nChannelID, rGetUserInfoRsp.m_pUserKey, rGetUserInfoRsp.m_nUserKeyLen
		, rGetUserInfoRsp.m_pUserUserInfo, rGetUserInfoRsp.m_nUserUserInfoLen);
}

void CWrapperCallback::OnGetTransInfoRsp(VBH_SDK::CGetTransactionInfoRsp& rGetTransInfoRsp)
{
	this->m_pOnGetBcTransRsp(rGetTransInfoRsp.m_nReturnCode, rGetTransInfoRsp.m_pTransKey, rGetTransInfoRsp.m_nTransKeyLen,
		rGetTransInfoRsp.m_pTransInfo, rGetTransInfoRsp.m_nTransInfoLen);
}

void CWrapperCallback::OnLaunchTransRsp(VBH_SDK::CProposeRsp& rSetUserInfoRsp)
{
	this->m_pOnProposeRsp(rSetUserInfoRsp.m_nReturnCode, rSetUserInfoRsp.m_nChannelID, 
		rSetUserInfoRsp.m_pUserKey, rSetUserInfoRsp.m_nUserKeyLen,
		rSetUserInfoRsp.m_pTransKey, rSetUserInfoRsp.m_nTransKeyLen);
}
