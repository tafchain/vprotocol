#include "ace/OS_NS_strings.h"

#include "common/vbh_comm_func.h"
#include "common/vbh_committer_router.h"


ACE_INT32 VBH::CXCommitterServiceRouter::Init()
{
	if (LoadChannelMapXcsAddr())
	{
		DSC_RUN_LOG_ERROR("load channel map x committer serivce addr failed.");

		return -1;
	}

	return 0;
}

ACE_INT32 VBH::CXCommitterServiceRouter::GetXcsAddr(CDscMsg::CDscMsgAddr& addr, const ACE_UINT32 nChannelID)
{
	//查找用户对应的tcs地址
	auto tcsIt = m_mapChannelMapXcsAddr.find(nChannelID);

	if (tcsIt == m_mapChannelMapXcsAddr.end())
	{
		return -1;
	}

	addr.SetNodeType(VBH::EN_X_COMMITTER_APP_TYPE);
	addr.SetNodeID(tcsIt->second.m_nNodeID);
	addr.SetServiceType(VBH::EN_X_COMMITTER_SERVICE_TYPE);
	addr.SetServiceID(tcsIt->second.m_nServiceID);

	return 0;
}

class CVbhCommonXcsConfig
{
public:
	CVbhCommonXcsConfig()
		: m_nodeID("NODE_ID")
		, m_xcsID("XCS_ID")
		, m_channelID("CH_ID")
	{
	}

public:
	PER_BIND_ATTR(m_nodeID, m_xcsID, m_channelID);

public:
	CColumnWrapper< ACE_INT32 > m_nodeID;
	CColumnWrapper< ACE_INT32 > m_xcsID;
	CColumnWrapper< ACE_INT32 > m_channelID;
};

ACE_INT32 VBH::CXCommitterServiceRouter::LoadChannelMapXcsAddr()
{
	CDscDatabase database;
	CDBConnection dbConnection;
	ACE_INT32 nRet = CDscDatabaseFactoryDemon::instance()->CreateDatabase(database, dbConnection);

	if (nRet)
	{
		DSC_RUN_LOG_ERROR("connect database failed.");

		return -1;
	}

	CTableWrapper< CCollectWrapper<CVbhCommonXcsConfig> > lstCfg("XCS_CFG");

	nRet = ::PerSelect(lstCfg, database, dbConnection);
	if (nRet)
	{
		DSC_RUN_LOG_ERROR("select from XCS_CFG failed");

		return -1;
	}
	else
	{
		SDscAddr addr;
		ACE_UINT32 nChannelID;
		for (auto it = lstCfg->begin(); it != lstCfg->end(); ++it)
		{
			addr.m_nNodeID = *it->m_nodeID;
			addr.m_nServiceID = *it->m_xcsID;

			nChannelID = *it->m_channelID;

			m_mapChannelMapXcsAddr.insert(channel_map_dsc_addr_type::value_type(nChannelID, addr));
		}
	}

	return 0;
}
