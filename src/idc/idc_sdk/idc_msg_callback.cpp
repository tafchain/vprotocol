#include "idc/idc_sdk/idc_msg_callback.h"

#include "idc/idc_sdk/idc_client.h"

using namespace IDC_SDK;

void CIdcMsgCallback::OnCommonRsp(ACE_INT32 nReqID, ACE_INT32 nReturnCode, ACE_INT32 nType, DSC::CDscBlob &data)
{
	if (nReturnCode)
	{
		switch (nType)
		{
		case IDC::EN_IDC_REGISTER_USER_MSG:
			OnRegisterUserRsp(nReqID, nReturnCode, 0, 0);
			break;
		case IDC::EN_IDC_LOGIN_MSG:
			OnLoginRsp(nReqID, nReturnCode);
			break;
		case IDC::EN_IDC_QUERY_USER_INFO_MSG:
			OnQueryUserInfoRsp(nReqID, nReturnCode, 0,0);
			break;

		default:
			break;
		}

		return;
	}

	TypeProc(nType, nReqID, data);
}

void CIdcMsgCallback::OnTypeProc(ACE_UINT32 nReqID, IDC::CIdcRegisterUserSrvCltRsp& rsp)
{
	OnRegisterUserRsp((int)nReqID, 0, rsp.m_cryptoKey.GetBuffer(), rsp.m_cryptoKey.GetSize());
}

void CIdcMsgCallback::OnTypeProc(ACE_UINT32 nReqID, IDC::CIdcLoginSrvCltRsp& rsp)
{
	m_client.OnGetSessionId(rsp.m_nSessionId);
	m_client.OnRefreshToken(rsp.m_nToken);
	OnLoginRsp((int)nReqID, 0);
}

void CIdcMsgCallback::OnTypeProc(ACE_UINT32 nReqID, IDC::CIdcQueryUserInfoSrvCltRsp& rsp)
{
	OnQueryUserInfoRsp((int)nReqID, 0, rsp.m_info.GetBuffer(), rsp.m_info.GetSize());
}


