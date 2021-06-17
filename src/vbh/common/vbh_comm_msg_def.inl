template <typename TYPE>
ACE_INT32 VBH::CSimpleVector<TYPE>::Open(const ACE_UINT32 nSize)
{
	ACE_ASSERT(m_pItemBuf == NULL);
	//只有在空间增加时，才重新开辟空间
	m_nBufLen = nSize;
	DSC_THREAD_TYPE_ALLOCATE_ARRAY(m_pItemBuf, m_nBufLen);

	//开辟成功和失败, 返回不同
	if (m_pItemBuf)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

template <typename TYPE>
void VBH::CSimpleVector<TYPE>::Close(void)
{
	if (m_pItemBuf)
	{
		DSC_THREAD_TYPE_DEALLOCATE_ARRAY(m_pItemBuf, m_nBufLen);
		m_pItemBuf = NULL;
	}
}

template <typename TYPE>
inline ACE_UINT32 VBH::CSimpleVector<TYPE>::GetSize(void)
{
	return m_nItemCount;
}

template <typename TYPE>
inline ACE_UINT32 VBH::CSimpleVector<TYPE>::GetBufLen(void)
{
	return m_nBufLen;
}

template <typename TYPE>
inline ACE_INT32 VBH::CSimpleVector<TYPE>::PushBack(TYPE& t)
{
	if (m_nItemCount < m_nBufLen)
	{
		m_pItemBuf[m_nItemCount] = t;
		++m_nItemCount;

		return 0;
	}

	return -1;
}

template <typename TYPE>
inline TYPE& VBH::CSimpleVector<TYPE>::operator[](const ACE_UINT32 idx)
{
	return m_pItemBuf[idx];
}

template <typename TYPE>
inline void VBH::CSimpleVector<TYPE>::Clear(void)
{
	m_nItemCount = 0;
}

template <typename TYPE>
inline void VBH::CSimpleVector<TYPE>::GetEncodeSize(CDscGetSizeCodeState& rCodeStateControl)
{
	DscVCodecBindAttr(m_nItemCount, rCodeStateControl);

	ACE_ASSERT(m_pItemBuf);
	for (ACE_UINT32 idx = 0; idx < m_nItemCount; ++idx)
	{
		DscVCodecBindAttr(m_pItemBuf[idx], rCodeStateControl);
	}
}

template <typename TYPE>
inline void VBH::CSimpleVector<TYPE>::Encode(CDscEncodeState& rCodeStateControl)
{
	DscVCodecBindAttr(m_nItemCount, rCodeStateControl);

	ACE_ASSERT(m_pItemBuf);
	for (ACE_UINT32 idx = 0; idx < m_nItemCount; ++idx)
	{
		DscVCodecBindAttr(m_pItemBuf[idx], rCodeStateControl);
	}
}

template <typename TYPE>
inline ACE_INT32 VBH::CSimpleVector<TYPE>::Decode(CDscDecodeState& rCodeStateControl)
{
	ACE_UINT32 nSize;

	if (rCodeStateControl.DecodeItem(nSize))
	{
		return -1;
	}
	else
	{
		if (nSize > m_nBufLen)
		{
			this->Close();
			this->Open(nSize);
		}
		
		m_nItemCount = nSize; //clear

		for (ACE_UINT32 i = 0; i < nSize; ++i)
		{
			if (DscVCodecBindAttr(m_pItemBuf[i], rCodeStateControl))
			{
				return -1;
			}
		}

		return 0;
	}
}

template< typename TYPE >
inline void DscVCodecBindAttr(VBH::CSimpleVector<TYPE>& rSimpleVec, CDscGetSizeCodeState& rCodeStateControl)
{
	rSimpleVec.GetEncodeSize(rCodeStateControl);
}

template< typename TYPE >
inline void DscVCodecBindAttr(VBH::CSimpleVector<TYPE>& rSimpleVec, CDscEncodeState& rCodeStateControl)
{
	rSimpleVec.Encode(rCodeStateControl);
}

template< typename TYPE >
inline ACE_INT32 DscVCodecBindAttr(VBH::CSimpleVector<TYPE>& rSimpleVec, CDscDecodeState& rCodeStateControl)
{
	rSimpleVec.Decode(rCodeStateControl);
}
