#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

#include "committer/plugin/xcm_xcs/const_length_storage.h"
#include "committer/plugin/xcm_xcs/variable_length_storage.h"

#include "dsc/dsc_app_mng.h"

#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_sys_stat.h"

#define BEGIN_TEST(FROMAT, ...)  \
	printf("%s test begin£º " FROMAT "\n", __FUNCTION__, ##__VA_ARGS__);

#define TEST_FAILED(FROMAT, ...)  \
	printf("%s test failed£º " FROMAT "\n", __FUNCTION__, ##__VA_ARGS__);

#define TEST_SUCCESS(FROMAT, ...)  \
	printf("%s test success£º " FROMAT "\n", __FUNCTION__, ##__VA_ARGS__);

#define FORWARD_TEST(FUNC, FROMAT, ...) \
if (-1 == (FUNC)) { TEST_FAILED(FROMAT, ##__VA_ARGS__); return false;}


#define COMMON_BEGIN_TEST \
MyIndex index; \
MyIndex* pIndex = nullptr; \
char *readBuf = nullptr; \
ACE_INT32 nResult; \
CConstLengthStorage indexDB; \
CDscString testfile("F:\\code\\VBH\\src\\test"); \
nResult = indexDB.Open(testfile, cfg); \
if (nResult) \
{ \
	TEST_FAILED("open failed"); \
	return false; \
} \
ACE_UINT64 nIndexNum = indexDB.GetLastItemID() + 1;

struct MyIndex
{
	ACE_UINT64 m_nA;
	ACE_UINT64 m_nB;
	ACE_UINT32 m_nC;
};

CConstLengthStorage::CCfgParam cfg = { 20, 6, 4, 23 };


bool test1()
{
	BEGIN_TEST("append 1 ¸ö index");
	COMMON_BEGIN_TEST;


	indexDB.BeginTransaction();

	index.m_nA = nIndexNum;
	index.m_nB = nIndexNum + 1;
	index.m_nC = nIndexNum + 2;
	FORWARD_TEST(indexDB.Append(nIndexNum, (char*)& index), "append failed");

	indexDB.CommitteTransaction();

	FORWARD_TEST(indexDB.Read(nIndexNum, readBuf), "read failed");
	pIndex = (MyIndex*)readBuf;
	if (pIndex->m_nA != nIndexNum || pIndex->m_nB != (nIndexNum + 1) || pIndex->m_nC != (nIndexNum + 2))
	{
		TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
			nIndexNum, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, nIndexNum, (nIndexNum + 1), (nIndexNum + 2));
		return false;
	}

	TEST_SUCCESS("");

	return true;
}

bool test2()
{
	BEGIN_TEST("append 128 ¸ö index =>  append 100 ¸ö index");
	COMMON_BEGIN_TEST;

	auto firstIndexNum = nIndexNum;

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = nIndexNum; i < nIndexNum + 128; i++)
	{
		index.m_nA = i;
		index.m_nB = i + 1;
		index.m_nC = i + 2;
		FORWARD_TEST(indexDB.Append(i, (char*)& index), "append failed");
	}

	indexDB.CommitteTransaction();

	nIndexNum = indexDB.GetLastItemID() + 1;

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = nIndexNum; i < nIndexNum + 700; i++)
	{
		index.m_nA = i;
		index.m_nB = i + 1;
		index.m_nC = i + 2;
		FORWARD_TEST(indexDB.Append(i, (char*)& index), "append failed");
	}

	indexDB.CommitteTransaction();

	for (ACE_UINT64 i = firstIndexNum; i < firstIndexNum + 828; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i || pIndex->m_nB != (i + 1) || pIndex->m_nC != (i + 2))
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i, (i + 1), (i + 2));
			return false;
		}
	}

	TEST_SUCCESS("");

	return true;
}

