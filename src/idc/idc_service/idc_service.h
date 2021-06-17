#ifndef _IDC_SERVICE_H_170623046521019
#define _IDC_SERVICE_H_170623046521019

#include "idc_common/idc_common_msg.h"

#include "vbh/sdk/client_sdk/client_api.h"
#include "vbh/common/vbh_block_msg_def.h"

#include "dsc/codec/codec_base/dsc_codec_base.h"
#include "dsc/dsc_msg_bind_proc_macros.h"
#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/protocol/mcp/mcp_asynch_acceptor.h"
#include "dsc/configure/dsc_configure.h"

#define BEGIN_BIND_TYPE_PROC \
	ACE_INT32 TypeProc(const ACE_UINT32 nType, ACE_UINT32 nReqID, DSC::CDscBlob& info, CMcpHandler* pMcpHandler,DSC:: CDscBlob& nonce, ACE_UINT32 nClientSessionID) \
	{ \
		switch(nType) \
		{ \

#define BIND_TYPE_PROC(PROPOSAL_TYPE) \
		case PROPOSAL_TYPE::EN_MSG_ID: \
		{ \
			PROPOSAL_TYPE prop; \
			if (DSC_UNLIKELY(DSC::Decode(prop, info.GetBuffer(), info.GetSize()))) \
			{ \
				DSC_RUN_LOG_FINE("idom server decode info error"); \
				return -1; \
			} \
			else \
			{ \
				return this->OnTypeProc(prop, nReqID,  pMcpHandler, nonce, nClientSessionID); \
			} \
		}

#define END_BIND_TYPE_PROC \
		default: \
		{ \
			DSC_RUN_LOG_FINE("cann't proc type:%d", nType); \
			return -1; \
		} \
		} \
	}

class CIdcService : public CDscHtsServerService
{
public:
	CIdcService(const CDscString& strIpAddr, const ACE_INT32 nPort, const ACE_INT32 nShareTime);
	~CIdcService();

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);

	enum
	{
		EN_SERVICE_TYPE = IDC::EN_IDC_SERVER_TYPE
	};

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(IDC::CIdcCommonCltSrvReq)
	END_HTS_MESSAGE_BIND

public:
	ACE_INT32 OnHtsMsg(IDC::CIdcCommonCltSrvReq& rCommonReq, CMcpHandler* pMcpHandler);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH::CRegistUserCltCltRsp)
	DSC_BIND_MESSAGE(VBH::CQueryUserInfoCltCltRsp)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH::CRegistUserCltCltRsp& rRegistUserRsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryUserInfoCltCltRsp& rQueryUserRsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);


