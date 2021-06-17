#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"

#include "dsc/dsc_app_mng.h"
#include "dsc/storage/b_tree/b_tree_format.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
	printf("%llu\n", std::numeric_limits<unsigned long long>::max());

	CDscAppManager* pDscServerAppManager = ::new(std::nothrow) CDscAppManager;

	pDscServerAppManager->SetNodeType(4520);
	if (pDscServerAppManager->Init(argc, argv))
	{
		ACE_OS::printf("dsc app init failed, now exit!\n");
		pDscServerAppManager->Exit();
		delete pDscServerAppManager;

		return -1;
	}

	ACE_OS::printf("dsc app init succeed, type:%d, id:%d, running...\n", pDscServerAppManager->GetNodeType(), pDscServerAppManager->GetNodeID());

	//format TCDB
	CDscString strStoragePathName;

	strStoragePathName = pDscServerAppManager->GetWorkRoot();
	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
	strStoragePathName += "storage";
	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;

	strStoragePathName += "btree_t";
	strStoragePathName += '_';
	strStoragePathName += 1;
	strStoragePathName += '_';
	strStoragePathName += 1;

	//�Ƽ������С�̶�256 * 1024��ҳ��С % ���С��Ϊ0���ܿ��� /(ҳ��С / ���С��Ϊ0); sata���io��С256 * 1024�ı���������.������⿪��С��page cache�����ڵ���ҳ�Ļ��뻻��
	if (::BTreeFormat(strStoragePathName, 256, 16, 40960, 1024))
	{
		ACE_OS::printf("call BTreeFormat() failed, strStoragePathName:%s!", strStoragePathName.c_str());
	}

	//format UCDB
	strStoragePathName;

	strStoragePathName = pDscServerAppManager->GetWorkRoot();
	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;
	strStoragePathName += "storage";
	strStoragePathName += DSC_FILE_PATH_SPLIT_CHAR;

	strStoragePathName += "btree_u";
	strStoragePathName += '_';
	strStoragePathName += 1;
	strStoragePathName += '_';
	strStoragePathName += 1;

	//�Ƽ������С�̶�256 * 1024��ҳ��С % ���С��Ϊ0���ܿ��� /(ҳ��С / ���С��Ϊ0); sata���io��С256 * 1024�ı���������.������⿪��С��page cache�����ڵ���ҳ�Ļ��뻻��
	if (::BTreeFormat(strStoragePathName, 256, 16, 4096, 1024))
	//if (::BTreeFormat(strStoragePathName, 128, 300, 40960))
	{
		ACE_OS::printf("call BTreeFormat() failed, strStoragePathName:%s!", strStoragePathName.c_str());
	}

	delete pDscServerAppManager;

	ACE_OS::printf("dsc app terminated!\n");

	return 0;
}
