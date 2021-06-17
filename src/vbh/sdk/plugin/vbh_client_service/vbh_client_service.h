#ifndef VBH_CLIENT_SERVICE_H_4238972341423121345621356
#define VBH_CLIENT_SERVICE_H_4238972341423121345621356

#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"
#include "dsc/container/bare_hash_map.h"

#include "common/vbh_comm_msg_def.h"

class PLUGIN_EXPORT CVbhClientService : public CDscHtsClientService
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
		ACE_UINT32 m_nIndex; //��Ҫ�����ӽ���ʱΪindex��һ�������ʼֵ���Ա�֤���пͻ�����������ӵ�ĳ��peer������endorser.
	};

	class CVbhPeerConnectionSummaryInfo
	{
	public:
		CVbhPeerConnectionInfo* m_parrPeerConnInfo = NULL;
		ACE_UINT32 m_nPeerSessionNum = 0;
		ACE_UINT32 m_nIndex = 0;
	};

private:
	//ͨ�û���session 
	class IBaseSession : public CDscServiceTimerHandler
	{
	public:
		IBaseSession(CVbhClientService& rRegistUserService);

	public:
		virtual void OnNetError(void) = 0; //�������ʱ���ͷű�session�ĺ���

	public:
		CDscMsg::CDscMsgAddr m_srcMsgAddr;
		ACE_UINT32 m_nCltSessionID; //�ͻ���sessionID
		ACE_UINT32 m_nSrcSessionID; //����ҵ�����󷽵�sessionID

	public:
		ACE_UINT32 m_nIndex = 0; //ʹ�� CDscTypeArray ��������߱��Ľӿ�

	protected:
		CVbhClientService& m_rClientBaseService;
	};

	class CHandleSession  //����ͨ�ž����session
	{
	public:
		CDscTypeArray<IBaseSession> m_arrSession;
		CMcpHandler* m_pMcpHandler = NULL;

	public:
		ACE_UINT32 m_nIndex = 0; //ʹ�� CDscTypeArray ��������߱��Ľӿ�

	public:
		ACE_UINT32 m_nKey = 0;
		CHandleSession* m_pPrev = NULL;
		CHandleSession* m_pNext = NULL;
	};

	//ע���û���session
	class CRegistUserSession : public IBaseSession
	{
	public:
		CRegistUserSession(CVbhClientService& rClientBaseService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CDscTypeArray<CHandleSession> m_arrHandleSession; //�ͻ��˺Ͷ������ڵ������

	public:
		bool m_bWaitNotify; //�û��Ƿ�ȴ�֪ͨ
		ACE_UINT32 m_nChannelID;
		ACE_UINT32 m_nSucceedRspNum = 0; //����ڵ��гɹ�Ӧ�����Ŀ
		CHandleSession* m_pSubmitEsMcpHandleSession = NULL; //��ѡ�е�endorser service ��handle-session
		ACE_UINT32 m_nEsSessionID; //����endorser service �� session 

		CDscString m_ccGenerateUserInfo; //CC����ע��ʱ��Ϊ�û����ɵĳ�ʼ��Ϣ//�Ƿ�����ע�᲻�ý��б��飬ֻҪ��1������ڵ㲻����ע�ᣬ��ע��ʧ��

	public:
		ACE_UINT32 m_nKey = 0;
		CRegistUserSession* m_pPrev = NULL;
		CRegistUserSession* m_pNext = NULL;
	};

	//ע���û��ȴ�notify��session
	class CRegistUserWaitNotitySession : public IBaseSession
	{
	public:
		CRegistUserWaitNotitySession(CVbhClientService& rClientBaseService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CHandleSession* m_pHandleSession; //�ȴ�Ӧ��ʱ��������

	public:
		ACE_UINT32 m_nKey = 0;
		CRegistUserWaitNotitySession* m_pPrev = NULL;
		CRegistUserWaitNotitySession* m_pNext = NULL;
	};

	//ע���û���session
	class CUnRegistUserSession : public IBaseSession
	{
	public:
		CUnRegistUserSession(CVbhClientService& rClientBaseService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		ACE_UINT64 m_nUserSystemIndex;
		CDscString m_envelopeKey; //�Գ���Կ

	public:
		ACE_UINT32 m_nKey = 0;
		CUnRegistUserSession* m_pPrev = NULL;
		CUnRegistUserSession* m_pNext = NULL;
	};

	//�����᰸���׵�session
	class CProposeSession : public IBaseSession
	{
	public:
		CProposeSession(CVbhClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CDscTypeArray<CHandleSession> m_arrHandleSession; //�ͻ��˺Ͷ������ڵ������

	public:
		bool m_bWaitNotify; //�û��Ƿ�ȴ�֪ͨ
		ACE_UINT32 m_nSucceedRspNum = 0; //����ڵ��гɹ�Ӧ�����Ŀ
		CHandleSession* m_pSubmitEsMcpHandleSession = NULL; //��ѡ�е�endorser service ��handle-session
		ACE_UINT32 m_nEsSessionID; //����endorser service �� session 
		CDscString m_transContent; //transaction - content

	public:
		ACE_UINT32 m_nKey = 0;
		CProposeSession* m_pPrev = NULL;
		CProposeSession* m_pNext = NULL;
	};

	//�����᰸�ȴ�notify��session
	class CProposeWaitNotifySession : public IBaseSession
	{
	public:
		CProposeWaitNotifySession(CVbhClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CHandleSession* m_pHandleSession; //�ȴ�Ӧ��ʱ��������

	public:
		ACE_UINT32 m_nKey = 0;
		CProposeWaitNotifySession* m_pPrev = NULL;
		CProposeWaitNotifySession* m_pNext = NULL;
	};

	//��ѯ�û���Ϣ��session
	class CQueryUserSession : public IBaseSession
	{
	public:
		CQueryUserSession(CVbhClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CHandleSession* m_pHandleSession; //��ѯʱ��������

	public:
		ACE_UINT32 m_nKey = 0;
		CQueryUserSession* m_pPrev = NULL;
		CQueryUserSession* m_pNext = NULL;
	};

	//��ѯ�����session
	class CQueryTransactionSession : public IBaseSession
	{
	public:
		CQueryTransactionSession(CVbhClientService& rRegistUserService);

	public:
		virtual void OnTimer(void) override;
		virtual void OnNetError(void) override;

	public:
		CHandleSession* m_pHandleSession;

	public:
		ACE_UINT32 m_nKey = 0;
		CQueryTransactionSession* m_pPrev = NULL;
		CQueryTransactionSession* m_pNext = NULL;
	};

public:
	enum
	{
		EN_SERVICE_TYPE = VBH::EN_VBH_CLIENT_SERVICE_TYPE
	};

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(VBH::CRegistUserEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CSubmitRegistTransactionEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CSubscribeRegistTransactionEsCltNotify)
	BIND_HTS_MESSAGE(VBH::CUnRegistUserRsp)
	BIND_HTS_MESSAGE(VBH::CSubmitProposalEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CSubmitProposalTransactionEsCltRsp)
	BIND_HTS_MESSAGE(VBH::CSubscribeProposeTransactionEsCltNotify)
	BIND_HTS_MESSAGE(VBH::CQueryUserInfoEdsCltRsp)
	BIND_HTS_MESSAGE(VBH::CQueryTransInfoEsCltRsp)
	END_HTS_MESSAGE_BIND

public:
	ACE_INT32 OnHtsMsg(VBH::CRegistUserEsCltRsp& rRegistUserRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CSubmitRegistTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CSubscribeRegistTransactionEsCltNotify& rNotify, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CUnRegistUserRsp& rUnRegistUserRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CSubmitProposalEsCltRsp& rSubmitProposalRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CSubmitProposalTransactionEsCltRsp& rSubmitTransRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CSubscribeProposeTransactionEsCltNotify& rNotify, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CQueryUserInfoEdsCltRsp& rQueryUserRsp, CMcpHandler* pMcpHandler);
	ACE_INT32 OnHtsMsg(VBH::CQueryTransInfoEsCltRsp& rQueryTransRsp, CMcpHandler* pMcpHandler);

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH::CRegistUserCltCltReq)
	DSC_BIND_MESSAGE(VBH::CUnRegistUserReq)
	DSC_BIND_MESSAGE(VBH::CProposeCltCltReq)
	DSC_BIND_MESSAGE(VBH::CQueryUserInfoCltCltReq)
	DSC_BIND_MESSAGE(VBH::CQueryTransInfoCltCltReq)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH::CRegistUserCltCltReq& rRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CUnRegistUserReq& rUnRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CProposeCltCltReq& rProposeReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryUserInfoCltCltReq& rQueryUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryTransInfoCltCltReq& rGetBcTransInfoReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

public:
	//�ͷ��ж�����ӵ�session�� (ע���û�session�� �᰸session)
	template <typename SESSION_TYPE>
	void OnReleaseMultiHandleSession(SESSION_TYPE* pSession);

	//�ͷ�ֻ��1�����ӵ�session��(��ѯsession�� wait-notify-session)
	template <typename SESSION_TYPE>
	void OnReleaseSingleHanleSession(SESSION_TYPE* pSession);

	void OnResponse(CRegistUserSession* pRegistUserSession, const ACE_INT32 nRetrunCode, const bool bAllowRegist = false,
		CDscString& strCryptUserKey = CDscString(), CDscString& strGenUserKey = CDscString());
	void OnTimeOut(CRegistUserSession* pRegistUserSession);
	void OnNetError(CRegistUserSession* pRegistUserSession);

	void OnResponse(CRegistUserWaitNotitySession* pNotifySession, const ACE_INT32 nRetrunCode, const bool bSuccess = false);
	void OnTimeOut(CRegistUserWaitNotitySession* pNotifySession);
	void OnNetError(CRegistUserWaitNotitySession* pNotifySession);

	void OnResponse(CProposeSession* pProposeSession, const ACE_INT32 nRetrunCode, const char* pTransKey = nullptr, const size_t nTranKeyLen = 0);
	void OnTimeOut(CProposeSession* pProposeSession);
	void OnNetError(CProposeSession* pProposeSession);

	void OnResponse(CProposeWaitNotifySession* pNotifySession, const ACE_INT32 nRetrunCode, const bool bSuccess = false);
	void OnTimeOut(CProposeWaitNotifySession* pNotifySession);
	void OnNetError(CProposeWaitNotifySession* pNotifySession);

	void OnResponse(CQueryUserSession* pQueryUserSession, const ACE_INT32 nRetrunCode, DSC::CDscBlob& userInfo = DSC::CDscBlob());
	void OnTimeOut(CQueryUserSession* pQueryUserSession);
	void OnNetError(CQueryUserSession* pQueryUserSession);

	void OnResponse(CQueryTransactionSession* pQueryTransSession, const ACE_INT32 nRetrunCode, DSC::CDscBlob& transInfo = DSC::CDscBlob());
	void OnTimeOut(CQueryTransactionSession* pQueryTransSession);
	void OnNetError(CQueryTransactionSession* pQueryTransSession);

protected:
	virtual ACE_INT32 OnConnectedNodify(CMcpClientHandler* pMcpClientHandler);
	virtual void OnNetworkError(CMcpHandler* pMcpHandler);

private:
	ACE_INT32 GetPeerID(const ACE_UINT32 nHandleID);
	CMcpHandler* GetMcpHandler(void);
	ACE_INT32 LoadPeerInfo(void);

private:
	using handle_session_map_type = CBareHashMap<ACE_UINT32, CHandleSession, EN_HASH_MAP_BITES>; //handleID -> handlesession
	//ע���û���session-map
	using regist_user_session_map_type = CBareHashMap<ACE_UINT32, CRegistUserSession, EN_HASH_MAP_BITES>;
	using regist_user_wait_notify_session_map_type = CBareHashMap<ACE_UINT32, CRegistUserWaitNotitySession, EN_HASH_MAP_BITES>;
	//ע���û���session-map
	using unregist_user_session_map_type = CBareHashMap<ACE_UINT32, CUnRegistUserSession, EN_HASH_MAP_BITES>;
	//�᰸/���������session-map�������᰸�����û�����
	using propose_session_map_type = CBareHashMap<ACE_UINT32, CProposeSession, EN_HASH_MAP_BITES>;
	using propose_wait_notify_session_map_type = CBareHashMap<ACE_UINT32, CProposeWaitNotifySession, EN_HASH_MAP_BITES>;
	//��ѯ��ص�session-map
	using query_user_map_type = CBareHashMap<ACE_UINT32, CQueryUserSession, EN_HASH_MAP_BITES>;
	using query_transaction_map_type = CBareHashMap<ACE_UINT32, CQueryTransactionSession, EN_HASH_MAP_BITES>;

private:
	handle_session_map_type m_mapHandleSession;
	regist_user_session_map_type m_mapRegistUserSession;
	regist_user_wait_notify_session_map_type m_mapRegistUserWaitNotifySession;
	unregist_user_session_map_type m_mapUnRegistUserSession;
	propose_session_map_type m_mapProposeSession;
	propose_wait_notify_session_map_type m_mapProposeWaitNotifySession;
	query_user_map_type m_mapQueryUserSession;
	query_transaction_map_type m_mapQueryTransSession;
	DSC::CDscList <CVbhPeer> m_lstBcPeer;

	CVbhPeerConnectionSummaryInfo m_vbhPeerConnSumInfo; //������peer��������Ϣ����
	ACE_UINT32 m_seed = 0;
	ACE_UINT32 m_nSessionID = 0;//��һ�������session id
	CDscString m_peerPublicKey; //peer�˵�public key //TODO: ���������Գ���Կʱ��������
	CDscString m_peerEnvelopeKey; //SDK �� endorserͨ��ʱʹ�õĶԳ���Կ��TODO: ���Ϊ��ʱ���ӵı�����������sdk��endorser����Կ����ͨ����ͨ��ͻ��ӵ�
	ACE_INT32 m_nEndorsePeerCount = 0; //�ύ����ڵ�ĸ�������ǰʹ����򵥵ı������; TODO: ���ڰ汾Ҫ������
};

#include "vbh_client_service/vbh_client_service.inl"

#endif
