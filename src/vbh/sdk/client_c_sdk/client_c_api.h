#ifndef CLIENT_C_API_H_328928987643289
#define CLIENT_C_API_H_328928987643289

#include "client_c_sdk/client_c_sdk_export.h"

#ifdef __cplusplus 
extern "C" {
#endif

	//��ʼ�����
	typedef void(*on_vbh_ready) (void);

	//�쳣
	typedef void(*on_vbh_abnormal) (void);

	//�˳�
	typedef void(*on_vbh_exit) (void);

	//ע���û�Ӧ��
	typedef void(*on_regist_vbh_user_rsp) (const int nReturnCode, const unsigned int nChannelID, 
		const char* pUserKey, const unsigned int nUserKeyLen, 
		const char* pCryptUserKey, const unsigned int nCryptUserKeyLen);

	//ȡ��ע��Ӧ��
	typedef void(*on_unregist_vbh_user_rsp)  (const int nReturnCode, const unsigned int nChannelID,
		const char* pUserKey, const unsigned int nUserKeyLen);

	//�����᰸Ӧ��
	typedef void(*on_vbh_propose_rsp)  (const int nReturnCode, const unsigned int nChannelID,
		const char* pUserKey, const unsigned int nUserKeyLen,
		const char* pTransKey, const unsigned int nTransKeyLen);

	//��ѯ�û�Ӧ��
	typedef void(*on_query_vbh_user_rsp) (const int nReturnCode, const unsigned int nChannelID, 
		const char* pUserKey, const unsigned int nUserKeyLen, 
		const char* pUserUserInfo, const unsigned int nUserUserInfoLen);

	//��ѯ����Ӧ��
	typedef void(*on_query_vbh_trans_rsp)(const int nReturnCode, 
		const char* pTransKey, const int nTransKeyLen, 
		const char* pTransInfo, const int nTransInfoLen);

	//===============

	//��ʼ��
	VBH_CLIENT_C_SDK_EXPORT int init_vbh_c_sdk(const short int nAppID, on_vbh_ready pOnReady, 
		on_vbh_abnormal pOnAbnormal, 
		on_vbh_exit pOnExit, 
		on_regist_vbh_user_rsp pOnRegistUserRsp,
		on_unregist_vbh_user_rsp pOnUnRegistUserRsp,
		on_query_vbh_user_rsp pOnQueryUserRsp, 
		on_vbh_propose_rsp pOnProposeRsp, 
		on_query_vbh_trans_rsp pOnQueryTransRsp);

	//ע���û�
	VBH_CLIENT_C_SDK_EXPORT int reg_vbh_user(const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen,
		char* pUserInfo, const unsigned int nUserInfoLen);

	//ȡ��ע��
	VBH_CLIENT_C_SDK_EXPORT int unreg_vbh_user(const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen);

	//��ѯ�û�
	VBH_CLIENT_C_SDK_EXPORT int query_vbh_user(const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen,
		char* pCryptKey, const unsigned int nCryptKeyLen);

	//�����᰸
	VBH_CLIENT_C_SDK_EXPORT int vbh_propose(const short int nCcID, const unsigned int nAction, 
		const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen,
		char* pProposal, const unsigned int nProposalLen, 
		char* pCryptUserKey, const unsigned int nCryptUserKeyLen);

	//��ѯ��������
	VBH_CLIENT_C_SDK_EXPORT int query_vbh_trans(int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen, 
		char* pCryptUserKey, int nCryptUserKeyLen, 
		char* pTransKey, int nTransKeyLen);
	//===============

#ifdef __cplusplus 
}
#endif


#endif
