#ifndef MQ_SCHEDULER_SERVICE_H_432974231973428563428562314973248634267234856
#define MQ_SCHEDULER_SERVICE_H_432974231973428563428562314973248634267234856

#include "dsc/service_asynch_acceptor.h"
#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/mem_mng/dsc_stl_type.h"

#include "mq_common/mq_msg_def.h"

class PLUGIN_EXPORT CMqSchedulerService : public CDscHtsServerService
{
private:
	class CForwarderAddrSession
	{
	public:
		bool operator< (const CForwarderAddrSession& rForwarderAddrSession) const;

	public:
		ACE_INT16 m_nNodeType;
		ACE_INT16 m_nNodeID;
		ACE_INT16 m_nPort;
		CDscString m_strAddr;
	};

public:
	enum
	{
		EN_SERVICE_TYPE = MQ_ID::EN_MQ_SCHEDULER_SERVICE_TYPE
	};

public:
	CMqSchedulerService();

public:
	ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(MQ_MSG::CRegistMqForwarderServiceReq)
	END_BIND_DSC_MESSAGE

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(MQ_MSG::CRegMqClientAddrReq)
	END_HTS_MESSAGE_BIND

public:
	void OnDscMsg(MQ_MSG::CRegistMqForwarderServiceReq& rRegistMqForwarderServiceReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

public:
	ACE_INT32 OnHtsMsg(MQ_MSG::CRegMqClientAddrReq& rRegMqClientAddrReq, const ACE_UINT32 nHandleID);

protected:
	virtual void OnNetworkError(CMcpHandler* pMcpHandler);
	virtual void OnDisConnected(const CDscConnectInfo& rNetInfo);

private:
	ACE_UINT32 AllocClientID(void);

private:
	CServiceAsynchAcceptor<CMqSchedulerService>* m_pMqSchedulerServiceAcceptor;
	dsc_set_type(CForwarderAddrSession) m_setForwarderAddr;
	dsc_set_type(ACE_UINT32) m_setMqClientHandlerID;
	ACE_UINT32 m_nClientID;
};

#include "mq_scheduler_service/mq_scheduler_service.inl"
#endif
