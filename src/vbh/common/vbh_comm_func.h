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
	//blob, string�ĸ�ֵ�ͱȶ�
	bool IsEqual(const CDscString& str, DSC::CDscBlob& blob); //string��blob�Ƿ����
	void Assign(CDscString& str, DSC::CDscBlob& blob); //��blob���ݸ���string
	void Assign(DSC::CDscBlob& blob, CDscString& str); //��stirng���ݸ���blb

	//��ȡͨ������
	ACE_INT32 GetVbhProfileInt(const char* pszKeyName, ACE_INT32& nValue); //��ȡ�������͵�����ֵ
	ACE_INT32 GetVbhProfileString(const char* pszKeyName, CDscString& strValue); //��ȡ�ַ������͵�����ֵ

	ACE_INT32 GetStorageDirectory(ACE_UINT16 nServerID,const char* name, CDscString& strValue);

	//�������ݵ�hashֵ
	ACE_UINT16 GetDataHashValue(const DSC::CDscBlob& data, const ACE_UINT32 nHashBits);

	//���ݰ汾�Ų����� user-info�İ汾�� �� information-value�İ汾�� //0ֵ��ʾ�½� //��תʱ����0ֵ
	void IncreaseVersion(ACE_UINT32& nVersion); //�汾������
	bool IsVersionMatch(const ACE_UINT32 nPreVersion, const ACE_UINT32 nCurVersion); //ǰ1���汾�ź����ڵİ汾���Ƿ�ƥ��
	bool IsCreatedVersion(const ACE_UINT32 nVersion); //�Ƿ����´�����verison

	//�ڴ�ӳ�������ļ��Ĵ����ͼ��أ�ͨ�ú��� (���ã���־���Ⱦ���ʹ��)
	template <typename CFG_TYPE>
	ACE_INT32 CreateMmapCfgFile(const CDscString& strFilePath, CFG_TYPE& initCfg); //initCfg�ǳ�ʼд���ļ�������
	template<typename CFG_TYPE>
	ACE_INT32 LoadMmapCfgFile(const CDscString& strFilePath, ACE_Shared_Memory_MM& shmHandle, CFG_TYPE*& pCfg);

}

#include "common/vbh_comm_func.inl"
#endif
