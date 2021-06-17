#ifndef MQ_FORWARDER_SERVICE_H_23410984236823416821368356542318792436
#define MQ_FORWARDER_SERVICE_H_23410984236823416821368356542318792436

#include "dsc/service_asynch_acceptor.h"
#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/container/dsc_hash_map.h"

#include "mq_common/mq_msg_def.h"
#include "mq_common/type_array.h"

class PLUGIN_EXPORT CMqForwarderService : public CDscHtsServerService
{
private:
	class CTopicSession;
	class CHandlerSessionItem
	{
	public:
		ACE_UINT32 GetIndex(void) const;
		void SetIndex(const ACE_UINT32 nIndex);

	public:
		ACE_UINT32 m_nClientID;
		ACE_UINT32 m_nIndex = -1;
		CTopicSession* m_pTopicSession = NULL;
		CMcpHandler* m_pMcpHandler = NULL;
	};

	class CTopicSession
	{
	public:
		CTypeArray<CHandlerSessionItem> m_HandlerSessionArray;
		ACE_UINT32 m_nTopicID = 0;

	public:
		ACE_UINT32 m_nKey = 0;
		CTopicSession* m_pPrev = NULL;
		CTopicSession* m_pNext = NULL;
	};

	class CHandlerSession
	{
	public:
		dsc_list_type(CHandlerSessionItem*) m_lstHandlerSessionItem;
		CMcpHandler* m_pMcpHandler = NULL;
		ACE_UINT32 m_nClientID = 0;

	public:
		ACE_UINT32 m_nKey = 0;
		CHandlerSession* m_pPrev = NULL;
		CHandlerSession* m_pNext = NULL;
	};
	using CClientIDSession = CHandlerSession;

public:
	enum
	{
		EN_HASH_MAP_SIZE = 16,
		EN_SERVICE_TYPE = MQ_ID::EN_MQ_FORWARDER_SERVICE_TYPE
	};

public:
	CMqForwarderService();

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);

protected:
	virtual void OnNetworkError(CMcpHandler* pMcpHandler);
	virtual void OnConnected(const CDscConnectInfo& rNetInfo);
	virtual CMcpServerHandler* AllocMcpHandler(ACE_HANDLE handle);

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(MQ_MSG::CRegMqTopicReq)
	BIND_HTS_MESSAGE(MQ_MSG::CPubMqTopicMsg)
	BIND_HTS_MESSAGE(MQ_MSG::CPubMqAddrMsg)
	BIND_HTS_MESSAGE(MQ_MSG::CPubMqBroadcastMsg)
	END_HTS_MESSAGE_BIND

public:
	ACE_INT32 OnHtsMsg(MQ_MSG::CRegMqTopicReq& rRegMqTopicReq, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CPubMqTopicMsg& rPubMqTopicMsg, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CPubMqAddrMsg& rPubMqAddrMsg, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CPubMqBroadcastMsg& rPubMqBroadcastMsg, CMcpHandler* pMcpHandler);

private:
	bool m_bDurindBroadcastMsg = false;
	ACE_INT32 m_nMqForwarderPort = 0;
	ACE_INT32 m_nTcpNodelayFlag = 0;
	ACE_INT32 m_nSocketBufSize = 0;
	CServiceAsynchAcceptor<CMqForwarderService>* m_pMqForwarderServiceAcceptor;
	CDscString m_strMqForwarderAddr;
	dsc_stack_type(CMcpHandler*) m_stackMcpHandler;

private:
	CDscHashMap<CClientIDSession, EN_HASH_MAP_SIZE> m_mapMqClientIDSession;
	CDscHashMap<CHandlerSession, EN_HASH_MAP_SIZE> m_mapHandlerSession;
	CDscHashMap<CTopicSession, EN_HASH_MAP_SIZE> m_mapTopicSession;
};

#include "mq_forwarder_service/mq_forwarder_servicee.inl"
#endif
