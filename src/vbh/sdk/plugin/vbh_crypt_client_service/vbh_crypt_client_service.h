#ifndef VBH_CRYPT_CLIENT_SERVICE_H_432988766832867432
#define VBH_CRYPT_CLIENT_SERVICE_H_432988766832867432

#include "openssl/ec.h"

#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"
#include "dsc/container/bare_hash_map.h"

#include "common/vbh_comm_msg_def.h"

#include "client_sdk/client_api.h"

class PLUGIN_EXPORT CVbhCryptClientService : public CDscHtsClientService
{
private:
	enum
	{
		EN_HASH_MAP_BITES = 16,
		EN_SESSION_TIMEOUT_VALUE = 60,
		EN_SESSION_WAIT_NOTIFY_TIMEOUT_VALUE = 300
	};

private:
	class CEndorserAddr
	{
	public:
		CDscString m_strIpAddr;
		ACE_INT32 m_nPort = 0;
		ACE_UINT32 m_nHandleID = 0;
	};

	class CVbhPeer
	{
	public:
		ACE_UINT16 m_nPeerID = 0;
		DSC::CDscList <CEndorserAddr> m_lstEndorserAddr;
	};

	using CMcpHandlerPtr = CMcpHandler *;

	class CVbhPeerConnectionInfo
	{
	public:
		CMcpHandlerPtr* m_ppMcpHandler = NULL;
		ACE_UINT16 m_nPeerID = 0;
		ACE_UINT32 m_nHandleIDNum = 0;
		ACE_UINT32 m_nIndex; //需要在连接建立时为index赋一个随机初始值；以保证所有客户端能随机连接到某个peer的所有endorser.
	};

	class CVbhPeerConnectionSummaryInfo
	{
	public:
		CVbhPeerConnectionInfo* m_parrPeerConnInfo = NULL;
		ACE_UINT32 m_nPeerSessionNum = 0;
		ACE_UINT32 m_nIndex = 0;
	};

private:
	//通用基础session 
	class IBaseSession : public CDscServiceTimerHandler
	{
	public:
		IBaseSession(CVbhCryptClientService& rRegistUserService);

	public:
		virtual void OnNetError(void) = 0; //网络出错时，释放本session的函数

	public:
		ACE_UINT32 m_nCltSessionID; //客户端sessionID
		ACE_UINT32 m_nChannelID;

		CDscString m_strNonce;  //通信随机数

	public:
		ACE_UINT32 m_nIndex = 0; //使用 CDscTypeArray 容器必须具备的接口

	protected:
		CVbhCryptClientService& m_rClientBaseService;
	};

	class CHandleSession  //管理通信句柄的session
	{
	public:
		CDscTypeArray<IBaseSession> m_arrSession;
		CMcpHandler* m_pMcpHandler = NULL;

	public:
		ACE_UINT32 m_nIndex = 0; //使用 CDscTypeArray 容器必须具备的接口

	public:
		ACE_UINT32 m_nKey = 0;
		CHandleSession* m_pPrev = NULL;
		CHandleSession* m_pNext = NULL;
	};

	//注册用户的session
	class CRegistUserSession : public IBaseSession
	{
	public:
		CRegistUserSession(CVbhCryptClientService& rClientBaseService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CDscTypeArray<CHandleSession> m_arrHandleSession; //客户端和多个背书节点的连接

	public:
		bool m_bWaitNotify; //用户是否等待通知
		bool m_bCompleteRegistRsp = false; //标记是否已经完成了注册应答 //如果完成注册应答后，session还存在，则是在等待注册通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nSucceedRspNum = 0; //背书节点中成功应答的数目
		CHandleSession* m_pSubmitEsMcpHandleSession = NULL; //被选中的endorser service 的handle-session
		ACE_UINT32 m_nEsSessionID; //保存endorser service 的 session 

		CDscString m_userKey;  //注册时提交的user-key
		CDscString m_userInfo; //注册时提交的用户信息
		CDscString m_envelopeKey; //对称加密秘钥 //TODO: 将来交换了对称秘钥后，该字段有用

		CDscString m_ccGenerateUserInfo; //CC允许注册时，为用户生成的初始信息//是否允许注册不用进行背书，只要有1个背书节点不允许注册，则注册失败
		CDscString m_encodedClientCryptKey; //编码后的客户端秘钥组

	public:
		ACE_UINT32 m_nKey = 0;
		CRegistUserSession* m_pPrev = NULL;
		CRegistUserSession* m_pNext = NULL;
	};

	//注销用户的session
	class CUnRegistUserSession : public IBaseSession
	{
	public:
		CUnRegistUserSession(CVbhCryptClientService& rClientBaseService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		ACE_UINT64 m_nUserSystemIndex;
		CDscString m_envelopeKey; //对称秘钥

	public:
		ACE_UINT32 m_nKey = 0;
		CUnRegistUserSession* m_pPrev = NULL;
		CUnRegistUserSession* m_pNext = NULL;
	};

