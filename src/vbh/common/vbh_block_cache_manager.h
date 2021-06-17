#ifndef VBH_BLOCK_CACHE_MANAGER_H_8E799742D48B11E98A9060F18A3A20D1
#define VBH_BLOCK_CACHE_MANAGER_H_8E799742D48B11E98A9060F18A3A20D1

#include "ace/Basic_Types.h"
#include "ace/Shared_Memory_MM.h"

#include "dsc/container/dsc_string.h"

namespace VBH
{
	//����ѭ������� ���黺���� ������
	class CBlockCacheManager
	{
	public:
		//ѭ����������� //ͷָ��ָ��FIFO�� ��ͷ�����������飩��βָ��ָ�����·������ݵĺ�1��λ��
		struct SCaCtrl
		{
			ACE_UINT32 m_nArrayLength = 0; //���鳤��
			ACE_UINT32 m_nItemCount = 0; //����Ԫ�ظ���
			ACE_UINT32 m_nFrontIndex = 0; //ͷ����
			ACE_UINT32 m_nRearIndex = 0; //β����
		};

		//����� cache
		class CCache
		{
		public:
			class CFileHeader //cache file��ͷ
			{
			public:
				ACE_UINT64 m_nBlockID;
			};

		public:
			ACE_UINT32 m_nFileID = 0; //�ļ�ID
			ACE_Shared_Memory_MM m_shmCache; //�����ڴ���
			char* m_pShareMem = NULL; //�����ڴ��ָ�룬�ڴ�ṹ: �ļ�ͷ|��������
			CFileHeader* m_pHeader;
			size_t m_nShareMemLen = 0; //�����ڴ�ĳ���
			size_t m_nBlockDataLen = 0; //�������ݵĳ���
		};

	public:
		//�򿪹�����
		VBH_COMM_DEF_EXPORT ACE_INT32 Open(const CDscString& strBasePath, const ACE_UINT32 nCacheCount);

		//�رչ�����
		VBH_COMM_DEF_EXPORT void Close(void);

		//ѭ�������Ƿ�����
		bool IsFull(void) const;

		//ѭ�������Ƿ��ѿ�
		bool IsEmpty(void) const;

		//��ȡβ�������������� //���øú���������β���������������� //����ǰ��֤����δ��
		CBlockCacheManager::CCache& GetRearCache(void);

		//��ȡͷ�������������� //���øú������Ի�ȡͷ���������е����� //����ǰ��֤���鲻��
		CBlockCacheManager::CCache& GetFrontCache(void);
		
		//�ڶ�β����Ԫ�� //ʵ�ʷ�������Ѿ�ͨ��GetRearCache���� //pushǰҪ���������ģ������;ʧ�ܣ���Ҫ�ָ�������
		void PushBack(void);

		//�Ѷ�ͷԪ��pop���� //ʵ��ȡ�������Ѿ�ͨ��GetFrontCache���� //popǰҪ���������ģ������;ʧ�ܣ���Ҫ�ָ�������
		void PopFront(void);

		//���������ģ�ѭ������Ŀ��ƽṹ��
		void SaveContext(CBlockCacheManager::SCaCtrl& cac);

		//�ָ������ģ�ѭ������Ŀ��ƽṹ��
		void RestoreContext(const CBlockCacheManager::SCaCtrl& cac);

		//�������û����ļ���С
		VBH_COMM_DEF_EXPORT ACE_INT32 ResizeCache(CCache& cache, const size_t nSize);

	private:
		//����cache�ļ�
		ACE_INT32 CreateCacheFile(ACE_UINT32 nFileID);

		//����cache�ļ�
		ACE_INT32 LoadCacheFile(CCache& cache);

	private:
		CDscString m_strBasePath; //����·��
		ACE_UINT32 m_nCacheCount; //����ĸ���

		ACE_Shared_Memory_MM m_shmCaCtrl;
		SCaCtrl* m_pCaCtrl; //ѭ������ �����ڴ�����ָ��

		CCache* m_pzCache = NULL; //����Ļ�������
	};

}

#include "common/vbh_block_cache_manager.inl"

#endif // !VBH_BLOCK_CACHE_MANAGER_H_8E799742D48B11E98A9060F18A3A20D1
