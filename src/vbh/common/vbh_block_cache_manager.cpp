#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"

#include "common/vbh_comm_func.h"
#include "common/vbh_comm_macro_def.h"
#include "common/vbh_comm_error_code.h"

#include "common/vbh_block_cache_manager.h"

//Ĭ�ϵ�cache�ļ���С
#define DEF_BCM_DEFAULT_CACHE_FILE_SIZE 4*1024*1024 //2���ݴ�����
//cache�ļ�ÿ�������Ĵ�С
#define DEF_BCM_INCREASE_CACHE_FILE_SIZE 1*1024*1024 //2���ݴ�����


ACE_INT32 VBH::CBlockCacheManager::Open(const CDscString& strBasePath, const ACE_UINT32 nCacheCount)
{
	ACE_stat stat;
	CDscString strCaCtrlFilePath(strBasePath);

	m_strBasePath = strBasePath;
	m_nCacheCount = nCacheCount;

	strCaCtrlFilePath += DSC_FILE_PATH_SPLIT_CHAR;
	strCaCtrlFilePath += DEF_BLOCK_CACHE_CIRCULAR_CONTROLLER_FILE_NAME;

	//1. �������·�������ڣ��򴴽�����·������������Ӧ���ļ�
	if (-1 == ACE_OS::stat(strBasePath.c_str(), &stat)) 
	{
		SCaCtrl initCaCtrl;
		initCaCtrl.m_nArrayLength = nCacheCount; //��ʼʱ�����鳤��Ϊ����ֵ

		//1.1 ·�������ڣ�����·��
		DSC::DscRecurMkdir(strBasePath.c_str(), strBasePath.size());

		//1.2 ����ѭ����������ļ��ļ�
		if (VBH::CreateMmapCfgFile(strCaCtrlFilePath, initCaCtrl))
		{
			return -1;
		}

		//1.3 �������е�cache�ļ�
		for (ACE_UINT32 idx = 0; idx < nCacheCount; ++idx)
		{
			if (CreateCacheFile(idx))
			{
				return -1;
			}
		}
	}

	//2. ����ѭ����������ļ��ļ�
	if (VBH::LoadMmapCfgFile(strCaCtrlFilePath, m_shmCaCtrl, m_pCaCtrl))
	{
		return -1;
	}

	//3. ����cache����
	DSC_THREAD_TYPE_ALLOCATE_ARRAY(m_pzCache, nCacheCount);
	if (!m_pzCache)
	{
		DSC_RUN_LOG_ERROR("alloc memory failed.");
		return -1;
	}

	//4. �������е�cache�ļ�
	for (ACE_UINT32 idx = 0; idx < nCacheCount; ++idx)
	{
		m_pzCache[idx].m_nFileID = idx;
		if (LoadCacheFile(m_pzCache[idx]))
		{
			return -1;
		}
	}

	return 0;
}

void VBH::CBlockCacheManager::Close(void)
{
	if (m_pzCache)
	{
		for (ACE_UINT32 idx = 0; idx < m_nCacheCount; ++idx)
		{
			m_pzCache[idx].m_shmCache.close();
		}
		DSC_THREAD_TYPE_DEALLOCATE_ARRAY(m_pzCache, m_pCaCtrl->m_nArrayLength);
	}

	m_shmCaCtrl.close();
}

