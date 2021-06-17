inline void VBH::CMerkelNode::operator = (const VBH::CMerkelNode& value)
{
	::memcpy(m_valueBuffer, value.m_valueBuffer, VBH_BLOCK_DIGEST_LENGTH);
}

inline bool VBH::CMerkelNode::operator ==(const VBH::CMerkelNode& value) const //����hashֵ�Ƿ����
{
	return 0 == ::memcmp(m_valueBuffer, value.m_valueBuffer, VBH_BLOCK_DIGEST_LENGTH);
}

inline bool VBH::CMerkelNode::IsEqual(const VBH::CHashBuffer& value) const
{
	return 0 == ::memcmp(m_valueBuffer, value.c_array(), VBH_BLOCK_DIGEST_LENGTH);
}

inline void VBH::CMerkelNode::SetHashValue(const char* pValue) //���ü���õ�hashֵ
{
	::memcpy(m_valueBuffer, pValue, VBH_BLOCK_DIGEST_LENGTH);
}

inline void VBH::CMerkelNode::SetHashValue(const VBH::CHashBuffer& hashBlock) //���ü���õ�hashֵ
{
	::memcpy(m_valueBuffer, hashBlock.c_array(), VBH_BLOCK_DIGEST_LENGTH);
}

inline void VBH::CMerkelNode::SetOriginalData(const char* pData, const ACE_UINT32& nLen)//����ԭʼ���ݣ����м���hashֵ
{
	VBH::vbhDigest(pData, nLen, m_valueBuffer);
}

inline void VBH::CMerkelNode::GetHashValue(VBH::CHashBuffer& hashBlock) const //��ȡ�洢��hashֵ
{
	::memcpy(hashBlock.data(), m_valueBuffer, VBH_BLOCK_DIGEST_LENGTH);
}

inline void VBH::CMerkelNode::GetHashValue(char* pValue)
{
	::memcpy(pValue, m_valueBuffer, VBH_BLOCK_DIGEST_LENGTH);
}

inline void VBH::CMerkelTree::Clear()
{
	m_nCurNum = 0;
}

inline void VBH::CMerkelTree::GetRoot(VBH::CHashBuffer& hashBlock) const
{
	m_pNodes[0].GetHashValue(hashBlock);
}

inline ACE_INT32 VBH::CMerkelTree::AddLeafNode(const char* pOriginalData, const ACE_UINT32& nLen)
{
	if (m_nCurNum >= m_nAllocNum)
	{
		InceaseNode();
	}

	m_pNodes[m_nCurNum].SetOriginalData(pOriginalData, nLen);
	++m_nCurNum;

	return 0;
}

inline void VBH::CMerkelTree::CalculateHashValue(VBH::CMerkelNode* pLeft, VBH::CMerkelNode* pRight, VBH::CMerkelNode* pParent)
{
	::memcpy(pRight->m_valueBuffer + VBH_BLOCK_DIGEST_LENGTH, pLeft->m_valueBuffer , VBH_BLOCK_DIGEST_LENGTH);
	vbhDigest(pRight->m_valueBuffer, VBH_BLOCK_DIGEST_LENGTH * 2, pParent->m_valueBuffer);//����hash��
}

inline bool VBH::IsEqual(CHashBuffer& left, CHashBuffer& right)
{
	return 0 == ::memcmp(left.data(), right.data(), VBH_BLOCK_DIGEST_LENGTH);
}
