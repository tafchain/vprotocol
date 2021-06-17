#ifndef VBH_BLOCK_MSG_DEF_H_B087D2B2BF0611E9B4F760F18A3A20D1
#define VBH_BLOCK_MSG_DEF_H_B087D2B2BF0611E9B4F760F18A3A20D1

#include "common/vbh_comm_msg_def.h"
#include "common/merkel_tree/merkel_tree.h"

namespace VBH
{
	//����ͷ�Ľṹ����
	class CBcBlockHeader 
	{
	public:
		DSC_BIND_ATTR(m_nOrderID, m_nBlockID, m_preBlockHash, m_merkelTreeRootHash);

	public:
		ACE_UINT16 m_nOrderID; //�������ʱ��orderID��У��ʱҲҪ��
		ACE_UINT64 m_nBlockID; //����ID����1��ʼ����
		ACE_UINT64 m_nBlockTime; //��������ʱ��
		CHashBuffer m_preBlockHash; //ǰһ�����hashֵ
		CHashBuffer m_merkelTreeRootHash; //������merkel������hashֵ
	};

	class CBcTransaction
	{
	public:
		CBcTransaction(VBH::CTransactionType type);
		~CBcTransaction();

		CBcTransaction() = delete;
		CBcTransaction(const CBcTransaction&) = delete;

		VBH::CRegistUserTransaction* GetRegisterUserTransaction();
		VBH::CProposeTransaction* GetProposeTransaction();

	private:
		VBH::CTransactionType m_type;
		union
		{
			VBH::CRegistUserTransaction* m_regTrans;
			VBH::CProposeTransaction* m_proposeTrans;
		};
	};
	//����һ������
	class CBcBlock
	{
	public:
		//����ǰ��Ҫ���õĳ�ʼ������ //���ú���ǰ�������Ѿ����������
		VBH_COMM_DEF_EXPORT ACE_INT32 CaculateMerkelTree(VBH::CMerkelTree& merkelTree);

		//������merkel������У��
		VBH_COMM_DEF_EXPORT ACE_INT32 CheckMerkelTree(VBH::CMerkelTree& merkelTree);

	public:
		DSC_BIND_ATTR(m_header, m_vecTransaction);

	public:
		VBH::CBcBlockHeader m_header; //����ͷ
		VBH::CSimpleVector<DSC::CDscBlob> m_vecTransaction; //�����������б�
	};
}

#include "common/vbh_block_msg_def.inl"

#endif
