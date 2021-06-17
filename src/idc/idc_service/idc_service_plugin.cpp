#include "idc/idc_service/idc_service_plugin.h"
#include "idc/idc_service/idc_service.h"

#include "dsc/service/dsc_service_container.h"
#include "dsc/db/per/persistence.h"
#include "dsc/dsc_database_factory.h"
#include "dsc/configure/dsc_configure.h"

class CIDomAddr
{
public:
	CIDomAddr()
		: m_IpAddr("IP_ADDR")
		, m_port("PORT")
		, m_nShareTime("SHARE_TIME")
	{
	}

public:
	PER_BIND_ATTR(m_IpAddr, m_port, m_nShareTime);

public:
	CColumnWrapper< CDscString > m_IpAddr;
	CColumnWrapper< ACE_INT32 > m_port;
	CColumnWrapper< ACE_INT32 > m_nShareTime;
};

class CIDomAddrCriterion : public CSelectCriterion
{
public:
	virtual void SetCriterion(CPerSelect& rPerSelect)
	{
		rPerSelect.Where(
			(rPerSelect["NODE_TYPE"] == CDscAppManager::Instance()->GetNodeType()) &&
			((rPerSelect["NODE_ID"] == CDscAppManager::Instance()->GetNodeID()))
		);
	}
};

CIDomServerServicePlugin::CIDomServerServicePlugin()
{
}

ACE_INT32 CIDomServerServicePlugin::OnInit(void)
{
	ACE_INT32 nPort = 0;
	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("IDOM_SVR_PORT", nPort))
	{
		DSC_RUN_LOG_WARNING("cann't read 'IDOM_SVR_PORT' configure item value");
		return -1;
	}

	CDscString strAddr;
	if (-1 == CDscConfigureDemon::instance()->GetDscProfileString("IDOM_SVR_ADDR", strAddr))
	{
		DSC_RUN_LOG_WARNING("cann't read 'IDOM_SVR_ADDR' configure item value");
		return -1;
	}

	ACE_INT32 nShareTime = 0;
	if (-1 == CDscConfigureDemon::instance()->GetDscProfileInt("IDOM_SHARE_TIME", nShareTime))
	{
		DSC_RUN_LOG_WARNING("cann't read 'IDOM_SHARE_TIME' configure item value");
		return -1;
	}

	CIDomServerServiceFactory idomServiceFactory;

	idomServiceFactory.m_strAddr = strAddr;
	idomServiceFactory.m_nPort = nPort;
	idomServiceFactory.m_nShareTime = nShareTime;
	if (::RegistOneReactorDscService(&idomServiceFactory))
	{
		DSC_RUN_LOG_ERROR("regist idom service failed");
	}

	return 0;
}

CIDomServerServiceFactory::CIDomServerServiceFactory()
	: m_nServiceID(0)
{
}

CDscService* CIDomServerServiceFactory::CreateDscService(void)
{
	CIdcService* pIDomService = NULL;

	DSC_NEW(pIDomService, CIdcService(m_strAddr, m_nPort, m_nShareTime));

	if (pIDomService)
	{
		pIDomService->SetType(CIdcService::EN_SERVICE_TYPE);
		pIDomService->SetID(++m_nServiceID);
	}

	return pIDomService;
}


extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CIDomServerServicePlugin* pPlugIn = NULL;

	DSC_NEW(pPlugIn, CIDomServerServicePlugin);

	return pPlugIn;
}