	//发起提案交易的session
	class CProposeSession : public IBaseSession
	{
	public:
		CProposeSession(CVbhCryptClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CDscTypeArray<CHandleSession> m_arrHandleSession; //客户端和多个背书节点的连接

	public:
		bool m_bWaitNotify; //用户是否等待通知
		bool m_bCompleteRegistRsp = false; //标记是否已经完成了注册应答 //如果完成注册应答后，session还存在，则是在等待注册通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nActionID;

		ACE_UINT32 m_nSucceedRspNum = 0; //背书节点中成功应答的数目
		CHandleSession* m_pSubmitEsMcpHandleSession = NULL; //被选中的endorser service 的handle-session
		ACE_UINT32 m_nEsSessionID; //保存endorser service 的 session 

		CDscString m_userKey; //用户key，注册用户时，系统为用户生成的，可以解码为 VB::COrderGenerateKey 的 user-key
		CDscString m_envelopeKey; //对称秘钥

		CDscString m_transContent; //transaction - content

	public:
		ACE_UINT32 m_nKey = 0;
		CProposeSession* m_pPrev = NULL;
		CProposeSession* m_pNext = NULL;
	};

	//查询用户信息的session
	class CQueryUserSession : public IBaseSession
	{
	public:
		CQueryUserSession(CVbhCryptClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CHandleSession* m_pHandleSession; //查询时所用连接

	public:
		CDscString m_userKey;
		CDscString m_envelopeKey; //对称秘钥

	public:
		ACE_UINT32 m_nKey = 0;
		CQueryUserSession* m_pPrev = NULL;
		CQueryUserSession* m_pNext = NULL;
	};

	//查询事务的session
	class CQueryTransactionSession : public IBaseSession
	{
	public:
		CQueryTransactionSession(CVbhCryptClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CHandleSession* m_pHandleSession;

	public:
		ACE_UINT64 m_nUserSystemIndex;
		CDscString m_envelopeKey; //对称秘钥
		CDscString m_userKey;
		CDscString m_transKey;

	public:
		ACE_UINT32 m_nKey = 0;
		CQueryTransactionSession* m_pPrev = NULL;
		CQueryTransactionSession* m_pNext = NULL;
	};

public:
	enum
	{
		EN_SERVICE_TYPE = VBH::EN_VBH_CRYPT_CLIENT_SERVICE_TYPE
	};

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(VBH::CCryptRegistUserEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CCryptSubmitRegistTransactionEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CCryptUnRegistUserRsp)
	BIND_HTS_MESSAGE(VBH::CCryptSubmitProposalEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CCryptSubmitProposalTransactionEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CCryptQueryUserInfoEdsCltRsp)
	BIND_HTS_MESSAGE(VBH::CCryptQueryTransInfoEsCltRsp)
	END_HTS_MESSAGE_BIND

public:
	ACE_INT32 OnHtsMsg(VBH::CCryptRegistUserEsCltRsp& rRegistUserRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CCryptSubmitRegistTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CCryptUnRegistUserRsp& rUnRegistUserRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CCryptSubmitProposalEsCltRsp& rSubmitProposalRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CCryptSubmitProposalTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CCryptQueryUserInfoEdsCltRsp& rQueryUserRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CCryptQueryTransInfoEsCltRsp& rQueryTransRsp, CMcpHandler* pMcpHandler);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH::CInitBcUserSdk)
	DSC_BIND_MESSAGE(VBH::CRegistUserApiCltReq)
	DSC_BIND_MESSAGE(VBH::CCryptUnRegistUserReq)
	DSC_BIND_MESSAGE(VBH::CProposeApiCltReq)
	DSC_BIND_MESSAGE(VBH::CQueryUserInfoApiCltReq)
	DSC_BIND_MESSAGE(VBH::CQueryTransInfoApiCltReq)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH::CInitBcUserSdk& rInitUserSdk, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CRegistUserApiCltReq& rRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CCryptUnRegistUserReq& rUnRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CProposeApiCltReq& rLaunchBcTransApiCltReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryUserInfoApiCltReq& rGetBcUserInfoReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryTransInfoApiCltReq& rGetBcTransInfoReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

public:
	void OnRegistUserResponse(ACE_INT32 nReturnCode, CRegistUserSession* pRegistUserSession, DSC::CDscBlob& genUserKey);
	void OnRelease(CRegistUserSession* pRegistUserSession);
	void OnTimeOut(CRegistUserSession* pRegistUserSession);
	void OnNetError(CRegistUserSession* pRegistUserSession);