bool test3()
{
	BEGIN_TEST("update 1-128  index");
	COMMON_BEGIN_TEST;

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = 1; i < 138; i++)
	{
		index.m_nA = i * 2;
		index.m_nB = (i + 1) * 2;
		index.m_nC = (i + 2) * 2;
		FORWARD_TEST(indexDB.Update(i, (char*)& index), "update failed");
	}

	indexDB.CommitteTransaction();

	for (ACE_UINT64 i = 1; i < 138; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i * 2 || pIndex->m_nB != (i + 1) * 2 || pIndex->m_nC != (i + 2) * 2)
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i * 2, (i + 1) * 2, (i + 2) * 2);
			return false;
		}
	}

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = 1; i < 138; i++)
	{
		index.m_nA = i;
		index.m_nB = (i + 1);
		index.m_nC = (i + 2);
		FORWARD_TEST(indexDB.Update(i, (char*)& index), "update failed");
	}

	indexDB.CommitteTransaction();

	TEST_SUCCESS("");

	return true;
}

bool test4()
{
	BEGIN_TEST("update 1-128  index and RollBack");
	COMMON_BEGIN_TEST;
	indexDB.BeginTransaction();

	for (ACE_UINT64 i = 1; i < 128; i++)
	{
		index.m_nA = i * 2;
		index.m_nB = (i + 1) * 2;
		index.m_nC = (i + 2) * 2;
		FORWARD_TEST(indexDB.Update(i, (char*)& index), "update failed");
	}

	indexDB.RollBackTransaction();

	for (ACE_UINT64 i = 1; i < 128; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i || pIndex->m_nB != (i + 1) || pIndex->m_nC != (i + 2))
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i, (i + 1), (i + 2));
			return false;
		}
	}

	TEST_SUCCESS("");

	return true;
}

bool test5()
{
	BEGIN_TEST("append 128 index and update 1-128  index alternant ");
	COMMON_BEGIN_TEST;
	indexDB.BeginTransaction();

	for (ACE_UINT64 i = nIndexNum; i < nIndexNum + 178; i++)
	{
		index.m_nA = i;
		index.m_nB = (i + 1);
		index.m_nC = (i + 2);
		FORWARD_TEST(indexDB.Append(i, (char*)& index), "append failed");

		index.m_nA = (i - nIndexNum + 1) * 2;
		index.m_nB = ((i - nIndexNum + 1 + 1)) * 2;
		index.m_nC = ((i - nIndexNum + 1 + 2)) * 2;
		FORWARD_TEST(indexDB.Update(i - nIndexNum + 1, (char*)& index), "update failed");
	}

	indexDB.CommitteTransaction();

	for (ACE_UINT64 i = 1; i < 178; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i * 2 || pIndex->m_nB != (i + 1) * 2 || pIndex->m_nC != (i + 2) * 2)
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i * 2, (i + 1) * 2, (i + 2) * 2);
			return false;
		}
	}

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = 1; i <=	 178; i++)
	{
		index.m_nA = i;
		index.m_nB = (i + 1);
		index.m_nC = (i + 2);
		FORWARD_TEST(indexDB.Update(i, (char*)& index), "update failed");
	}

	indexDB.CommitteTransaction();

	for (ACE_UINT64 i = nIndexNum; i < nIndexNum + 178; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i || pIndex->m_nB != (i + 1) || pIndex->m_nC != (i + 2))
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i, (i + 1), (i + 2));
			return false;
		}
	}

	TEST_SUCCESS("");

	return true;
}

bool test6()
{
	BEGIN_TEST("read invalid index ");
	COMMON_BEGIN_TEST;

	auto r = indexDB.Read(100000, readBuf);

	if (r == 0)
	{
		TEST_FAILED("read invalid index 100000 return true");
		return false;
	}

	TEST_SUCCESS("");

	return true;
}

bool test7()
{
	BEGIN_TEST("append invalid index ");
	COMMON_BEGIN_TEST;

	indexDB.BeginTransaction();

	index.m_nA = nIndexNum;
	index.m_nB = nIndexNum + 1;
	index.m_nC = nIndexNum + 2;
	auto r = indexDB.Append(nIndexNum + 10, (char*)& index);

	if (r == 0)
	{
		TEST_FAILED("append invalid index %llu return true", nIndexNum + 10);
		return false;
	}

	indexDB.CommitteTransaction();
	TEST_SUCCESS("");

	return true;
}

