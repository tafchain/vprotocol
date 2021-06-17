#ifndef S28_CLIENT_SDK_APPMANAGER_H_7893247892346326832653
#define S28_CLIENT_SDK_APPMANAGER_H_7893247892346326832653

#include "dsc/dsc_app_mng.h"

#include "p2p_mq/mq_if_def.h"

class CMqAppMng : public CDscAppManager
{
public:
	ACE_INT32 InitService(IP2pMqMsgCallback* pP2pMqMsgCallback);

protected:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
};

#endif
