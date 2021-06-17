#ifndef CC_OBJ_SERVICE_H_234987123658132571325432
#define CC_OBJ_SERVICE_H_234987123658132571325432

#include "common/cc_service.h"

class CCcObjService : public CCcService
{
protected:
	template<typename USER_KEY_TYPE>
	ACE_INT32 GetVbhUser(const ACE_UINT32 nSessionID, const ACE_UINT32 nChannelID, USER_KEY_TYPE& rUserKey);

	template<typename USER_KEY_TYPE, typename USER_INFO_TYPE>
	ACE_INT32 DecodeUser(USER_KEY_TYPE& rUserKey, USER_INFO_TYPE& userInfo, CSimpleUser& rUser);

	template<typename USER_KEY_TYPE, typename USER_INFO_TYPE>
	ACE_INT32 SetVbhUser(const ACE_UINT32 nSessionID, const ACE_UINT32 nChannelID, USER_KEY_TYPE& rUserKey, USER_INFO_TYPE& rUserInfo);
};

#define BEGIN_BIND_PROPOSAL_PROC \
	ACE_INT32 ProposalProc(const ACE_UINT32 nSessionID, const ACE_UINT32 nAction, CDscString& proposal) \
	{ \
		switch(nAction) \
		{ \

#define BIND_PROPOSAL_PROC(PROPOSAL_TYPE) \
		case PROPOSAL_TYPE::EN_ACTION_ID: \
		{ \
			PROPOSAL_TYPE prop; \
			if (DSC_UNLIKELY(DSC::Decode(prop, proposal.c_str(), proposal.size()))) \
			{ \
				return -1; \
			} \
			else \
			{ \
				return this->OnPorposal(nSessionID, prop); \
			} \
		}

#define END_BIND_PROPOSAL_PROC \
		default: \
		{ \
			DSC_RUN_LOG_FINE("cann't proc action:%d", nAction); \
			return -1; \
		} \
		} \
	}

#include "common/cc_obj_service.inl"

#endif
