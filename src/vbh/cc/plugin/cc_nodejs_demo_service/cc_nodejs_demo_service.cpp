#include "cc_nodejs_demo_service/cc_nodejs_demo_service.h"
#include "common/vbh_comm_error_code.h"
#include "bc/vbh_test/vbh_test_comm/vbh_user_info_def.h"

CCcNodeJsDemoService::CCcNodeJsDemoService()
{
}

ACE_INT32 CCcNodeJsDemoService::OnInit(void)
{
	if(CDscService::OnInit())
	{
		DSC_RUN_LOG_ERROR("cc demo service init failed!");

		return -1;
	}

	DSC_RUN_LOG_INFO("cc demo service init succeed!");

	return 0;
}

ACE_INT32 CCcNodeJsDemoService::OnExit(void)
{
	return CDscService::OnExit();
}

void CCcNodeJsDemoService::OnDscMsg(VBH::CUpdateProposalReq& req, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	//1、收消息处理
	VBH::CUpdateProposalRsp rsp;

	rsp.m_nReturnCode = 0;
	rsp.m_nSessionID = req.m_nSessionID;
	rsp.m_userInfo = req.m_userOriInfo;

	this->SendDscMessage(rsp, rSrcMsgAddr);
}
