#ifndef VBH_COMM_FUNC_H_432843298432658431469719764325685
#define VBH_COMM_FUNC_H_432843298432658431469719764325685

#include "ace/Shared_Memory_MM.h"

#include "dsc/container/dsc_string.h"
#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/configure/dsc_configure.h"
#include "dsc/dsc_app_mng.h"

#include "common/vbh_comm_def_export.h"
#include "common/vbh_comm_macro_def.h"

namespace VBH
{
	//blob, string的赋值和比对
	bool IsEqual(const CDscString& str, DSC::CDscBlob& blob); //string和blob是否相等
	void Assign(CDscString& str, DSC::CDscBlob& blob); //将blob内容赋给string
	void Assign(DSC::CDscBlob& blob, CDscString& str); //将stirng内容赋给blb

	//读取通用配置
	ACE_INT32 GetVbhProfileInt(const char* pszKeyName, ACE_INT32& nValue); //读取整数类型的配置值
	ACE_INT32 GetVbhProfileString(const char* pszKeyName, CDscString& strValue); //读取字符串类型的配置值

	ACE_INT32 GetStorageDirectory(ACE_UINT16 nServerID,const char* name, CDscString& strValue);

	//计算数据的hash值
	ACE_UINT16 GetDataHashValue(const DSC::CDscBlob& data, const ACE_UINT32 nHashBits);

	//数据版本号操作， user-info的版本号 和 information-value的版本号 //0值表示新建 //翻转时跳过0值
	void IncreaseVersion(ACE_UINT32& nVersion); //版本号自增
	bool IsVersionMatch(const ACE_UINT32 nPreVersion, const ACE_UINT32 nCurVersion); //前1个版本号和现在的版本号是否匹配
	bool IsCreatedVersion(const ACE_UINT32 nVersion); //是否是新创建的verison

	//内存映射配置文件的创建和加载，通用函数 (配置，日志，等均可使用)
	template <typename CFG_TYPE>
	ACE_INT32 CreateMmapCfgFile(const CDscString& strFilePath, CFG_TYPE& initCfg); //initCfg是初始写入文件的配置
	template<typename CFG_TYPE>
	ACE_INT32 LoadMmapCfgFile(const CDscString& strFilePath, ACE_Shared_Memory_MM& shmHandle, CFG_TYPE*& pCfg);

}

#include "common/vbh_comm_func.inl"
#endif
