#include "common/merkel_tree/merkel_tree.h"
#include "dsc/mem_mng/dsc_allocator.h"

VBH::CMerkelTree::CMerkelTree()
{
	DSC_THREAD_TYPE_ALLOCATE_ARRAY(m_pNodes, m_nAllocNum);
}

VBH::CMerkelTree::~CMerkelTree()
{
	if (m_pNodes)
	{
		DSC_THREAD_TYPE_DEALLOCATE_ARRAY(m_pNodes, m_nAllocNum);
	}
}

ACE_INT32 VBH::CMerkelTree::BuildMerkleTree()
{
	if (!m_nCurNum) //�ڵ�Ϊ��ʱ, ֱ�ӷ��ؼ���ʧ�ܣ���ʱ�޷�����
	{
		return -1;
	}

	ACE_UINT32 curSize = m_nCurNum;
	ACE_UINT32 newSize;
	ACE_UINT32 idx;

	while (curSize > 1) //��ֻ1���ڵ����
	{
		newSize = 0;
		for (idx = 0; idx < curSize; idx += 2)
		{
			if ((idx + 1) < curSize)
			{
				CalculateHashValue(m_pNodes+idx, m_pNodes + idx + 1, m_pNodes + newSize);
			}
			else
			{
				CalculateHashValue(m_pNodes + idx, m_pNodes + idx, m_pNodes + newSize);
			}
			++newSize;
		}
		curSize = newSize;
	}
	
	return 0;
}

void VBH::CMerkelTree::InceaseNode()
{
	VBH::CMerkelNode* pNodes;
	const ACE_UINT32 nAllocNum = m_nAllocNum + MERKEL_TREE_NODE_INCREASE_STEP;

	DSC_THREAD_TYPE_ALLOCATE_ARRAY(pNodes, nAllocNum);
	
	for (ACE_UINT32 i = 0; i < m_nCurNum; ++i)
	{
		pNodes[i] = m_pNodes[i];
	}
	
	DSC_THREAD_TYPE_DEALLOCATE_ARRAY(m_pNodes, m_nAllocNum);

	m_pNodes = pNodes;
	m_nAllocNum = nAllocNum;
}
