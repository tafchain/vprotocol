#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include "dsc/storage/b_tree/b_tree.h"
#include "dsc/dsc_app_mng.h"
#include "dsc/codec/json_codec/json_codec.h"
#include "dsc/codec/json_codec/json_codec_bind_macros.h"

#define LOOP_NUM 100000

class CBtreeValue
{
public:
	bool operator== (const CBtreeValue& rBtreeValue) const;
	ACE_INT32 Compare(const CBtreeValue& rBtreeValue) const;

public:
	DSC_BIND_ATTR(m_nUserIdNumber);

public:
	ACE_INT64 m_nUserIdNumber;
};

class CDemoJsonNestMsg
{
public:
	JSON_BIND_ATTR(m_lstAttr, m_strAttr, m_dtAttr);

public:
	DSC::CDscList<ACE_INT32> m_lstAttr;
	CDscString m_strAttr;
	DSC::CDscDateTime m_dtAttr;
};

class CDemoJsonMsg
{
public:
	JSON_BIND_ATTR(m_demoJsonNestMsg, m_lstAttr, m_strAttr, m_dtAttr);

public:
	CDemoJsonNestMsg m_demoJsonNestMsg;
	DSC::CDscList<ACE_INT32> m_lstAttr;
	CDscString m_strAttr;
	DSC::CDscDateTime m_dtAttr;
};

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	CDscAppManager* pDscServerAppManager = ::new(std::nothrow) CDscAppManager;

	pDscServerAppManager->SetNodeType(4520);
	if (pDscServerAppManager->Init(argc, argv))
	{
		ACE_OS::printf("dsc app init failed, now exit!\n");
		pDscServerAppManager->Exit();
		delete pDscServerAppManager;

		return -1;
	}

	CDscBTree<CBtreeValue, CBtreeValue>* pTestBTree = new CDscBTree<CBtreeValue, CBtreeValue>;
	CDscBTree<CBtreeValue, CBtreeValue>& testBTree = *pTestBTree;

	CDscString strStoragePathName;

	strStoragePathName = pDscServerAppManager->GetWorkRoot();
	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
	strStoragePathName += "storage";
	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;

	strStoragePathName += "btree_u";
	strStoragePathName += '_';
	strStoragePathName += 1;
	strStoragePathName += '_';
	strStoragePathName += 1;

	if (testBTree.Open(strStoragePathName))
	{
		ACE_OS::printf("call Open() failed, strStoragePathName:%s!", strStoragePathName.c_str());

		return -1;
	}
	else
	{
		int i = 0;
		CBtreeValue testKey;
		CBtreeValue testValue;
		CBtreeValue* pTestValue;
		ACE_Time_Value start;
		ACE_Time_Value stop;
		ACE_Time_Value duration;
	
		for (int jjj = 0; jjj < 1024; ++jjj)
		{
			start = ACE_OS::gettimeofday();
			testBTree.BeginTransaction();
			for (i = 0; i < LOOP_NUM; ++i)
			{
				testKey.m_nUserIdNumber = i;
				testValue.m_nUserIdNumber = i;
				if (!testBTree.Insert(testKey, testValue))
				{
					printf("insert error, key:%lld, line:%d\n", testKey.m_nUserIdNumber, __LINE__);
				}
			}
			testBTree.CommitteTransaction();
			stop = ACE_OS::gettimeofday();
			duration = stop - start;
			ACE_OS::printf("insert time:%d\n", duration.msec());

			start = ACE_OS::gettimeofday();
			for (i = 0; i < LOOP_NUM; ++i)
			{
				testKey.m_nUserIdNumber = i;
				pTestValue = testBTree.Search(testKey);
				if (pTestValue)
				{
					if (testKey == *pTestValue)
					{
						//printf("found:%d, line:%d\n", testArray[i], __LINE__);
					}
					else
					{
						printf("search error, key:%lld, line:%d\n", testKey.m_nUserIdNumber, __LINE__);
					}
				}
				else
				{
					printf("not found, key:%lld, line:%d\n", testKey.m_nUserIdNumber, __LINE__);
				}
			}
			stop = ACE_OS::gettimeofday();
			duration = stop - start;
			ACE_OS::printf("select time:%d\n", duration.msec());

			start = ACE_OS::gettimeofday();
			testBTree.BeginTransaction();
			for (i = 0; i < LOOP_NUM; ++i)
			{
				testKey.m_nUserIdNumber = i;
				testValue.m_nUserIdNumber = ++i;
				if (testBTree.Update(testKey, testValue))
				{				
				}
				else
				{
					printf("update failed, key:%lld, line:%d\n", testKey.m_nUserIdNumber, __LINE__);
				}
			}
			testBTree.CommitteTransaction();
			stop = ACE_OS::gettimeofday();
			duration = stop - start;
			ACE_OS::printf("update time:%d\n", duration.msec());

			start = ACE_OS::gettimeofday();
			testBTree.BeginTransaction();
			for (i = 0; i < LOOP_NUM; ++i)
			{
				testKey.m_nUserIdNumber = i;
				if (!testBTree.Remove(testKey))
				{
					printf("delete failed, key:%lld, line:%d\n", testKey.m_nUserIdNumber, __LINE__);
				}
			}
			testBTree.CommitteTransaction();
			stop = ACE_OS::gettimeofday();
			duration = stop - start;
			ACE_OS::printf("delete time:%d\n", duration.msec());

			start = ACE_OS::gettimeofday();
			for (i = 0; i < LOOP_NUM; ++i)
			{
				testKey.m_nUserIdNumber = i;
				if (testBTree.Search(testKey))
				{
					printf("found, key:%lld, line:%d\n", testKey.m_nUserIdNumber, __LINE__);
				}
				else
				{
				}
			}
			stop = ACE_OS::gettimeofday();
			duration = stop - start;
			ACE_OS::printf("select time:%d\n", duration.msec());
		}

		testBTree.Close();
		delete pTestBTree;

		return -1;
	}

	delete pDscServerAppManager;

	ACE_OS::printf("dsc app terminated!\n");

	return 0;
}