	//nOrderID 打包事务时的orderID //nSequenceNumber 打包事务时的流水号 //nTransIdx 事务在区块中的序号 //nBlockID 事务所在区块的区块ID
	void OnProposeResponse(const ACE_INT32 nReturnCode, CProposeSession* pProposeSession, const ACE_UINT16 nOrderID, const ACE_UINT32 nSequenceNumber,
		const ACE_UINT32 nTransIdx, const ACE_UINT64 nBlockID, DSC::CDscShortList<VBH::CConciseOnCreateInformationKey>& lstCreatedInfoKey);
	void OnRelease(CProposeSession* pProposeSession);
	void OnTimeOut(CProposeSession* pProposeSession);
	void OnNetError(CProposeSession* pProposeSession);

	void OnRelease(CQueryUserSession* pQueryUserSession, ACE_INT32 nReturnCode, DSC::CDscBlob& userInfo);
	void OnTimeOut(CQueryUserSession* pQueryUserSession);
	void OnNetError(CQueryUserSession* pQueryUserSession);

	void OnRelease(CQueryTransactionSession* pQueryTransSession, ACE_INT32 nReturnCode, DSC::CDscBlob& transInfo);
	void OnTimeOut(CQueryTransactionSession* pQueryTransSession);
	void OnNetError(CQueryTransactionSession* pQueryTransSession);

protected:
	virtual ACE_INT32 OnConnectedNodify(CMcpClientHandler* pMcpClientHandler);
	virtual void OnNetworkError(CMcpHandler* pMcpHandler);

private:
	ACE_INT32 RegistUser(VBH::CRegistUserApiCltReq& rRegistUserReq);
	ACE_INT32 UnRegistUser(VBH::CCryptUnRegistUserReq& rUnRegistUserReq);
	ACE_INT32 Propose(VBH::CProposeApiCltReq& rProposeReq);
	ACE_INT32 GetUserInfo(VBH::CQueryUserInfoApiCltReq& rQueryUserReq);
	ACE_INT32 GetTransInfo(VBH::CQueryTransInfoApiCltReq& rQueryTransReq);

private:
	ACE_INT32 GetPeerID(const ACE_UINT32 nHandleID);
	CMcpHandler* GetMcpHandler(void);
	ACE_INT32 LoadPeerInfo(void);

private:
	void OnReady(void);
	void OnAbnormal(void);
	void OnRegistUserRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& cryptKey, DSC::CDscBlob& genUserKey);
	void OnUnRegistUserRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey);
	void OnProposeRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& transKey);
	void OnGetUserInfoRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& userInfo);
	void OnGetTransInfoRsp(const ACE_INT32 nReturnCode, ACE_INT32 nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& transKey, DSC::CDscBlob& transInfo);

private:
	using handle_session_map_type = CBareHashMap<ACE_UINT32, CHandleSession, EN_HASH_MAP_BITES>; //handleID -> handlesession
	//注册用户的session-map
	using regist_user_session_map_type = CBareHashMap<ACE_UINT32, CRegistUserSession, EN_HASH_MAP_BITES>;
	//注销用户的session-map
	using unregist_user_session_map_type = CBareHashMap<ACE_UINT32, CUnRegistUserSession, EN_HASH_MAP_BITES>;
	//提案/发起事务的session-map，发起提案不受用户限制
	using propose_session_map_type = CBareHashMap<ACE_UINT32, CProposeSession, EN_HASH_MAP_BITES>;

	//查询相关的session-map
	using query_user_map_type = CBareHashMap<ACE_UINT32, CQueryUserSession, EN_HASH_MAP_BITES>;
	using query_transaction_map_type = CBareHashMap<ACE_UINT32, CQueryTransactionSession, EN_HASH_MAP_BITES>;

private:
	handle_session_map_type m_mapHandleSession;
	regist_user_session_map_type m_mapRegistUserSession;
	unregist_user_session_map_type m_mapUnRegistUserSession;
	propose_session_map_type m_mapProposeSession;
	query_user_map_type m_mapQueryUserSession;
	query_transaction_map_type m_mapQueryTransSession;
	DSC::CDscList <CVbhPeer> m_lstBcPeer;

	CVbhPeerConnectionSummaryInfo m_vbhPeerConnSumInfo; //和所有peer的连接信息汇总
	ACE_UINT32 m_seed = 0;
	ACE_UINT32 m_nSessionID = 0;//下一个分配的session id
	CDscString m_peerPublicKey; //peer端的public key //TODO: 将来交换对称秘钥时可能有用
	CDscString m_peerEnvelopeKey; //SDK 和 endorser通信时使用的对称秘钥；TODO: 这个为临时增加的变量，待将来sdk和endorser的秘钥交换通道打通后就会扔掉
	ACE_INT32 m_nEndorsePeerCount = 0; //提交背书节点的个数，当前使用最简单的背书策略; TODO: 后期版本要调整。

private:
	VBH_SDK::IClientSdkMsgCallback* m_pCallback = NULL;
	bool m_bReady = false;
};

#include "vbh_crypt_client_service/vbh_crypt_client_service.inl"

#endif
