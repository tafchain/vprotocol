#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include <stack>
#include <functional>

#include "dsc/mem_mng/dsc_allocator.h"
#include "dsc/dsc_log.h"

#include "demo_server/demo_server_appmanager.h"
#include "common/comm_id_def.h"

//#include "dsc/mem_mng/dsc_stl_type.h"
//#include "dsc/container/dsc_search_array.h"
//#include "dsc/container/dsc_hash_map.h"
//#include "dsc/container/bare_hash_map.h"
//#include "dsc/container/rb_tree.h"
//#include "dsc/container/hash_map.h"
//
//class CNode
//{
//public:
//	ACE_UINT32 m_nKey;
//	int m_value;
//
//public:
//	CNode* m_pPrev;
//	CNode* m_pNext;
//};
//
//class CTreeNode
//{
//public:
//	int m_value;
//
//public:
//	void Release(void)
//	{
//		CTreeNode* pTreeNode = this;
//
//		DSC_THREAD_TYPE_DEALLOCATE(pTreeNode);
//	}
//
//public://rb tree管理需要属性
//	int m_color;
//	CTreeNode* m_parent;
//	CTreeNode* m_left;
//	CTreeNode* m_right;
//	int m_key;
//};
//
//class CHashRbNode
//{
//public:
//	int m_value;
//
//public:
//	void Release(void)
//	{
//		CHashRbNode* pTreeNode = this;
//
//		DSC_THREAD_TYPE_DEALLOCATE(pTreeNode);
//	}
//
//	static ACE_UINT32 Hash(const CDscString& key)
//	{
//		return DSC::Hash(key.c_str(), key.size()) >> 32;
//	}
//
//	static ACE_UINT32 Hash(const ACE_UINT32& key)
//	{
//		return DSC::Hash(key);
//	}
//public://rb tree管理需要属性
//	int m_color;
//	CHashRbNode* m_parent;
//	CHashRbNode* m_left;
//	CHashRbNode* m_right;
//	CDscString m_key;
//};
//
//class CHashRbNodeInt
//{
//public:
//	int m_value;
//
//public:
//	void Release(void)
//	{
//		CHashRbNodeInt* pTreeNode = this;
//
//		DSC_THREAD_TYPE_DEALLOCATE(pTreeNode);
//	}
//
//	static ACE_UINT32 Hash(const ACE_UINT32& key)
//	{
//		return DSC::Hash(key);
//	}
//public://rb tree管理需要属性
//	int m_color;
//	CHashRbNodeInt* m_parent;
//	CHashRbNodeInt* m_left;
//	CHashRbNodeInt* m_right;
//	ACE_UINT32 m_key;
//};
//
//#define LOOP_NUM 65536
//#define FIND_LOOP_NUM 10
//
//int k;
//
int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
//	char szuuid[32];
//
//	DSC::Uuid(szuuid);
//	InitThreadAllocator();
//
//	ACE_Time_Value start;
//	ACE_Time_Value stop;
//	ACE_Time_Value duration;
//
//	srand(time(0));
//
//	{
//		int i = rand();
//		int j = rand();
//		start = ACE_OS::gettimeofday();
//		for (int i = 0; i < 1000000000; ++i)
//		{
//			k = i < j ? 0 : 1;
//		}
//		stop = ACE_OS::gettimeofday();
//		duration = stop - start;
//		ACE_OS::printf("< time:%d, k:%d\n", duration.msec(), k);
//	}
//
//	{
//		int i = rand();
//		int j = rand();
//		start = ACE_OS::gettimeofday();
//		for (int i = 0; i < 1000000000; ++i)
//		{
//			k = i <= j ? 0 : 1;
//		}
//		stop = ACE_OS::gettimeofday();
//		duration = stop - start;
//		ACE_OS::printf("<= time:%d, k:%d\n", duration.msec(), k);
//	}
//
//	{
//		int i = rand();
//		int j = rand();
//		start = ACE_OS::gettimeofday();
//		for (int i = 0; i < 100000000; ++i)
//		{
//			k = i + j;
//		}
//		stop = ACE_OS::gettimeofday();
//		duration = stop - start;
//		ACE_OS::printf("+ time:%d, k:%d\n", duration.msec(), k);
//	}
//
//	{
//		int i = rand();
//		int j = rand();
//		start = ACE_OS::gettimeofday();
//		for (int i = 0; i < 100000000; ++i)
//		{
//			k = i * j;
//		}
//		stop = ACE_OS::gettimeofday();
//		duration = stop - start;
//		ACE_OS::printf("* time:%d, k:%d\n", duration.msec(), k);
//	}
//
//	{
//		int i = rand();
//		int j = rand();
//		start = ACE_OS::gettimeofday();
//		for (int i = 0; i < 100000000; ++i)
//		{
//			k = i / j;
//		}
//		stop = ACE_OS::gettimeofday();
//		duration = stop - start;
//		ACE_OS::printf("/ time:%d, k:%d\n", duration.msec(), k);
//	}
//
//	{
//		int i = rand();
//		int j = rand();
//		int k;
//		start = ACE_OS::gettimeofday();
//		for (int i = 0; i < 100000000; ++i)
//		{
//			k = i % j;
//		}
//		stop = ACE_OS::gettimeofday();
//		duration = stop - start;
//		ACE_OS::printf("yu time:%d, k:%d\n", duration.msec(), k);
//	}
//	{
//		dsc_set_type(CDscString) string_key_set;
//		dsc_set_type(CDscString)::iterator string_set_it;
//		CDscString s;
//		char c;
//		ACE_INT32 n;
//
//		for (int iii = 0; iii < LOOP_NUM;)
//		{
//			n = rand();
//			if (n < 0)
//			{
//				n = -n;
//			}
//			if (n == 0)
//			{
//				n = 1;
//			}
//
//			n %= 256;
//
//			for (int jj = 0; jj < n; ++jj)
//			{
//				c = rand();
//				s += c;
//			}
//
//			string_set_it = string_key_set.find(s);
//			if (string_set_it == string_key_set.end())
//			{
//				string_key_set.insert(s);
//				++iii;
//			}
//			s.clear();
//		}
//
//		//--------------------------------------------------------------------------------------
//		{
//			CHashRbMap<CDscString, CHashRbNode, 16>* hashMap = new CHashRbMap<CDscString, CHashRbNode, 16>;
//			CHashRbNode* pHashRbNode;
//
//			start = ACE_OS::gettimeofday();
//			for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pHashRbNode);
//				pHashRbNode->m_key = *string_set_it;
//				hashMap->Insert(*string_set_it, pHashRbNode);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc string hash rb map insert time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//				{
//					pHashRbNode = hashMap->Find(*string_set_it);
//					if (!pHashRbNode)
//					{
//						ACE_OS::printf("dsc string hash rb map find eror, key:%d\n", *string_set_it);
//					}
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc string hash rb map find time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//			{
//				pHashRbNode = hashMap->Erase(*string_set_it);
//				if (pHashRbNode)
//				{
//					DSC_THREAD_TYPE_DEALLOCATE(pHashRbNode);
//				}
//				else
//				{
//					ACE_OS::printf("dsc string hash rb map find eror, key:%d\n", *string_set_it);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc string hash rb map delete time:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//
//			delete hashMap;
//		}
//		//--------------------------------------------------------------------------------------
//		//--------------------------------------------------------------------------------------
//		{
//			CRbTree<CDscString, CHashRbNode> rbMap;
//			CHashRbNode* pHashRbNode;
//
//			start = ACE_OS::gettimeofday();
//			for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pHashRbNode);
//				pHashRbNode->m_key = *string_set_it;
//				rbMap.Insert(pHashRbNode);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc string rb map insert time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//				{
//					pHashRbNode = rbMap.Find(*string_set_it);
//					if (pHashRbNode == rbMap.End())
//					{
//						ACE_OS::printf("dsc string rb map find eror, key:%d\n", *string_set_it);
//					}
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc string rb map find time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//			{
//				pHashRbNode = rbMap.Erase(*string_set_it);
//				if (pHashRbNode == rbMap.End())
//				{
//					ACE_OS::printf("dsc string rb map find eror, key:%d\n", *string_set_it);
//				}
//				else
//				{
//					DSC_THREAD_TYPE_DEALLOCATE(pHashRbNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc string rb map delete time:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//
//		//--------------------------------------------------------------------------------------
//		{
//			dsc_map_type(CDscString, CHashRbNode*) stl_rb;
//			dsc_map_type(CDscString, CHashRbNode*)::iterator stl_rb_it;
//			CHashRbNode* pHashRbNode;
//
//			start = ACE_OS::gettimeofday();
//			for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pHashRbNode);
//				pHashRbNode->m_key = *string_set_it;
//				stl_rb.insert(dsc_map_type(CDscString, CHashRbNode*)::value_type(*string_set_it, pHashRbNode));
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl string rb map insert time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//				{
//					stl_rb_it = stl_rb.find(*string_set_it);
//					if (stl_rb_it == stl_rb.end())
//					{
//						ACE_OS::printf("stl string rb map find eror, key:%d\n", *string_set_it);
//					}
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl string rb map find time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (string_set_it = string_key_set.begin(); string_set_it != string_key_set.end(); ++string_set_it)
//			{
//				stl_rb_it = stl_rb.find(*string_set_it);
//				if (stl_rb_it == stl_rb.end())
//				{
//					ACE_OS::printf("stl string rb map find eror, key:%d\n", *string_set_it);
//				}
//				else
//				{
//					pHashRbNode = stl_rb_it->second;
//					stl_rb.erase(stl_rb_it);
//					DSC_THREAD_TYPE_DEALLOCATE(pHashRbNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl string rb map delete time:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//	}
//	//--------------------------------------------------------------------------------------
//	//--------------------------------------------------------------------------------------
//	{
//		dsc_set_type(ACE_INT32) key_set;
//		dsc_set_type(ACE_INT32)::iterator set_it;
//		ACE_INT32 n;
//
//		for (int iii = 0; iii < LOOP_NUM;)
//		{
//			n = rand();
//			set_it = key_set.find(n);
//			if (set_it == key_set.end())
//			{
//				key_set.insert(n);
//				++iii;
//			}
//		}
//
//		{
//			CRbTree<int, CTreeNode> tree;
//			CTreeNode* pTreeNode;
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pTreeNode);
//				pTreeNode->m_key = *set_it;
//				tree.Insert(pTreeNode);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc int rb map insert time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//				{
//					pTreeNode = tree.Find(*set_it);
//					if (pTreeNode == tree.End())
//					{
//						ACE_OS::printf("dsc int rb map find eror, key:%d\n", *set_it);
//					}
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc int rb map find time:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				pTreeNode = tree.Erase(*set_it);
//				if (pTreeNode == tree.End())
//				{
//					ACE_OS::printf("dsc int rb map erase eror, key:%d\n", *set_it);
//				}
//				else
//				{
//					DSC_THREAD_TYPE_DEALLOCATE(pTreeNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc int rb map delete time:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//		//--------------------------------------------------------------------------------------
//		{
//			dsc_map_type(ACE_UINT32, CNode*) stl_rb_map;
//			dsc_map_type(ACE_UINT32, CNode*)::iterator stl_rb_it;
//			CNode* pNode;
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pNode);
//				stl_rb_map.insert(dsc_map_type(ACE_UINT32, CNode*)::value_type(*set_it, pNode));
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl rb map insert:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//				{
//					stl_rb_it = stl_rb_map.find(*set_it);
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl rb map find:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				stl_rb_it = stl_rb_map.find(*set_it);
//				if (stl_rb_it != stl_rb_map.end())
//				{
//					pNode = stl_rb_it->second;
//					stl_rb_map.erase(stl_rb_it);
//					DSC_THREAD_TYPE_DEALLOCATE(pNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl rb map delete:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//		//	//--------------------------------------------------------------------------------------
//		{
//			dsc_unordered_map_type(ACE_UINT32, CNode*) stl_hash_map(65536);
//			dsc_unordered_map_type(ACE_UINT32, CNode*)::iterator hash_it;
//			CNode* pNode;
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pNode);
//				stl_hash_map.insert(dsc_unordered_map_type(ACE_UINT32, CNode*)::value_type(*set_it, pNode));
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl hash map insert:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//				{
//					hash_it = stl_hash_map.find(*set_it);
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl has map find:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				hash_it = stl_hash_map.find(*set_it);
//				if (hash_it != stl_hash_map.end())
//				{
//					pNode = hash_it->second;
//					stl_hash_map.erase(hash_it);
//					DSC_THREAD_TYPE_DEALLOCATE(pNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("stl hash map delete:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//		////--------------------------------------------------------------------------------------
//		{
//			CDscHashMap<CNode, 16>* dsc_map = new CDscHashMap<CNode, 16>;
//			CNode* pNode;
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pNode);
//				dsc_map->Insert(*set_it, pNode);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc map insert:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//				{
//					pNode = dsc_map->Find(*set_it);
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc map find:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				pNode = dsc_map->Erase(*set_it);
//				if (pNode)
//				{
//					DSC_THREAD_TYPE_DEALLOCATE(pNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("dsc map delete:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//			delete dsc_map;
//		}
//		////--------------------------------------------------------------------------------------
//		{
//			CBareHashMap<CNode, 16>* bare_map = new CBareHashMap<CNode, 16>;
//			CNode* pNode;
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pNode);
//				bare_map->Insert(*set_it, pNode);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("bare map insert:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//				{
//					pNode = bare_map->Find(*set_it);
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("bare map find:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				pNode = bare_map->Erase(*set_it);
//				if (pNode)
//				{
//					DSC_THREAD_TYPE_DEALLOCATE(pNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("bare map delete:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//
//		////--------------------------------------------------------------------------------------
//		{
//			CHashRbMap<ACE_UINT32, CHashRbNodeInt, 16>* hashMap = new CHashRbMap<ACE_UINT32, CHashRbNodeInt, 16>;
//			CHashRbNodeInt* pNode;
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				DSC_THREAD_TYPE_ALLOCATE(pNode);
//				pNode->m_key = *set_it;
//				hashMap->Insert(*set_it, pNode);
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("hash rb int map insert:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//
//			for (int iii = 0; iii < FIND_LOOP_NUM; ++iii)
//			{
//				for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//				{
//					pNode = hashMap->Find(*set_it);
//				}
//			}
//
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("hash rb int map find:%d\n", duration.msec());
//
//			start = ACE_OS::gettimeofday();
//			for (set_it = key_set.begin(); set_it != key_set.end(); ++set_it)
//			{
//				pNode = hashMap->Erase(*set_it);
//				if (pNode)
//				{
//					DSC_THREAD_TYPE_DEALLOCATE(pNode);
//				}
//			}
//			stop = ACE_OS::gettimeofday();
//			duration = stop - start;
//			ACE_OS::printf("hash rb int map delete:%d\n", duration.msec());
//			ACE_OS::printf("----------------------\n");
//		}
//	}
//
//	ACE_OS::sleep(10000000);
	////======================================================================================================================
printf("uint64 max:%lld\n", std::numeric_limits<ACE_UINT64>::max());
	CDemoServerAppManager* pAppManager = new CDemoServerAppManager;
	if (!pAppManager)
	{
		ACE_OS::printf("failed to new demo server appmanager!");

		return -1;
	}

	pAppManager->SetNodeType(NS_DEMO_ID_DEF::DEMO_SERVER_NODE_TYPE);
	if (pAppManager->Init(argc, argv))
	{
		ACE_OS::printf("demo server init failed, now exit!\n");
		pAppManager->Exit();
		delete pAppManager;

		return -1;
	}

	ACE_OS::printf("demo server init succeed, running...\n");
	pAppManager->Run_Loop();
	delete pAppManager;
	ACE_OS::printf("demo server terminated!\n");

	return 0;
}