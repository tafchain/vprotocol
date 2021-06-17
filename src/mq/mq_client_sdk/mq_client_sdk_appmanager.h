#ifndef MQ_CLIENT_SDK_APPMANAGER_H_7893247892346326832653
#define MQ_CLIENT_SDK_APPMANAGER_H_7893247892346326832653

#include "dsc/dsc_app_mng.h"

#include "mq_client_sdk/mq_client_sdk_common_def.h"

class CMqClientSdkAppManager : public CDscAppManager
{
public:
	ACE_INT32 InitService(MQ::CInitMqClientSdkParam& rInitMqClientSdkParam);

protected:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
};

#endif
