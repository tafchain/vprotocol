#ifndef VBH_BLOCK_MSG_DEF_H_B087D2B2BF0611E9B4F760F18A3A20D1
#define VBH_BLOCK_MSG_DEF_H_B087D2B2BF0611E9B4F760F18A3A20D1

#include "common/vbh_comm_msg_def.h"
#include "common/merkel_tree/merkel_tree.h"

namespace VBH
{
	//区块头的结构定义
	class CBcBlockHeader 
	{
	public:
		DSC_BIND_ATTR(m_nOrderID, m_nBlockID, m_preBlockHash, m_merkelTreeRootHash);

	public:
		ACE_UINT16 m_nOrderID; //打包区块时的orderID，校验时也要用
		ACE_UINT64 m_nBlockID; //区块ID，从1开始编码
		ACE_UINT64 m_nBlockTime; //区块生成时间
		CHashBuffer m_preBlockHash; //前一区块的hash值
		CHashBuffer m_merkelTreeRootHash; //本区块merkel树根的hash值
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
	//代表一个区块
	class CBcBlock
	{
	public:
		//编码前需要调用的初始化函数 //调用函数前，必须已经填好数据了
		VBH_COMM_DEF_EXPORT ACE_INT32 CaculateMerkelTree(VBH::CMerkelTree& merkelTree);

		//解码后对merkel树进行校验
		VBH_COMM_DEF_EXPORT ACE_INT32 CheckMerkelTree(VBH::CMerkelTree& merkelTree);

	public:
		DSC_BIND_ATTR(m_header, m_vecTransaction);

	public:
		VBH::CBcBlockHeader m_header; //区块头
		VBH::CSimpleVector<DSC::CDscBlob> m_vecTransaction; //区块内事务列表
	};
}

#include "common/vbh_block_msg_def.inl"

#endif
