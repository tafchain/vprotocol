#ifndef VBH_BLOCK_CACHE_MANAGER_H_8E799742D48B11E98A9060F18A3A20D1
#define VBH_BLOCK_CACHE_MANAGER_H_8E799742D48B11E98A9060F18A3A20D1

#include "ace/Basic_Types.h"
#include "ace/Shared_Memory_MM.h"

#include "dsc/container/dsc_string.h"

namespace VBH
{
	//基于循环数组的 区块缓冲区 管理器
	class CBlockCacheManager
	{
	public:
		//循环数组控制器 //头指针指向FIFO的 队头（待发送区块），尾指针指向最新放入数据的后1个位置
		struct SCaCtrl
		{
			ACE_UINT32 m_nArrayLength = 0; //数组长度
			ACE_UINT32 m_nItemCount = 0; //数据元素个数
			ACE_UINT32 m_nFrontIndex = 0; //头索引
			ACE_UINT32 m_nRearIndex = 0; //尾索引
		};

		//区块的 cache
		class CCache
		{
		public:
			class CFileHeader //cache file的头
			{
			public:
				ACE_UINT64 m_nBlockID;
			};

		public:
			ACE_UINT32 m_nFileID = 0; //文件ID
			ACE_Shared_Memory_MM m_shmCache; //共享内存句柄
			char* m_pShareMem = NULL; //共享内存的指针，内存结构: 文件头|区块内容
			CFileHeader* m_pHeader;
			size_t m_nShareMemLen = 0; //共享内存的长度
			size_t m_nBlockDataLen = 0; //区块数据的长度
		};

	public:
		//打开管理器
		VBH_COMM_DEF_EXPORT ACE_INT32 Open(const CDscString& strBasePath, const ACE_UINT32 nCacheCount);

		//关闭管理器
		VBH_COMM_DEF_EXPORT void Close(void);

		//循环数组是否已满
		bool IsFull(void) const;

		//循环数组是否已空
		bool IsEmpty(void) const;

		//获取尾部缓冲区的引用 //调用该函数，以向尾部缓冲区存入数据 //调用前保证数组未满
		CBlockCacheManager::CCache& GetRearCache(void);

		//获取头部缓冲区的引用 //调用该函数，以获取头部缓冲区中的数据 //调用前保证数组不空
		CBlockCacheManager::CCache& GetFrontCache(void);
		
		//在队尾放入元素 //实际放入操作已经通过GetRearCache进行 //push前要保存上下文，如果中途失败，需要恢复上下文
		void PushBack(void);

		//把队头元素pop出来 //实际取出操作已经通过GetFrontCache进行 //pop前要保存上下文，如果中途失败，需要恢复上下文
		void PopFront(void);

		//保存上下文（循环数组的控制结构）
		void SaveContext(CBlockCacheManager::SCaCtrl& cac);

		//恢复上下文（循环数组的控制结构）
		void RestoreContext(const CBlockCacheManager::SCaCtrl& cac);

		//重新设置缓冲文件大小
		VBH_COMM_DEF_EXPORT ACE_INT32 ResizeCache(CCache& cache, const size_t nSize);

	private:
		//创建cache文件
		ACE_INT32 CreateCacheFile(ACE_UINT32 nFileID);

		//加载cache文件
		ACE_INT32 LoadCacheFile(CCache& cache);

	private:
		CDscString m_strBasePath; //基础路径
		ACE_UINT32 m_nCacheCount; //缓存的个数

		ACE_Shared_Memory_MM m_shmCaCtrl;
		SCaCtrl* m_pCaCtrl; //循环数组 共享内存句柄和指针

		CCache* m_pzCache = NULL; //区块的缓冲数组
	};

}

#include "common/vbh_block_cache_manager.inl"

#endif // !VBH_BLOCK_CACHE_MANAGER_H_8E799742D48B11E98A9060F18A3A20D1
