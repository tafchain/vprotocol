#ifndef CC_NODEJS_DEMO_SERVICE_H_3428723472314231213863217
#define CC_NODEJS_DEMO_SERVICE_H_3428723472314231213863217

#include "dsc/service/dsc_service.h"

#include "common/vbh_comm_msg_def.h"
#include "common/vbh_user_key.h"
#include "common/cc_comm_id_def.h"

class PLUGIN_EXPORT CCcNodeJsDemoService : public CDscService
{
public:
	enum
	{
		EN_SERVICE_TYPE = CC::EN_CC_NODEJS_DEMO_SERVICE_TYPE
	};

public:
	CCcNodeJsDemoService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH::CUpdateProposalReq)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH::CUpdateProposalReq& req, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
};

#include "cc_demo_service/cc_demo_service.inl"

#endif
