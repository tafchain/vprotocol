#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "common/send_vbh_msg.h"

ACE_INT32 VBH_SDK::SendVbhMessage(CDscMsg* pDscMsg)
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