private:
	enum
	{
		EN_HASH_MAP_BITES = 16,
		EN_SESSION_TIMEOUT_VALUE = 60,
		EN_CLIENT_SESSION_REFRESH_VALUE = 900
	};

	class CIdcClientSession : public CDscServiceTimerHandler
	{
	public:
		CIdcClientSession(CIdcService& idcService): m_idcService(idcService){}
		~CIdcClientSession(){}

		virtual void OnTimer(void);

	public:
		CIdcService& m_idcService;
		ACE_UINT64 m_nToken = 0;	// 随机token
		ACE_UINT64 m_nPreToken = 0;	// 上一个token
		time_t			   m_tokenTime = 0;
		ACE_UINT32 m_nSessionId;	// 随机session id
		ACE_UINT32 m_nHandleID;	// 客户端handle
		CDscString m_envelopeKey;

	public:
		ACE_UINT32 m_nKey = 0;
		CIdcClientSession* m_pPrev = NULL;
		CIdcClientSession* m_pNext = NULL;
	};

	class CBaseSession : public CDscServiceTimerHandler
	{
	public:
		CBaseSession(CIdcService& idcService, ACE_UINT32 nType): m_idcService(idcService), m_nType(nType) {}
		virtual ~CBaseSession(){}

	public:
		CIdcService& m_idcService;
		ACE_UINT32 m_nType;			// session类型
		ACE_UINT32 m_nSessionId;	// client session id
		ACE_UINT32 m_nReqID;		// 请求vbh、db
		ACE_UINT32 m_nClientReqID;	// 客户端请求id
		ACE_UINT32 m_nHandleID;	// 客户端handle
		CDscString m_nonce;	//本次请求nonce

	public:
		ACE_UINT32 m_nKey = 0;
		CBaseSession* m_pPrev = NULL;
		CBaseSession* m_pNext = NULL;
	};

	class CRegisterSession : public CBaseSession
	{
	public:
		CRegisterSession(CIdcService& idcService): CBaseSession(idcService, IDC::CIdcRegisterUserCltSrvReq::EN_MSG_ID){}
		virtual ~CRegisterSession(){}

		virtual void OnTimer(void);
	public:
		ACE_UINT64 m_nUid;
	};

	class CLoginSession : public CBaseSession
	{
	public:
		CLoginSession(CIdcService& idcService) : CBaseSession(idcService, IDC::CIdcLoginCltSrvReq::EN_MSG_ID) {}
		virtual ~CLoginSession() {}

		virtual void OnTimer(void);
	public:
		ACE_UINT64 m_nUid;
		CDscString m_cryptoKey;
	};

	class CQueryUserInfoSession : public CBaseSession
	{
	public:
		CQueryUserInfoSession(CIdcService& idcService) : CBaseSession(idcService, IDC::CIdcQueryUserInfoCltSrvReq::EN_MSG_ID) {}
		virtual ~CQueryUserInfoSession() {}

		virtual void OnTimer(void);
	public:
	};

	struct SDscAddr //在DSC系统中 地址 的 ID部分
	{
		ACE_INT16 m_nNodeID;
		ACE_INT16 m_nServiceID;
	};

	using client_session_map_type = CBareHashMap<ACE_UINT32, CIdcClientSession, EN_HASH_MAP_BITES>;
	using req_session_map_type = CBareHashMap<ACE_UINT32, CBaseSession, EN_HASH_MAP_BITES>;
	using db_dsc_addr_map_type = dsc_unordered_map_type(ACE_INT32, SDscAddr);

	BEGIN_BIND_TYPE_PROC;
	BIND_TYPE_PROC(IDC::CIdcRegisterUserCltSrvReq)
	BIND_TYPE_PROC(IDC::CIdcLoginCltSrvReq)
	BIND_TYPE_PROC(IDC::CIdcQueryUserInfoCltSrvReq)
	BIND_TYPE_PROC(IDC::CIdcUpdateUserInfoCltSrvReq)
	END_BIND_TYPE_PROC;

public:
	void OnSessionTimeOut(CBaseSession* sessoin);
	void OnClientSessionTimeout(CIdcService::CIdcClientSession* clientSession);

private:
	ACE_INT32 OnTypeProc(IDC::CIdcRegisterUserCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID);
	ACE_INT32 OnTypeProc(IDC::CIdcLoginCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID);
	ACE_INT32 OnTypeProc(IDC::CIdcQueryUserInfoCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID);
	ACE_INT32 OnTypeProc(IDC::CIdcUpdateUserInfoCltSrvReq& req, ACE_UINT32 reqID, CMcpHandler* pMcpHandler, DSC::CDscBlob& nonce, ACE_UINT32 nClientSessionID);

	ACE_INT32 LoadDbAddrMap(void);
	ACE_INT32 GetDbAddr(CDscMsg::CDscMsgAddr& addr, ACE_INT32 id = -1);

	ACE_INT32 SendClientMsg(ACE_UINT32 nReturnCode, ACE_UINT32 nType, DSC::CDscBlob &data, CDscString& envelopeKey,  CBaseSession* session);
	ACE_UINT32 NewSessionId();
	ACE_UINT64 NewSessionToken();
	ACE_UINT64 MaybeSessionTokenExpire(CIdcClientSession* client);
	CIdcClientSession* NewClientSession();
		
	CDscString m_peerEnvelopeKey;
	ACE_UINT32 m_nSessionId = 0;
	ACE_UINT32  m_nReqId = 0;

	ACE_UINT32 m_nRandSeed = 0;

	client_session_map_type m_mapClientSession;
	req_session_map_type m_mapReqSession;
	db_dsc_addr_map_type m_mapDbAddr;
	db_dsc_addr_map_type::iterator m_itCandDb;

	const ACE_INT32 m_nPort;
	const CDscString m_strIpAddr;
	const ACE_INT32 m_nShareTime;

	CMcpAsynchAcceptor<CIdcService>* m_pAcceptor = NULL;
};






#endif

