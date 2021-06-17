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
		EN_CC_REGIST_USER_SESSION_TYPE = 1, //注册用户的Session类型
		EN_CC_PROCESS_PROPOSAL_SESSION_TYPE, //处理提案的session类型
	};

	class CSimpleUser //不会在session中使用，仅用作临时变量，所以成员使用Blob类型
	{
	public:
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo;
	};

	class CSimpleInformation //不会在session中使用，仅用作临时变量，所以成员使用Blob类型
	{
	public:
		DSC::CDscBlob m_key;
		DSC::CDscBlob m_value;
	};

	class CCcUser
	{
	public:
		ACE_UINT32 m_nVersion = 0; //用户数据的版本号；0表示初始版本，每次设置+1
		VBH::COrderGenerateKey m_userKey; //保留为数据结构的 gen-user-key
		CDscString m_strUserKey; //编码成数据块的 gen-user-key
		CDscString m_userInfo;
		bool m_bSetted = false;
	};
	
	class CCcInformation
	{
	public:
		ACE_UINT32 m_nVersion = 0; //Information数据的版本号；0表示初始版本，每次设置+1
		VBH::COrderGenerateKey m_key; //保留为数据结构的 gen-information-key
		CDscString m_strKey; //编码成数据块的 gen-information-key
		CDscString m_value;
		bool m_bSetted = false;
	};

	class CCcCreatedInformation //新创建的information
	{
	public:
		CDscString m_key; //创建时设置的information-key
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
		CDscString m_userRegistInfo; //注册时携带的用户信息

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
		DSC::CDscList<CCcUser> m_lstUser; //涉及 读写 的用户列表
		DSC::CDscList<CCcInformation> m_lstInfo; //涉及 读写 的Information列表
		DSC::CDscList<CCcCreatedInformation> m_lstCreatedInfo; //涉及创建的 information 列表

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
	//回调上层实现逻辑：是否可以创建该用户，如果可以，则创建初始信息；返回非0值，表示处理失败
	//主要参数说明
	//userKey: 输入参数，用户key
	//userRegistInfo: 输入参数，注册时携带的用户信息
	virtual ACE_INT32 RegistUserProc(const ACE_UINT64 nCcSessionID, const ACE_UINT32 nChannelID, CDscString& userKey, CDscString& userRegistInfo) = 0;
	
	//回调上层实现逻辑：提案处理，返回非0值，表示处理失败
	virtual ACE_INT32 ProposalProc(const ACE_UINT64 nCcSessionID, const ACE_UINT32 nChannelID, const ACE_UINT32 nAction, CDscString& proposal) = 0;

	//回调上层实现逻辑：获得用户信息
	//用户不存在时，nReturnCode = EN_CANNOT_FOUND_USER_ERROR_TYPE
	virtual void OnGetVbhUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, CSimpleUser& rUser) = 0;

	//回调上层实现逻辑：获得信息
	virtual void OnGetVbhInfoRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, CSimpleInformation& rInfo) = 0;

protected:
	//留给上层逻辑使用： 获取用户信息 //如果有缓存的，则取缓存的信息；没有缓存信息，则从xcs取信息 
	//创建用户时，不支持调用该函数
	ACE_INT32 GetVbhUser(const ACE_UINT64 nCcSessionID, const char* pUserKey, const size_t nUserKey);

	//留给上层逻辑使用： 设置用户信息； 
	ACE_INT32 SetVbhUser(const ACE_UINT64 nCcSessionID, const char* pUserKey, const size_t nUserKey, const char* pUserInfo, const size_t nUserInfo);

	//留给上层逻辑使用： 获取信息 //如果有缓存的，则取缓存的信息；没有缓存信息，则从ics取信息
	ACE_INT32 GetVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey);

	//留给上层逻辑使用： 设置信息； 只有读取过的信息才可以设置
	ACE_INT32 SetVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey, const char* pValue, const size_t nValue);

	//留给上层逻辑使用：创建信息，创建时指定信息内容 //不支持一次创建相同内容
	ACE_INT32 CreateVbhInformation(const ACE_UINT64 nCcSessionID, const char* pKey, const size_t nKey, const char* pValue, const size_t nValue);

	//留给上层逻辑使用： 注册请求处理完毕
	ACE_INT32 RegistUserRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, const bool bAllowRegist, char* pUserInitInfo, const size_t nUserInitInfo);

	//留给上层逻辑使用： 提案处理完毕
	ACE_INT32 PorposalRsp(const ACE_INT32 nReturnCode, const ACE_UINT64 nCcSessionID, char* pTransProcessResult, const size_t nTransProcessResult);

private:
	//合成上层CC逻辑使用的SessionID
	ACE_UINT64 CombineUserCCSessionID(const ACE_UINT32 nSessionID, const ACE_UINT32 nSeesionType);

	//分解上层CC逻辑使用的SessionID
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
