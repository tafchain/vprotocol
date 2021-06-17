#include "dsc/dsc_comm_def.h"

#include "chain_code_appmanager.h"
#include "common/vbh_comm_func.h"
#include "common/vbh_encrypt_lib.h"

#ifdef DSC_TEST
#include "bc/vbh_kernel/plugin/vbh_endorser_service/vbh_endorser_service_plugin.h"
#include "bc/vbh_kernel/plugin/vbh_order_service/vbh_order_service_plugin.h"
#include "bc/vbh_kernel/plugin/vbh_committer_service/vbh_committer_service_plugin.h"
#include "bc/vbh_kernel/plugin/vbh_anchor_service/vbh_anchor_service_plugin.h"
#include "bc/vbh_cc/cc_demo_service/cc_demo_service_plugin.h"
#include "bc/vbh_cc/cc_json_demo_service/cc_json_demo_service_plugin.h"
#endif

ACE_INT32 CChainCodeAppManager::OnInit()
{
	VBH::InitOpenSsl();
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

#ifdef DSC_TEST
	//·½±ãvaldring¼ì²â£¬¾²Ì¬¼ÓÔØ²å¼þ
	COrderChannelManagerServicePlugin endorserServicePlugin;
	CBlockchainSchedulerServicePlugin blockchainOrderServicePlugin;
	CBlockchainCommitterServicePlugin blockchainCommitterServicePlugin;
	CBlockchainAnchorServicePlugin blockchainAnchorServicePlugin;
	CCcDemoServicePlugin ccDemoServicePlugin;
	CCcJsonDemoServicePlugin ccJsonDemoServicePlugin;

	endorserServicePlugin.OnInit();
	blockchainOrderServicePlugin.OnInit();
	blockchainCommitterServicePlugin.OnInit();
	blockchainAnchorServicePlugin.OnInit();
	ccDemoServicePlugin.OnInit();
	ccJsonDemoServicePlugin.OnInit();
#endif

	return 0;
}

ACE_INT32 CChainCodeAppManager::OnExit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnExit());

	return 0;
}


