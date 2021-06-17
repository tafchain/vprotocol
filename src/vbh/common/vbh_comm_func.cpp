
#include "dsc/configure/dsc_configure.h"

#include "common/vbh_comm_func.h"





ACE_INT32 VBH::GetStorageDirectory(ACE_UINT16 nServerID, const char* name, CDscString& strValue)
{
	char* root = ACE_OS::getenv("WORK_ROOT");

	if (!root)
	{
		return -1;
	}

	strValue = root;
	strValue += DSC_FILE_PATH_SPLIT_CHAR;
	strValue += DEF_STORAGE_DIR_NAME;
	strValue += DSC_FILE_PATH_SPLIT_CHAR;
	strValue += CDscAppManager::Instance()->GetNodeID();
	strValue += "_";
	strValue += nServerID;
	strValue += DSC_FILE_PATH_SPLIT_CHAR;
	strValue += name;

	return 0;
}