ACE_INT32 VBH::CBlockCacheManager::ResizeCache(CCache& cache, const size_t nSize)
{
	cache.m_nShareMemLen = DSC_ROUND_UP(nSize, DEF_BCM_INCREASE_CACHE_FILE_SIZE);

	//1. �ر�ԭʼӳ��
	cache.m_shmCache.close();

	//2. ���´��ļ���truncate �ļ��Ĵ�С
	CDscString strPath(m_strBasePath);

	strPath += DSC_FILE_PATH_SPLIT_CHAR;
	strPath += cache.m_nFileID;
	strPath += DEF_BLOCK_CACHE_FILE_SUBFIX;

	ACE_HANDLE handle = ACE_OS::open(strPath.c_str(), O_RDWR | O_BINARY);

	if (ACE_INVALID_HANDLE == handle)
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("cann't open file:%s, last error code:%d, last error reason:%s.", strPath.c_str(), nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}
	if (-1 == ACE_OS::ftruncate(handle, cache.m_nShareMemLen))
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("truncate file failed, file name:%s, last error code:%d, last error reason:%s.", strPath.c_str(), nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(handle);

		return -1;
	}
	ACE_OS::close(handle);

	//3. ����ӳ���ļ�
	if (-1 == cache.m_shmCache.open(strPath.c_str(), cache.m_nShareMemLen))
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("open share memory failed, file name:%s, last error code:%d, last error reason:%s.", strPath.c_str(), nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}

	cache.m_pShareMem = (char*)cache.m_shmCache.malloc();
	if (NULL == cache.m_pShareMem)
	{
		DSC_RUN_LOG_ERROR("alloc memory for memory map file failed, file name:%s.", strPath.c_str());
		cache.m_shmCache.close();

		return -1;
	}

	cache.m_pHeader = (CCache::CFileHeader*)cache.m_pShareMem;

	return 0;
}

ACE_INT32 VBH::CBlockCacheManager::CreateCacheFile(ACE_UINT32 nFileID)
{
	//cache�ļ���·��: ����·��/nFileID.cache
	CDscString strPath(m_strBasePath);

	strPath += DSC_FILE_PATH_SPLIT_CHAR;
	strPath += nFileID;
	strPath += DEF_BLOCK_CACHE_FILE_SUBFIX;

	ACE_HANDLE handle = ACE_OS::open(strPath.c_str(), O_RDWR | O_BINARY | O_CREAT);

	if (ACE_INVALID_HANDLE == handle)
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("cann't open file:%s, last error code:%d, last error reason:%s.", strPath.c_str(), nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}

	//��ʼ�������ļ���ʹ��ftruncate�������С
	if (-1 == ACE_OS::ftruncate(handle, DEF_BCM_DEFAULT_CACHE_FILE_SIZE))
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("truncate file failed, file name:%s, last error code:%d, last error reason:%s.", strPath.c_str(), nLastError, ACE_OS::strerror(nLastError));
		ACE_OS::close(handle);

		return -1;
	}

	ACE_OS::close(handle);

	return 0;
}


ACE_INT32 VBH::CBlockCacheManager::LoadCacheFile(CCache& cache)
{
	//cache�ļ���·��: ����·��/nFileID.cache
	CDscString strPath(m_strBasePath);
	ACE_stat stat;

	strPath += DSC_FILE_PATH_SPLIT_CHAR;
	strPath += cache.m_nFileID;
	strPath += DEF_BLOCK_CACHE_FILE_SUBFIX;

	//�ж��ļ��Ƿ����
	if (-1 == ACE_OS::stat(strPath.c_str(), &stat))
	{
		DSC_RUN_LOG_ERROR("file:%s is not exist.", strPath.c_str());
		return -1;
	}

	//�򿪰������ڴ���ļ�
	if (-1 == cache.m_shmCache.open(strPath.c_str(), cache.m_nShareMemLen))
	{
		int nLastError = ACE_OS::last_error();
		DSC_RUN_LOG_ERROR("open share memory failed, file name:%s, last error code:%d, last error reason:%s.", strPath.c_str(), nLastError, ACE_OS::strerror(nLastError));

		return -1;
	}

	cache.m_pShareMem = (char*)cache.m_shmCache.malloc();
	if (NULL == cache.m_pShareMem)
	{
		DSC_RUN_LOG_ERROR("alloc memory for memory map file failed, file name:%s.", strPath.c_str());
		cache.m_shmCache.close();

		return -1;
	}

	cache.m_pHeader = (CCache::CFileHeader*)cache.m_pShareMem;

	return 0;
}

