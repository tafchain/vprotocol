#include "dsc/dispatcher/dsc_dispatcher_center.h"
#include "dsc/dsc_log.h"

#include "p2p_mq/mq_app_mng.h"
#include "p2p_mq/mq_callback_service.h"

ACE_INT32 CMqAppMng::OnInit()
{
	if (CDscAppManager::OnInit())
	{
		return -1;
	}

	return 0;
}

ACE_INT32 CMqAppMng::OnExit(void)
{

	return CDscAppManager::OnExit();
}

ACE_INT32 CMqAppMng::InitService(IP2pMqMsgCallback* pP2pMqMsgCallback)
{
	CMqCallbackService* pMqCallbackService;

	DSC_NEW(pMqCallbackService, CMqCallbackService(pP2pMqMsgCallback));

	ACE_INT32 nRet = CDscDispatcherCenterDemon::instance()->RegistDscTask(pMqCallbackService, CMqCallbackService::EN_SERVICE_TYPE, 1);
	if (nRet)
	{
		DSC_RUN_LOG_ERROR("mq callback service regist failed");
	}

	return nRet;
}
