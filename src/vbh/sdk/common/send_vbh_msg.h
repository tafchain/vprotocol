#ifndef SEND_DSC_MESSAGE_H_380432894326812768432765
#define SEND_DSC_MESSAGE_H_380432894326812768432765

#include "dsc/dsc_msg.h"

#include "common/sdk_comm_export.h"

namespace VBH_SDK
{
	VBH_COMM_SDK_EXPORT ACE_INT32 SendVbhMessage(CDscMsg * pDscMsg);

	template<typename TYPE>
	CDscMsg* GetDscMsg(TYPE& t, const CDscMsg::CDscMsgAddr& rAddress, const ACE_UINT16 nMessageType);

	template<typename TYPE>
	ACE_INT32 SendVbhMessage(TYPE& t, const ACE_UINT16 nMessageType, const CDscMsg::CDscMsgAddr& rAddress);

	template<typename TYPE>
	ACE_INT32 SendVbhMessage(TYPE& t, const CDscMsg::CDscMsgAddr& rAddress);
}

#include "common/send_vbh_msg.inl"

#endif