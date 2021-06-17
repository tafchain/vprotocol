
inline bool VBH::CBlockCacheManager::IsFull(void) const
{
	return (m_pCaCtrl->m_nItemCount == m_pCaCtrl->m_nArrayLength);
}

inline bool VBH::CBlockCacheManager::IsEmpty(void) const
{
	return (m_pCaCtrl->m_nItemCount == 0);
}

inline VBH::CBlockCacheManager::CCache& VBH::CBlockCacheManager::GetRearCache(void)
{
	return m_pzCache[m_pCaCtrl->m_nRearIndex];
}

inline VBH::CBlockCacheManager::CCache& VBH::CBlockCacheManager::GetFrontCache(void)
{
	return m_pzCache[m_pCaCtrl->m_nFrontIndex];
}

inline void VBH::CBlockCacheManager::PushBack(void)
{
	++m_pCaCtrl->m_nRearIndex;
	if (m_pCaCtrl->m_nRearIndex >= m_pCaCtrl->m_nArrayLength)
	{
		m_pCaCtrl->m_nRearIndex = 0;
	}
	++m_pCaCtrl->m_nItemCount;
}

inline void VBH::CBlockCacheManager::PopFront(void)
{
	++m_pCaCtrl->m_nFrontIndex;
	if (m_pCaCtrl->m_nFrontIndex >= m_pCaCtrl->m_nArrayLength)
	{
		m_pCaCtrl->m_nFrontIndex = 0;
	}
	--m_pCaCtrl->m_nItemCount;
}

inline void VBH::CBlockCacheManager::SaveContext(CBlockCacheManager::SCaCtrl& cac)
{
	cac = *m_pCaCtrl;
}

inline void VBH::CBlockCacheManager::RestoreContext(const CBlockCacheManager::SCaCtrl& cac)
{
	*m_pCaCtrl = cac;
}
