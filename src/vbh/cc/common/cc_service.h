#ifndef CC_SERVICE_H_34297132321723146321385479
#define CC_SERVICE_H_34297132321723146321385479

#include "dsc/service/dsc_service.h"
#include "dsc/container/bare_hash_map.h"

#include "common/vbh_comm_msg_def.h"
#include "common/vbh_committer_router.h"
#include "common/cc_comm_def_export.h"

class CC_COMM_DEF_EXPORT CCcService : public CDscService
{
protected:
	enum
	{
		EN_HASH_MAP_BITES = 16,
		EN_SESSION_TIMEOUT_VALUE = 60
	};

	enum
	{
		EN_CC_REGIST_USER_SESSION_TYPE = 1, //ע���û���Session����
		EN_CC_PROCESS_PROPOSAL_SESSION_TYPE, //�����᰸��session����
	};

	class CSimpleUser //������session��ʹ�ã���������ʱ���������Գ�Աʹ��Blob����
	{
	public:
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo;
	};

	class CSimpleInformation //������session��ʹ�ã���������ʱ���������Գ�Աʹ��Blob����
	{
	public:
		DSC::CDscBlob m_key;
		DSC::CDscBlob m_value;
	};

	class CCcUser
	{
	public:
		ACE_UINT32 m_nVersion = 0; //�û����ݵİ汾�ţ�0��ʾ��ʼ�汾��ÿ������+1
		VBH::COrderGenerateKey m_userKey; //����Ϊ���ݽṹ�� gen-user-key
		CDscString m_strUserKey; //��������ݿ�� gen-user-key
		CDscString m_userInfo;
		bool m_bSetted = false;
	};
	
	class CCcInformation
	{
	public:
		ACE_UINT32 m_nVersion = 0; //Information���ݵİ汾�ţ�0��ʾ��ʼ�汾��ÿ������+1
		VBH::COrderGenerateKey m_key; //����Ϊ���ݽṹ�� gen-information-key
		CDscString m_strKey; //��������ݿ�� gen-information-key
		CDscString m_value;
		bool m_bSetted = false;
	};

	class CCcCreatedInformation //�´�����information
	{
	public:
		CDscString m_key; //����ʱ���õ�information-key
		CDscString m_value;
	};

private:
	class CSession : public CDscServiceTimerHandler
	{
	public:
		CSession(CCcService& rCcService);

	public:
		ACE_UINT32 m_nCcSessionID;
		ACE_UINT32 m_nEsSessionID;
		ACE_UINT32 m_nChannelID;
		CDscMsg::CDscMsgAddr m_esAddr;

	protected:
		CCcService& m_rCcService;
	};

	class CRegistUserSession : public CSession
	{
	public:
		CRegistUserSession(CCcService& rCcService);

	public:
		virtual void OnTimer(void) override;

	public:
		CDscString m_userKey;
		CDscString m_userRegistInfo; //ע��ʱЯ�����û���Ϣ

	public:
		ACE_UINT32 m_nKey = 0;
		CRegistUserSession* m_pPrev = NULL;
		CRegistUserSession* m_pNext = NULL;
	};

	class CProcessProposalSession : public CSession
	{
	public:
		CProcessProposalSession(CCcService& rCcService);

	public:
		virtual void OnTimer(void) override;

	public:
		CDscString m_proposal;
		DSC::CDscList<CCcUser> m_lstUser; //�漰 ��д ���û��б�
		DSC::CDscList<CCcInformation> m_lstInfo; //�漰 ��д ��Information�б�
		DSC::CDscList<CCcCreatedInformation> m_lstCreatedInfo; //�漰������ information �б�

