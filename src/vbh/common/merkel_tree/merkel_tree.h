#ifndef VBH_MERKEL_TREE_H_27C4328CA76D11E9B55260F18A3A20D1
#define VBH_MERKEL_TREE_H_27C4328CA76D11E9B55260F18A3A20D1

#include "ace/Basic_Types.h"
#include "common/vbh_encrypt_lib.h"

namespace VBH
{

	using CHashBuffer = CDscArray<char, VBH_BLOCK_DIGEST_LENGTH>;

	class VBH_COMM_DEF_EXPORT CMerkelNode
	{
	public:
		char m_valueBuffer[VBH_BLOCK_DIGEST_LENGTH * 2]; //ǰ������ڴ���Լ���hashֵ���������ڼ����ϲ�ڵ�ʱ����ʱ������

	public:
		bool operator == (const CMerkelNode& value)const; //����hashֵ�Ƿ����
		bool IsEqual(const VBH::CHashBuffer& value) const;
		void operator = (const CMerkelNode& value); //����hashֵ�Ƿ����

		void SetHashValue(const char* pValue); //���ü���õ�hashֵ
		void SetHashValue(const VBH::CHashBuffer& hashBlock); //���ü���õ�hashֵ

		void SetOriginalData(const char* pData, const ACE_UINT32& nLen); //����ԭʼ���ݣ����м���hashֵ

		void GetHashValue(VBH::CHashBuffer& hashBlock) const; //��ȡ�洢��hashֵ

		void GetHashValue(char* pValue); //��ȡ�洢��hashֵ
	};

#define  MERKEL_TREE_INIT_NODE_COUNT 1024 //��ʼֵ
#define  MERKEL_TREE_NODE_INCREASE_STEP 256 //����ֵ

	class VBH_COMM_DEF_EXPORT CMerkelTree
	{
	public:
		CMerkelTree();
		virtual ~CMerkelTree();

		//��յ�ǰ��
		void Clear();

		//��ȡ������hashֵ��hashֵ�����ǹ̶���
		void GetRoot(VBH::CHashBuffer& hashBlock) const;

		//���1��Ҷ�ӽڵ㣬����Ϊ����Ҷ�ӽڵ�����ݣ���ҪΪ�����ݼ���hashֵ
		ACE_INT32 AddLeafNode(const char* pOriginalData, const ACE_UINT32& nLen);

		//������������֮����Եõ����ڵ��ֵ
		ACE_INT32 BuildMerkleTree();

	protected:
		//���ӽڵ�
		void InceaseNode();

		//��������Ҷ�ӽڵ㣬���㸸�ڵ��hashֵ
		void CalculateHashValue(CMerkelNode* pLeft, CMerkelNode* pRight, CMerkelNode* pParent);

	private:
		CMerkelNode* m_pNodes = NULL;
		ACE_UINT32 m_nAllocNum = MERKEL_TREE_INIT_NODE_COUNT; //�Ѿ����ٵĽڵ����
		ACE_UINT32 m_nCurNum = 0; //�Ѿ���ӵĽڵ����
	};

	//�Ƚ�����CHashBuffer�Ƿ�һ��
	bool IsEqual(VBH::CHashBuffer& left, VBH::CHashBuffer& right);

}

#include "common/merkel_tree/merkel_tree.inl"

#endif // vbh_merkle_tree_h__
