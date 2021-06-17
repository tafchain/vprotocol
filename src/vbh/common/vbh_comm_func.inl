
inline bool VBH::IsEqual(const CDscString& str, DSC::CDscBlob& blob)
{
	if (str.size() == blob.GetSize())
	{
		return 0 == ::memcmp(str.c_str(), blob.GetBuffer(), blob.GetSize());
	}

	return false;
}

inline void VBH::Assign(CDscString& str, DSC::CDscBlob& blob)
{
	str.assign(blob.GetBuffer(), blob.GetSize());
}

inline void VBH::Assign(DSC::CDscBlob& blob, CDscString& str)
{
	blob.Set(str.data(), str.size());
}



inline ACE_INT32 VBH::GetVbhProfileInt(const char* pszKeyName, ACE_INT32& nValue)
{
	return CDscConfigureDemon::instance()->GetDscProfileInt("VBH_SYS_CFG", pszKeyName, nValue);
}

inline ACE_INT32 VBH::GetVbhProfileString(const char* pszKeyName, CDscString& strValue)
{
	return CDscConfigureDemon::instance()->GetDscProfileString("VBH_SYS_CFG", pszKeyName, strValue);
}


inline ACE_UINT16 VBH::GetDataHashValue(const DSC::CDscBlob& data, const ACE_UINT32 nHashBits)
{
	if (nHashBits)
	{
		ACE_UINT64 h = DSC::Hash(data.GetBuffer(), data.GetSize());

		h *= DSC_HASH_PRIME;

		return (h >> (64 - nHashBits)) + 1; //committerID�����1��ʼ��������
	}
	else
	{
		return 1;
	}
}

inline void VBH::IncreaseVersion(ACE_UINT32& nVersion)
{
	if (nVersion == ACE_UINT32(-1)) //������ת
	{
		nVersion = 1;
	}
	else
	{
		++nVersion;
	}
}

inline bool VBH::IsVersionMatch(const ACE_UINT32 nPreVersion, const ACE_UINT32 nCurVersion)
{
	if (nPreVersion == ACE_UINT32(-1)) //������ת
	{
		if (nCurVersion == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((nPreVersion+1) == nCurVersion)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

inline bool VBH::IsCreatedVersion(const ACE_UINT32 nVersion)
{
	return (nVersion == 0);
}

template <typename CFG_TYPE>
inline ACE_INT32 VBH::CreateMmapCfgFile(const CDscString& strFilePath, CFG_TYPE& initCfg)
{
	ACE_HANDLE handle = ACE_OS::open(strFilePath.c_str(), O_RDWR | O_BINARY | O_CREAT);

	if (ACE_INVALID_HANDLE == handle)
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("cann't open file:%s, last error code:%d, last error reason:%s.", strFilePath.c_str(), nLastError, ACE_OS::strerror(nLastError));
		return -1;
	}

	if (-1 == ACE_OS::write(handle, &initCfg, sizeof(initCfg)))
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("write file failed, file name:%s, last error code:%d, last error reason:%s.", strFilePath.c_str(), nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(handle);

		return -1;
	}

	ACE_OS::close(handle);

	return 0;
}

template<typename CFG_TYPE>
inline ACE_INT32 VBH::LoadMmapCfgFile(const CDscString& strFilePath, ACE_Shared_Memory_MM& shmHandle, CFG_TYPE*& pCfg)
{
	//�򿪰������ڴ���ļ�
	if (-1 == shmHandle.open(strFilePath.c_str(), sizeof(CFG_TYPE)))
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("open share memory failed, file name:%s, last error code:%d, last error reason:%s.", strFilePath.c_str(), nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}

	pCfg = (CFG_TYPE*)shmHandle.malloc();
	if (NULL == pCfg)
	{
		DSC_RUN_LOG_ERROR("alloc memory for memory map file failed, file name:%s.", strFilePath.c_str());
		shmHandle.close();

		return -1;
	}

	return 0;
}

