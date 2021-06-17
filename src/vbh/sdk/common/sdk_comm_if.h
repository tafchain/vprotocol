#ifndef SDK_COMM_IF_H_7EB16902999E11E99D4960F18A3A20D1
#define SDK_COMM_IF_H_7EB16902999E11E99D4960F18A3A20D1

#include "ace/Basic_Types.h"

#include "common/sdk_comm_export.h"

namespace VBH_SDK
{
	VBH_COMM_SDK_EXPORT ACE_INT32 InitBlockChainSdk(const ACE_INT16 nAppType, const ACE_INT16 nAppID);
	VBH_COMM_SDK_EXPORT ACE_INT32 FnitBlockChainSdk(void);
	VBH_COMM_SDK_EXPORT const char* GetBcSdkErrorString(const ACE_INT32 nErrorCode);
	VBH_COMM_SDK_EXPORT ACE_INT32 WriteCryptKeyToFile(const char* pstrPathname, const char* pCryptKey, const size_t nCryptKeyLen);
	VBH_COMM_SDK_EXPORT ACE_INT32 ReadyCryptKeyToFile(const char* pstrPathname, char*& pCryptKey, size_t& nCryptKeyLen);
}

#define DEF_DEGIN_VBH_CORE_KEY "======VBH Core Key======\r\n"
#define DEF_END_VBH_CORE_KEY "\r\n======VBH Core Key======\r\n"

#endif