bool test8()
{
	BEGIN_TEST("update invalid index ");
	COMMON_BEGIN_TEST;

	indexDB.BeginTransaction();

	index.m_nA = nIndexNum;
	index.m_nB = nIndexNum + 1;
	index.m_nC = nIndexNum + 2;
	auto r = indexDB.Update(nIndexNum + 10, (char*)& index);

	if (r == 0)
	{
		TEST_FAILED("update invalid index %llu return true", nIndexNum + 10);
		return false;
	}

	indexDB.CommitteTransaction();
	TEST_SUCCESS("");

	return true;
}

bool test9()
{
	BEGIN_TEST("update 1-128 and crash ");
	COMMON_BEGIN_TEST;

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = 1; i < 128; i++)
	{
		index.m_nA = i * 2;
		index.m_nB = (i + 1) * 2;
		index.m_nC = (i + 2) * 2;
		FORWARD_TEST(indexDB.Update(i, (char*)& index), "update failed");

		if (i == 65)
		{
			abort();
		}
	}

	indexDB.CommitteTransaction();
	TEST_SUCCESS("");

	return true;
}

bool test10()
{
	BEGIN_TEST("read 1-128");
	COMMON_BEGIN_TEST;

	for (ACE_UINT64 i = 1; i < 128; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i || pIndex->m_nB != (i + 1) || pIndex->m_nC != (i + 2))
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i, (i + 1), (i + 2));
			return false;
		}
	}

	TEST_SUCCESS("");

	return true;
}

bool test11()
{
	BEGIN_TEST("commit crash");
	COMMON_BEGIN_TEST;

	printf("now index=%llu \n", nIndexNum);

	indexDB.BeginTransaction();

	for (ACE_UINT64 i = 1; i < 138; i++)
	{
		index.m_nA = i * 2;
		index.m_nB = (i + 1) * 2;
		index.m_nC = (i + 2) * 2;
		FORWARD_TEST(indexDB.Update(i, (char*)& index), "append failed");
	}

	indexDB.CommitteTransaction();

	TEST_SUCCESS("");

	return true;
}

