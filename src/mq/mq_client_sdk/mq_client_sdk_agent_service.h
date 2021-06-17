#ifndef MQ_CLIENT_SDK_AGENT_SERVICE_H_42380423897432823186325723683265
#define MQ_CLIENT_SDK_AGENT_SERVICE_H_42380423897432823186325723683265

#include "dsc/protocol/hts/dsc_hts_service.h"

#include "mq_common/mq_msg_def.h"
#include "mq_common/type_array.h"
#include "mq_client_sdk/mq_client_sdk_common_def.h"

class CMqClientSdkAgentService : public CDscHtsClientService
{
private:
	class CForwarderAddrSession
	{
	public:
		ACE_UINT32 GetIndex(void) const;
		void SetIndex(const ACE_UINT32 nIndex);

	public:
		CDscString m_strAddr;
		CMcpHandler* m_pMcpHandler = NULL;
		ACE_UINT32 m_nHandleID = 0;
		ACE_UINT32 m_nIndex = -1;
		ACE_INT16  m_nPort;
	};

public:
	enum
	{
		EN_SERVICE_TYPE = MQ_ID::EN_MQ_CLIENT_SDK_AGENT_SERVICE_TYPE
	};

public:
	CMqClientSdkAgentService(MQ::CInitMqClientSdkParam& rInitMqClientSdkParam);

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(MQ_MSG::CPubMqTopicMsg)
	DSC_BIND_MESSAGE(MQ_MSG::CPubMqAddrMsg)
	DSC_BIND_MESSAGE(MQ_MSG::CPubMqBroadcastMsg)
	END_BIND_DSC_MESSAGE

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(MQ_MSG::CRegMqClientAddrRsp)
	BIND_HTS_MESSAGE(MQ_MSG::CAddFowrarderAddrNotify)
	BIND_HTS_MESSAGE(MQ_MSG::CDelFowrarderAddrNotify)
	BIND_HTS_MESSAGE(MQ_MSG::CPubMqTopicMsg)
	BIND_HTS_MESSAGE(MQ_MSG::CPubMqAddrMsg)
	END_HTS_MESSAGE_BIND

public:
	void OnDscMsg(MQ_MSG::CPubMqTopicMsg& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(MQ_MSG::CPubMqAddrMsg& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(MQ_MSG::CPubMqBroadcastMsg& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

public:
	ACE_INT32 OnHtsMsg(MQ_MSG::CRegMqClientAddrRsp& rRegMqClientAddrRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CAddFowrarderAddrNotify& rAddFowrarderAddrNotify, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CDelFowrarderAddrNotify& rDelFowrarderAddrNotify, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CPubMqTopicMsg& rPubMqTopicMsg, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(MQ_MSG::CPubMqAddrMsg& rPubMqAddrMsg, CMcpHandler* pMcpHandler);

protected:
	virtual void OnNetworkError(CMcpHandler* pMcpHandler);
	virtual ACE_INT32 OnConnectedNodify(CMcpClientHandler* pMcpClientHandler);

protected:
	virtual CMcpClientHandler* AllocMcpHandler(ACE_HANDLE handle, const ACE_UINT32 nHandleID,
		const ACE_INT16 nMaxLostHB, PROT_COMM::CDscIpAddr& rRemoteAddr, PROT_COMM::CDscIpAddr& localAddr);

private:
	void AddMqForwarderAddr(MQ_MSG::CMqForwarderAddr& addr);

	template<typename TYPE>
	void SendDscMsg(TYPE& rMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

	template<typename TYPE>
	void OnMqMsg(TYPE& rMqMsg);

private:
	dsc_map_type(PROT_COMM::CDscIpAddr, CForwarderAddrSession*) m_mapForwarderSession;

	MQ_MSG::CRegMqTopicReq m_regMqTopicReq;
	CTypeArray<CForwarderAddrSession> m_handlerSessionArray;

	MQ::IMqMsgCallback* m_mqMsgCallback;
	CDscString m_strMqSchedulerServiceAddr;
	ACE_UINT32 m_nSchedulerHandleID = 0;
	ACE_INT32 m_nMqSchedulerServicePort = 0;
	ACE_INT32 m_nTcpNodelayFlag = 0;
	ACE_INT32 m_nSocketBufSize = 0;
	ACE_INT32 m_nConnectRefNum = 0;
	ACE_UINT32 m_nIndex = 0;
};

#include "mq_client_sdk/mq_client_sdk_agent_service.inl"
#endif
