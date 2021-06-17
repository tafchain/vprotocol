#include "common/vbh_comm_wrap_msg_def.h"

VBH::CCryptRegistUserCltEsReqDataWrapper::CCryptRegistUserCltEsReqDataWrapper(bool& bWaitNotify, bool& bSubmitNode, ACE_INT16& nCcID, ACE_UINT32& nChannelID, DSC::CDscBlob& userKey, DSC::CDscBlob& userInfo, DSC::CDscBlob& nonce)
	: m_bWaitNotify(bWaitNotify)
	, m_bSubmitNode(bSubmitNode)
	, m_nCcID(nCcID)
	, m_nChannelID(nChannelID)
	, m_userKey(userKey)
	, m_userInfo(userInfo)
	, m_nonce(nonce)
{
}

VBH::CCryptRegistUserEsCltRspDataWrapper::CCryptRegistUserEsCltRspDataWrapper(bool& bAllowRegist, DSC::CDscBlob& userInitInfo, DSC::CDscBlob& nonce, DSC::CDscBlob& svrPubCryptKey, DSC::CDscBlob& cltPriCryptKey, DSC::CDscBlob& envelopeKey)
	: m_bAllowRegist(bAllowRegist)
	, m_userInitInfo(userInitInfo)
	, m_nonce(nonce)
	, m_svrPubCryptKey(svrPubCryptKey)
	, m_cltPriCryptKey(cltPriCryptKey)
	, m_envelopeKey(envelopeKey)
{
}

VBH::CCryptQueryUserInfoRspWrapper::CCryptQueryUserInfoRspWrapper(DSC::CDscBlob& userInfo, DSC::CDscBlob& nonce)
	: m_userInfo(userInfo)
	, m_nonce(nonce)
{
}


VBH::CCryptSubmitProposalCltEsReqDataWrapper::CCryptSubmitProposalCltEsReqDataWrapper(bool& bWaitNotify, bool& bSubmitNode, ACE_INT16& nCcID, ACE_UINT32& nAction, DSC::CDscBlob& proposal, DSC::CDscBlob& nonce)
	: m_bWaitNotify(bWaitNotify)
	, m_bSubmitNode(bSubmitNode)
	, m_nCcID(nCcID)
    , m_nAction(nAction)
	, m_proposal(proposal)
	, m_nonce(nonce)
{
}


VBH::CServerCryptKeyWrapper::CServerCryptKeyWrapper(DSC::CDscBlob& cltPubKey, DSC::CDscBlob& svrPriKey, DSC::CDscBlob& envelopeKey)
	: m_cltPubKey(cltPubKey)
	, m_svrPriKey(svrPriKey)
	, m_envelopeKey(envelopeKey)
{
}

VBH::CRoughProposalTransactionWrapper::CRoughProposalTransactionWrapper(DSC::CDscBlob& proposal, DSC::CDscBlob& transProcessResult,
	DSC::CDscShortVector<VBH::CVbhUser>& vecUpdateUser, DSC::CDscShortVector<VBH::CVbhOnCreateInformation>& vecOnCreateInfo,
	DSC::CDscShortVector<VBH::CVbhInformation>& vecUpdateInfo)
	: m_proposal(proposal)
	, m_transProcessResult(transProcessResult)
	, m_vecUpdateUser(vecUpdateUser)
	, m_vecOnCreateInfo(vecOnCreateInfo)
	, m_vecUpdateInfo(vecUpdateInfo)
{
}

VBH::CCryptSubmitProposalEsCltRspDataWrapper::CCryptSubmitProposalEsCltRspDataWrapper(DSC::CDscBlob& transContent, DSC::CDscBlob& nonce)
	: m_transContent(transContent)
	, m_nonce(nonce)
{
}

VBH::CCryptSubmitRegistTransactionEsCltRspDataWrapper::CCryptSubmitRegistTransactionEsCltRspDataWrapper(VBH::COrderGenerateKey& genKey, DSC::CDscBlob& nonce)
	: m_genKey(genKey)
	, m_nonce(nonce)
{
}

VBH::CCryptSubmitProposalTransactionEsCltRspDataWrapper::CCryptSubmitProposalTransactionEsCltRspDataWrapper(VBH::COrderGenerateTransactionKey& genTransKey, DSC::CDscBlob& nonce)
	: m_genTransKey(genTransKey)
	, m_nonce(nonce)
{
}


VBH::CCryptQueryTransactionReqDataWrapper::CCryptQueryTransactionReqDataWrapper(DSC::CDscBlob& nonce, VBH::COrderGenerateTransactionKey& transKey)
	:m_nonce(nonce)
	,m_transKey(transKey)
{

}

VBH::CCryptQueryTransactionRspDataWrapper::CCryptQueryTransactionRspDataWrapper(DSC::CDscBlob& nonce, DSC::CDscBlob& transInfo)
	:m_nonce(nonce)
	,m_transInfo(transInfo)
{

}