bool test12()
{
	BEGIN_TEST("read all");
	COMMON_BEGIN_TEST;

	for (ACE_UINT64 i = 1; i < nIndexNum; i++)
	{
		FORWARD_TEST(indexDB.Read(i, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;
		if (pIndex->m_nA != i || pIndex->m_nB != (i + 1) || pIndex->m_nC != (i + 2))
		{
			TEST_FAILED("update failed index = %llu, A = %llu, B = %llu, C = %llu (expect  A = %llu, B = %llu, C = %llu)",
				i, pIndex->m_nA, pIndex->m_nB, pIndex->m_nC, i, (i + 1), (i + 2));
			return false;
		}
	}

	TEST_SUCCESS("");

	return true;
}

CVariableLengthStorage::CCfgParam varcfg = { 10, 6, 26 };

#define COMMON_BEGIN_VAR_TEST  \
ACE_INT32 varResult; \
char *varbuf = nullptr; \
int varlength = 0; \
ACE_UINT32 varFileID; \
ACE_UINT64 varOffset; \
CVariableLengthStorage varDB; \
CDscString vartestfile("F:\\code\\VBH\\src\\test"); \
varResult = varDB.Open(vartestfile, varcfg); \
if (varResult) \
{ \
	TEST_FAILED("open failed"); \
	return false; \
} \

bool vartest1(int num)
{
	
	COMMON_BEGIN_TEST;
	COMMON_BEGIN_VAR_TEST;
#if 1
	BEGIN_TEST("var append %d %llu", num, nIndexNum);
	//while (true)
	{

		nIndexNum = indexDB.GetLastItemID() + 1;
		varDB.BeginTransaction();
		indexDB.BeginTransaction();

		for (ACE_UINT64 i = nIndexNum; i < nIndexNum + num; i++)
		{

			varlength = i % 5000;
			varbuf = (char*)realloc(varbuf, varlength);
			for (int k = 0; k < varlength; k++) {
				varbuf[k] = k % 120;
			}
			if (varDB.Append(varFileID, varOffset, varbuf, varlength)) {
				TEST_FAILED("varDB.Append failed");
				return false;
			}

			index.m_nA = varlength;
			index.m_nB = varOffset;
			index.m_nC = varFileID;
			
			if (indexDB.Append(i, (char*)& index)) {
				TEST_FAILED("indexDB.Append failed");
				return false;
			}
			
		}

		varDB.CommitteTransaction();
		indexDB.CommitteTransaction();


	}

#endif



	
	int rnum = rand() % 100 + 1;

	BEGIN_TEST("var read %d %llu", rnum, nIndexNum);

	for (int i = 0; i < rnum; i++) {
		auto idx = rand() % (nIndexNum);
		if (idx == 0) {
			continue;
		}
		//auto idx = i;
		FORWARD_TEST(indexDB.Read(idx, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;

		if (pIndex->m_nA != idx % 5000) {
			TEST_FAILED("pIndex->m_nA (%llu) != idx(%llu) \% 5000", pIndex->m_nA, idx);
			return false;
		}

		varbuf = (char*)realloc(varbuf, pIndex->m_nA);
		FORWARD_TEST(varDB.Read(pIndex->m_nC, pIndex->m_nB, varbuf, pIndex->m_nA), "var read failed");
		for (int k = 0; k < pIndex->m_nA; k++) {
			if (varbuf[k] != k % 120) {
				TEST_FAILED("varbuf[k] != k \% 120,,,`%d(%d)   idx=%llu,fileid=%u, offset = %llu, length= %llu", varbuf[k] , k, idx, pIndex->m_nC, pIndex->m_nB, pIndex->m_nA);
				return false;
			}
		}

	}


	












	free(varbuf);
	TEST_SUCCESS("");
	return true;
}

bool varTestRead()
{
	COMMON_BEGIN_TEST;
	COMMON_BEGIN_VAR_TEST;

	

	int num = rand() % 100 + 1;

	BEGIN_TEST("var read %d %llu", num, nIndexNum);

	for (int i = 0; i < num; i++) {
		auto idx = rand() % nIndexNum;

		FORWARD_TEST(indexDB.Read(idx, readBuf), "read failed");
		pIndex = (MyIndex*)readBuf;

		if (pIndex->m_nA != idx % 5000) {
			TEST_FAILED("pIndex->m_nA (%llu) != idx(%llu) \% 5000", pIndex->m_nA, idx);
			return false;
		}

		varbuf = (char*)realloc(varbuf, pIndex->m_nA);
		FORWARD_TEST(varDB.Read(pIndex->m_nC, pIndex->m_nB, varbuf, pIndex->m_nA) ,"var read failed");
		for (int k = 0; k < pIndex->m_nA; k++) {
			if (varbuf[k] != k % 120) {
				TEST_FAILED("varbuf[k] != k \% 120,,,%d ", k);
				return false;
			}
		}

	}

	free(varbuf);
	TEST_SUCCESS("");
	return true;
}

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

	ACE_stat fileStat;

	while (true)
	{

		if (!vartest1(rand() % 1000)) {
			return 1;
		}
	}
	return 0;

	while (1) {
		/*
		if (ACE_OS::stat("index.db", &fileStat) == 0)
		{
			if ((fileStat.st_size >> 10) > (off_t)50 * 1024 * 1024)
			{
				printf("test end, all succeed \n");
				return 0;
			}
		}
		*/
		if (!test12()) {
			return -1;
		}
		if (!test1()) {
			return -1;
		}
		if (!test2()) {
			return -1;
		}
		if (!test3()) {
			return -1;
		}
		if (!test4()) {
			return -1;
		}
		if (!test5()) {
			return -1;
		}
		//if (!test6()) {
		//	return -1;
		//}
		if (!test7()) {
			return -1;
		}
		if (!test8()) {
			return -1;
		}
		if (!test10()) {
			return -1;
		}

	}

	return 0;
}