	public:
		ACE_UINT32 m_nKey = 0;
		CProcessProposalSession* m_pPrev = NULL;
		CProcessProposalSession* m_pNext = NULL;
	};

public:
	CCcService();

public:
	virtual ACE_INT32 OnInit(void) override;
	virtual ACE_INT32 OnExit(void) override;

protected:
	BEGIN_BIND_DSC_MESSAGE
	DSC_BIND_MESSAGE(VBH::CRegistUserEsCcReq)
	DSC_BIND_MESSAGE(VBH::CSubmitProposalEsCcReq)
	DSC_BIND_MESSAGE(VBH::CQueryUserInfoXcsCcRsp)
	DSC_BIND_MESSAGE(VBH::CQueryInformationXcsCcRsp)
	END_BIND_DSC_MESSAGE

public:
	void OnDscMsg(VBH::CRegistUserEsCcReq& rRegistUserReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CSubmitProposalEsCcReq& rSubmitProposeReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryUserInfoXcsCcRsp rsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
	void OnDscMsg(VBH::CQueryInformationXcsCcRsp rsp, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

public:
	void OnTimeOut(CRegistUserSession* pRegistUserSession);
	void OnTimeOut(CProcessProposalSession* pProcessProposalSession);

protected:
	//�ص��ϲ�ʵ���߼����Ƿ���Դ������û���������ԣ��򴴽���ʼ��Ϣ�����ط�0ֵ����ʾ����ʧ��
	//��Ҫ����˵��
	//userKey: ����������û�key
	//userRegistInfo: ���������ע��ʱЯ�����û���Ϣ
	virtual ACE_INT32 RegistUserProc(const ACE_UINT64 nCcSessionID, const ACE_UINT32 nChannelID, CDscString& userKey, CDscString& userRegistInfo) = 0;
	
	//�ص��ϲ�ʵ���߼����᰸�������ط�0ֵ����ʾ����ʧ��
	virtual ACE_INT32 ProposalProc(const ACE_UINT64 nCcSessionID, const ACE_UINT32 nChannelID, const ACE_UINT32 nAction, CDscString& proposal) = 0;

	//�ص��ϲ�ʵ���߼�������û���Ϣ
	//�û�������ʱ��nReturnCode = EN_CANNOT_FOUND_USER_ERROR_TYPE
	virtual void OnGetVbhUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, CSimpleUser& rUser) = 0;

	//�ص��ϲ�ʵ���߼��������Ϣ
	virtual void OnGetVbhInfoRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, CSimpleInformation& rInfo) = 0;

protected:
	//�����ϲ��߼�ʹ�ã� ��ȡ�û���Ϣ //����л���ģ���ȡ�������Ϣ��û�л�����Ϣ�����xcsȡ��Ϣ 
	//�����û�ʱ����֧�ֵ��øú���
	ACE_INT32 GetVbhUser(const ACE_UINT64 nCcSessionID, const char* pUserKey, const size_t nUserKey);

	//�����ϲ��߼�ʹ�ã� �����û���Ϣ�� 
	ACE_INT32 SetVbhUser(const ACE_UINT64 nCcSessionID, const char* pUserKey, const size_t nUserKey, const char* pUserInfo, const size_t nUserInfo);

	//�����ϲ��߼�ʹ�ã� ��ȡ��Ϣ //����л���ģ���ȡ�������Ϣ��û�л�����Ϣ�����icsȡ��Ϣ
	ACE_INT32 GetVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey);

	//�����ϲ��߼�ʹ�ã� ������Ϣ�� ֻ�ж�ȡ������Ϣ�ſ�������
	ACE_INT32 SetVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey, const char* pValue, const size_t nValue);

	//�����ϲ��߼�ʹ�ã�������Ϣ������ʱָ����Ϣ���� //��֧��һ�δ�����ͬ����
	ACE_INT32 CreateVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey, const char* pValue, const size_t nValue);

	//�����ϲ��߼�ʹ�ã� ע�����������
	ACE_INT32 RegistUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, const bool bAllowRegist, char* pUserInitInfo, const size_t nUserInitInfo);

	//�����ϲ��߼�ʹ�ã� �᰸�������
	ACE_INT32 PorposalRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, char* pTransProcessResult, const size_t nTransProcessResult);

private:
	//�ϳ��ϲ�CC�߼�ʹ�õ�SessionID
	ACE_UINT64 CombineUserCCSessionID(const ACE_UINT32 nSessionID, const ACE_UINT32 nSeesionType);

	//�ֽ��ϲ�CC�߼�ʹ�õ�SessionID
	void SplitUserCCSessionID(ACE_UINT32& nSessionID, ACE_UINT32& nSessionType, const ACE_UINT64 nCcSessinID);

private:
	using regist_user_session_map_type = CBareHashMap<ACE_UINT32, CRegistUserSession, EN_HASH_MAP_BITES>;
	using process_proposal_session_map_type = CBareHashMap<ACE_UINT32, CProcessProposalSession, EN_HASH_MAP_BITES>;

private:
	ACE_UINT32 m_nSessionID = 0;
	regist_user_session_map_type m_mapRegistUserSession;
	process_proposal_session_map_type m_mapProcessProposalSession;
	VBH::CXCommitterServiceRouter m_xcsRouter;
};

#include "common/cc_service.inl"

#endif
