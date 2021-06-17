#ifndef __VBH_COMMMON_WRAPPER_MESSAGE_DEF_H
#define __VBH_COMMMON_WRAPPER_MESSAGE_DEF_H

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/container/dsc_array.h"

#include "common/vbh_comm_def_export.h"
#include "common/vbh_comm_msg_def.h"

namespace VBH
{
	//====================注册用户流程======================
	class CCryptRegistUserCltEsReqDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptRegistUserCltEsReqDataWrapper(bool& bWaitNotify, bool& bSubmitNode, ACE_INT16& nCcID, ACE_UINT32& nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& userInfo, DSC::CDscBlob& nonce);

	public:
		DSC_BIND_ATTR(m_bSubmitNode, m_nCcID, m_nChannelID, m_userKey, m_userInfo);

	public:
		bool& m_bWaitNotify; //客户端是否等待最终通知
		bool& m_bSubmitNode; //标记是否被选中的
		ACE_INT16& m_nCcID;
		ACE_UINT32& m_nChannelID; //用户所属channel
		DSC::CDscBlob& m_userKey; //用户key
		DSC::CDscBlob& m_userInfo; //用户信息
		DSC::CDscBlob& m_nonce; //通信用随机数
	};

	class CCryptRegistUserEsCltRspDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptRegistUserEsCltRspDataWrapper(bool& bAllowRegist, DSC::CDscBlob& userInitInfo, DSC::CDscBlob& nonce, DSC::CDscBlob& svrPubCryptKey, DSC::CDscBlob& cltPriCryptKey, DSC::CDscBlob& envelopeKey);

	public:
		DSC_BIND_ATTR(m_bAllowRegist, m_userInitInfo, m_nonce, m_svrPubCryptKey, m_cltPriCryptKey, m_envelopeKey);

	public:
		bool& m_bAllowRegist; //是否允许注册
		DSC::CDscBlob& m_userInitInfo; //允许注册时的初始信息
		DSC::CDscBlob& m_nonce; //通信用随机数
		DSC::CDscBlob& m_svrPubCryptKey;
		DSC::CDscBlob& m_cltPriCryptKey;
		DSC::CDscBlob& m_envelopeKey; //客户端要存储的秘钥组
	};

	//====================查询用户流程======================
	class CCryptQueryUserInfoRspWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptQueryUserInfoRspWrapper(DSC::CDscBlob& userInfo, DSC::CDscBlob& nonce);

	public:
		DSC_BIND_ATTR(m_userInfo, m_nonce);

	public:
		DSC::CDscBlob& m_userInfo;
		DSC::CDscBlob& m_nonce;
	};

	//====================发起提案流程======================
	class CCryptSubmitProposalCltEsReqDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptSubmitProposalCltEsReqDataWrapper(bool& bWaitNotify, bool& bUpdateNode, ACE_INT16& nCcID, ACE_UINT32& nAction, DSC::CDscBlob& proposal, DSC::CDscBlob& nonce);

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_bSubmitNode, m_nCcID, m_nAction, m_proposal, m_nonce);

	public:
		bool& m_bWaitNotify; //是否等待最终事务通知
		bool& m_bSubmitNode; //通知endorser保留session，用于后续commit transaction
		ACE_INT16& m_nCcID;
		ACE_UINT32& m_nAction;
		DSC::CDscBlob& m_proposal;
		DSC::CDscBlob& m_nonce;
	};

	class CServerCryptKeyWrapper //服务端存放秘钥数据的wrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CServerCryptKeyWrapper(DSC::CDscBlob& cltPubKey, DSC::CDscBlob& svrPriKey, DSC::CDscBlob& envelopeKey);

	public:
		DSC_BIND_ATTR(m_cltPubKey, m_svrPriKey, m_envelopeKey);

	public:
		DSC::CDscBlob& m_cltPubKey;
		DSC::CDscBlob& m_svrPriKey;
		DSC::CDscBlob& m_envelopeKey;
	};


	//提案事务的data-wrapper //初始的，写集还是3个集合的状态
	class CRoughProposalTransactionWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CRoughProposalTransactionWrapper(DSC::CDscBlob& proposal, DSC::CDscBlob& transProcessResult, DSC::CDscShortVector<VBH::CVbhUser>& vecUpdateUser,
			DSC::CDscShortVector<VBH::CVbhOnCreateInformation>& vecOnCreateInfo, DSC::CDscShortVector<VBH::CVbhInformation>& vecUpdateInfo);

	public:
		DSC_BIND_ATTR(m_proposal, m_transProcessResult, m_vecUpdateUser, m_vecOnCreateInfo, m_vecUpdateInfo);

	public:
		DSC::CDscBlob& m_proposal; //提案内容
		DSC::CDscBlob& m_transProcessResult; //提案处理结果 //remark
		DSC::CDscShortVector<VBH::CVbhUser>& m_vecUpdateUser; //涉及变更的用户写集 version 都> 0
		DSC::CDscShortVector<VBH::CVbhOnCreateInformation>& m_vecOnCreateInfo; //新创建的information列表
		DSC::CDscShortVector<VBH::CVbhInformation>& m_vecUpdateInfo; //涉及变更的 information 写集 version 都> 0
	};

	class CCryptSubmitProposalEsCltRspDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptSubmitProposalEsCltRspDataWrapper(DSC::CDscBlob& transContent, DSC::CDscBlob& nonce);

	public:
		DSC_BIND_ATTR(m_transContent, m_nonce);

	public:
		DSC::CDscBlob& m_transContent; //事务内容
		DSC::CDscBlob& m_nonce;
	};

	class CCryptSubmitRegistTransactionEsCltRspDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptSubmitRegistTransactionEsCltRspDataWrapper(VBH::COrderGenerateKey& genKey, DSC::CDscBlob& nonce);

	public:
		DSC_BIND_ATTR(m_genKey, m_nonce);

	public:
		VBH::COrderGenerateKey& m_genKey;
		DSC::CDscBlob& m_nonce;
	};

	class CCryptSubmitProposalTransactionEsCltRspDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptSubmitProposalTransactionEsCltRspDataWrapper(VBH::COrderGenerateTransactionKey& genTransKey, DSC::CDscBlob& nonce);

	public:
		DSC_BIND_ATTR(m_genTransKey, m_nonce);

	public:
		VBH::COrderGenerateTransactionKey& m_genTransKey;
		DSC::CDscBlob& m_nonce;
	};

	class CCryptQueryTransactionReqDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptQueryTransactionReqDataWrapper(DSC::CDscBlob& nonce, VBH::COrderGenerateTransactionKey& transKey);

	public:
		DSC_BIND_ATTR(m_nonce, m_transKey);

	public:
		DSC::CDscBlob& m_nonce;
		VBH::COrderGenerateTransactionKey& m_transKey;
	};

	class CCryptQueryTransactionRspDataWrapper
	{
	public:
		VBH_COMM_DEF_EXPORT CCryptQueryTransactionRspDataWrapper(DSC::CDscBlob& nonce, DSC::CDscBlob& transInfo);

	public:
		DSC_BIND_ATTR(m_nonce, m_transInfo);

	public:
		DSC::CDscBlob& m_nonce;
		DSC::CDscBlob& m_transInfo;
	};
}

#endif // !__vbh_commmon_wrapper_message_h

