template<typename USER_KEY_TYPE>
ACE_INT32 CCcJsonService::GetVbhUser(const ACE_UINT32 nSessionID, const ACE_UINT32 nChannelID, USER_KEY_TYPE& rUserKey)
{
	SJsonWriter* pJsonWriter = JSON_CODE::Encode(rUserKey);
	ACE_INT32 nRet =  CCcService::GetVbhUser(nSessionID, nChannelID, pJsonWriter->m_pBuffer, pJsonWriter->m_nCurrentPos);
	
	json_writer_free(pJsonWriter);

	return nRet;
}

template<typename USER_KEY_TYPE, typename USER_INFO_TYPE>
ACE_INT32 CCcJsonService::DecodeUser(USER_KEY_TYPE& rUserKey, USER_INFO_TYPE& userInfo, CVbhUser& rUser)
{
	SJsonReader* pJsonKeyReader = ::create_json_reader(rUser.m_userKey.m_key.data(), rUser.m_userKey.m_key.size());
	SJsonReader* pJsonValueReader = ::create_json_reader(rUser.m_userInfo.data(), rUser.m_userInfo.size());
	
	if (JSON_CODE::Decode(rUserKey, pJsonKeyReader))
	{
		json_reader_free(pJsonKeyReader);
		json_reader_free(pJsonValueReader);

		return -1;
	}

	if (JSON_CODE::Decode(userInfo, pJsonValueReader))
	{
		json_reader_free(pJsonKeyReader);
		json_reader_free(pJsonValueReader);

		return -1;
	}

	json_reader_free(pJsonKeyReader);
	json_reader_free(pJsonValueReader);

	return 0;
}

template<typename USER_KEY_TYPE, typename USER_INFO_TYPE>
ACE_INT32 CCcJsonService::SetVbhUser(const ACE_UINT32 nSessionID, const ACE_UINT32 nChannelID, USER_KEY_TYPE& rUserKey, USER_INFO_TYPE& rUserInfo)
{
	SJsonWriter* pJsonKeyWriter = JSON_CODE::Encode(rUserKey);
	SJsonWriter* pJsonValueWriter = JSON_CODE::Encode(rUserInfo);
	ACE_INT32 nRet = CCcService::SetVbhUser(nSessionID, nChannelID, pJsonKeyWriter->m_pBuffer, pJsonKeyWriter->m_nCurrentPos, pJsonValueWriter->m_pBuffer, pJsonValueWriter->m_nCurrentPos);

	json_writer_free(pJsonKeyWriter);
	json_writer_free(pJsonValueWriter);

	return nRet;
}
