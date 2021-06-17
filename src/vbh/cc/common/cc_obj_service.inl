template<typename USER_KEY_TYPE>
ACE_INT32 CCcObjService::GetVbhUser(const ACE_UINT32 nSessionID, const ACE_UINT32 nChannelID, USER_KEY_TYPE& rUserKey)
{
	char* pMsgBuf;
	size_t nMsgBufLen;

	DEF_ENCODE(rUserKey, pMsgBuf, nMsgBufLen);
	
	return CCcService::GetVbhUser(nSessionID, nChannelID, pMsgBuf, nMsgBufLen);
}

template<typename USER_KEY_TYPE, typename USER_INFO_TYPE>
ACE_INT32 CCcObjService::DecodeUser(USER_KEY_TYPE& rUserKey, USER_INFO_TYPE& userInfo, CSimpleUser& rUser)
{
	if (DSC::Decode(rUserKey, rUser.m_userKey.c_str(), rUser.m_userKey.size()))
	{
		return -1;
	}

	if (DSC::Decode(userInfo, rUser.m_userInfo.c_str(), rUser.m_userInfo.size()))
	{
		return -1;
	}

	return 0;
}

template<typename USER_KEY_TYPE, typename USER_INFO_TYPE>
ACE_INT32 CCcObjService::SetVbhUser(const ACE_UINT32 nSessionID, const ACE_UINT32 nChannelID, USER_KEY_TYPE& rUserKey, USER_INFO_TYPE& rUserInfo)
{
	char* pKeyBuf;
	size_t nKeyBufLen;
	char* pValueBuf;
	size_t nValueBufLen;

	DEF_ENCODE(rUserKey, pKeyBuf, nKeyBufLen);
	DSC::Encode(rUserInfo, pValueBuf, nValueBufLen);

	ACE_INT32 nRet = CCcService::SetVbhUser(nSessionID, nChannelID, pKeyBuf, nKeyBufLen, pValueBuf, nValueBufLen);

	DSC_THREAD_SIZE_FREE(pValueBuf, nValueBufLen);

	return nRet;
}