//int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
//{
//	CDscAppManager* pDscServerAppManager = ::new(std::nothrow) CDscAppManager;
//
//	pDscServerAppManager->SetNodeType(4520);
//	if (pDscServerAppManager->Init(argc, argv))
//	{
//		ACE_OS::printf("dsc app init failed, now exit!\n");
//		pDscServerAppManager->Exit();
//		delete pDscServerAppManager;
//
//		return -1;
//	}
//
//	CDscBTree*  pTestBTree = new CDscBTree(256);
//	CDscBTree& testBTree = *pTestBTree;
//
//	CDscString strStoragePathName;
//
//	strStoragePathName = pDscServerAppManager->GetWorkRoot();
//	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
//	strStoragePathName += "storage";
//	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
//
//	strStoragePathName += DEF_USER_WORLD_STATE_PATH_NAME;
//	strStoragePathName += '_';
//	strStoragePathName += 1;
//	strStoragePathName += '_';
//	strStoragePathName += 1;
//
//	if (testBTree.Open(strStoragePathName))
//	{
//		ACE_OS::printf("call Open() failed, strStoragePathName:%s!", strStoragePathName.c_str());
//
//		return -1;
//	}
//	else
//	{
//		CBtreeValue* testArray = new CBtreeValue[LOOP_NUM];
//		int i = 0;
//		CBtreeValue testKey;
//		ACE_Time_Value start;
//		ACE_Time_Value stop;
//		ACE_Time_Value duration;
//		char* pBuf = NULL;
//		size_t nBuf = 0;
//		DSC::CDscBlob dbKey;
//		DSC::CDscBlob dbValue;
//
//		srand((unsigned int)time(0));
//
//		for (int jjj = 0; jjj < 1024; ++jjj)
//		{
//			memset(testArray, 0, sizeof(int) * LOOP_NUM);
//			start = ACE_OS::gettimeofday();
//			testBTree.BeginTransaction();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				while (true)
//				{
//
//					testKey.m_nUserIdNumber = rand() + 1;
//					DSC::Encode(testKey, pBuf, nBuf);
//					
//					dbKey.Set(pBuf, nBuf);
//					dbValue.Set(pBuf, nBuf);
//
//					if (testBTree.Insert(dbKey, dbValue))
//					{//²»ÔÊÐíÖØ¸´
//						testArray[i] = testKey;
//						DSC_THREAD_SIZE_FREE(pBuf, nBuf);
//						break;
//					}
//					else
//					{
//						DSC_THREAD_SIZE_FREE(pBuf, nBuf);
//						continue;
//					}
//				}
//			}
//			testBTree.CommitteTransaction();
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("insert time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				DSC::Encode(testArray[i], pBuf, nBuf);
//
//				dbKey.Set(pBuf, nBuf);
//
//				if (testBTree.Search(dbKey, dbValue))
//				{
//					if (DSC::Decode(testKey, dbValue.GetBuffer(), dbValue.GetSize()))
//					{
//						printf("decode error, key:%lld, line:%d\n", testArray[i].m_nUserIdNumber, __LINE__);
//					}
//					else
//					{
//						if (testKey == testArray[i])
//						{
//							//printf("found:%d, line:%d\n", testArray[i], __LINE__);
//						}
//						else
//						{
//							printf("search error, key:%lld, line:%d\n", testArray[i].m_nUserIdNumber, __LINE__);
//						}
//					}
//				}
//				else
//				{
//					printf("not found, key:%lld, line:%d\n", testArray[i].m_nUserIdNumber, __LINE__);
//				}
//				DSC_THREAD_SIZE_FREE(pBuf, nBuf);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("select time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			testBTree.BeginTransaction();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				DSC::Encode(testArray[i], pBuf, nBuf);
//
//				dbKey.Set(pBuf, nBuf);
//
//				if (!testBTree.Remove(dbKey))
//				{
//					printf("delete failed, key:%lld, line:%d\n", testArray[i].m_nUserIdNumber, __LINE__);
//				}
//				DSC_THREAD_SIZE_FREE(pBuf, nBuf);
//			}
//			testBTree.CommitteTransaction();
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("update time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				DSC::Encode(testArray[i], pBuf, nBuf);
//
//				dbKey.Set(pBuf, nBuf);
//
//				if (testBTree.Search(dbKey, dbValue))
//				{
//					printf("found, key:%lld, line:%d\n", testArray[i].m_nUserIdNumber, __LINE__);
//				}
//				else
//				{
//				}
//				DSC_THREAD_SIZE_FREE(pBuf, nBuf);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("select time:%d\n", duration.msec());
//		}
//
//		delete[] testArray;
//		testBTree.Close();
//		delete pTestBTree;
//
//		return -1;
//	}
//
//	delete pDscServerAppManager;
//
//	ACE_OS::printf("dsc app terminated!\n");
//
//	return 0;
//}

//int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
//{
//	CDscAppManager* pDscServerAppManager = ::new(std::nothrow) CDscAppManager;
//
//	pDscServerAppManager->SetNodeType(4520);
//	if (pDscServerAppManager->Init(argc, argv))
//	{
//		ACE_OS::printf("dsc app init failed, now exit!\n");
//		pDscServerAppManager->Exit();
//		delete pDscServerAppManager;
//
//		return -1;
//	}
//
//	CDscBTree* m_pTestBTree = new CDscBTree(256);
//	CDscBTree& m_testBTree = *m_pTestBTree;
//	CDscString strStoragePathName;
//
//	strStoragePathName = pDscServerAppManager->GetWorkRoot();
//	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
//	strStoragePathName += "storage";
//	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
//
//	strStoragePathName += DEF_USER_WORLD_STATE_PATH_NAME;
//	strStoragePathName += '_';
//	strStoragePathName += pDscServerAppManager->GetNodeType();
//	strStoragePathName += '_';
//	strStoragePathName += pDscServerAppManager->GetNodeID();
//
//	if (m_testBTree.Open(strStoragePathName))
//	{
//		ACE_OS::printf("call Open() failed, strStoragePathName:%s\n!", strStoragePathName.c_str());
//
//		return -1;
//	}
//	else
//	{
//		int i = 0;
//		CBtreeValue testKey;
//		CBtreeValue* pTestKeyResult;
//		CBtreeValue testKeyResult;
//		ACE_Time_Value start;
//		ACE_Time_Value stop;
//		ACE_Time_Value duration;
//
//		for (int jjj = 0; jjj < 16; ++jjj)
//		{
//			start = ACE_OS::gettimeofday();
//			
//			m_testBTree.BeginTransaction();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				testKey.m_nUserIdNumber = i;
//				testKey.m_nPageID = testKey.m_nUserIdNumber;
//				testKey.m_nPageBlockID = testKey.m_nUserIdNumber;
//
//				m_testBTree.Insert(i, testKey);
//			}
//			m_testBTree.CommitteTransaction();
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("insert time:%d\n", duration.msec());
//
////=========================================================================================================================
//			start = ACE_OS::gettimeofday();
//			
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				pTestKeyResult = m_testBTree.Search(i);
//				if (!pTestKeyResult)
//				{
//					printf("not found, key:%d, line:%d\n", i, __LINE__);
//				}
//			}
//			
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("select time:%d\n", duration.msec());
//
////=========================================================================================================================
//			start = ACE_OS::gettimeofday();
//
//			m_testBTree.BeginTransaction();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				testKeyResult.m_nUserIdNumber = i;
//				testKeyResult.m_nPageID = i;
//				testKeyResult.m_nPageBlockID = i;
//				if (!m_testBTree.Update(i, testKeyResult))
//				{
//					printf("update failed, key:%d, line:%d\n", i, __LINE__);
//				}
//			}
//			m_testBTree.CommitteTransaction();
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("update time:%d\n", duration.msec());
//
////=========================================================================================================================
//			start = ACE_OS::gettimeofday();
//
//			m_testBTree.BeginTransaction();
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				if (!m_testBTree.Remove(i))
//				{
//					printf("delete failed, key:%d, line:%d\n", i, __LINE__);
//				}
//			}
//			m_testBTree.CommitteTransaction();
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("delete time:%d\n", duration.msec());
//
////=========================================================================================================================
//			for (i = 0; i < LOOP_NUM; ++i)
//			{
//				pTestKeyResult = m_testBTree.Search(i);
//				if (pTestKeyResult)
//				{
//					printf("found, key:%d, line:%d\n", i, __LINE__);
//				}
//				else
//				{
//					//printf("not found, key:%lld, line:%d\n", testArray[i].m_nUserIdNumber, __LINE__);
//				}
//			}
//		}
//
//		m_testBTree.Close();
//		delete m_pTestBTree;
//
//		return -1;
//	}
//
//	delete pDscServerAppManager;
//
//	ACE_OS::printf("dsc app terminated!\n");
//
//	return 0;
//}

inline bool CBtreeValue::operator== (const CBtreeValue& rBtreeValue) const
{
	return m_nUserIdNumber == rBtreeValue.m_nUserIdNumber;
}

inline ACE_INT32 CBtreeValue::Compare(const CBtreeValue& rBtreeValue) const
{
	if (m_nUserIdNumber < rBtreeValue.m_nUserIdNumber)
	{
		return -1;
	}
	else if (m_nUserIdNumber > rBtreeValue.m_nUserIdNumber)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
