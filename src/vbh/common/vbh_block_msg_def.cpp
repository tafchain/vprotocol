#include "common/vbh_block_msg_def.h"

ACE_INT32 VBH::CBcBlock::CaculateMerkelTree(VBH::CMerkelTree& merkelTree)
{
	merkelTree.Clear();
	for (ACE_UINT32 idx = 0; idx < m_vecTransaction.GetSize(); ++idx)
	{
		merkelTree.AddLeafNode(m_vecTransaction[idx].GetBuffer(), m_vecTransaction[idx].GetSize());
	}
	merkelTree.BuildMerkleTree();
	merkelTree.GetRoot(m_header.m_merkelTreeRootHash);

	return 0;
}

ACE_INT32 VBH::CBcBlock::CheckMerkelTree(VBH::CMerkelTree& merkelTree)
{
	merkelTree.Clear();
	for (ACE_UINT32 idx = 0; idx < m_vecTransaction.GetSize(); ++idx)
	{
		merkelTree.AddLeafNode(m_vecTransaction[idx].GetBuffer(), m_vecTransaction[idx].GetSize());
	}
	merkelTree.BuildMerkleTree();

	VBH::CHashBuffer merkelRoot;

	merkelTree.GetRoot(merkelRoot);

	return VBH::IsEqual(merkelRoot, m_header.m_merkelTreeRootHash) ? 0 : -1;
}
