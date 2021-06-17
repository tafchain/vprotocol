#ifndef CLIENT_C_API_H_328928987643289
#define CLIENT_C_API_H_328928987643289

#include "client_c_sdk/client_c_sdk_export.h"

#ifdef __cplusplus 
extern "C" {
#endif

	//初始化完成
	typedef void(*on_vbh_ready) (void);

	//异常
	typedef void(*on_vbh_abnormal) (void);

	//退出
	typedef void(*on_vbh_exit) (void);

	//注册用户应答
	typedef void(*on_regist_vbh_user_rsp) (const int nReturnCode, const unsigned int nChannelID, 
		const char* pUserKey, const unsigned int nUserKeyLen, 
		const char* pCryptUserKey, const unsigned int nCryptUserKeyLen);

	//取消注册应答
	typedef void(*on_unregist_vbh_user_rsp)  (const int nReturnCode, const unsigned int nChannelID,
		const char* pUserKey, const unsigned int nUserKeyLen);

	//发起提案应答
	typedef void(*on_vbh_propose_rsp)  (const int nReturnCode, const unsigned int nChannelID,
		const char* pUserKey, const unsigned int nUserKeyLen,
		const char* pTransKey, const unsigned int nTransKeyLen);

	//查询用户应答
	typedef void(*on_query_vbh_user_rsp) (const int nReturnCode, const unsigned int nChannelID, 
		const char* pUserKey, const unsigned int nUserKeyLen, 
		const char* pUserUserInfo, const unsigned int nUserUserInfoLen);

	//查询事务应答
	typedef void(*on_query_vbh_trans_rsp)(const int nReturnCode, 
		const char* pTransKey, const int nTransKeyLen, 
		const char* pTransInfo, const int nTransInfoLen);

	//===============

	//初始化
	VBH_CLIENT_C_SDK_EXPORT int init_vbh_c_sdk(const short int nAppID, on_vbh_ready pOnReady, 
		on_vbh_abnormal pOnAbnormal, 
		on_vbh_exit pOnExit, 
		on_regist_vbh_user_rsp pOnRegistUserRsp,
		on_unregist_vbh_user_rsp pOnUnRegistUserRsp,
		on_query_vbh_user_rsp pOnQueryUserRsp, 
		on_vbh_propose_rsp pOnProposeRsp, 
		on_query_vbh_trans_rsp pOnQueryTransRsp);

	//注册用户
	VBH_CLIENT_C_SDK_EXPORT int reg_vbh_user(const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen,
		char* pUserInfo, const unsigned int nUserInfoLen);

	//取消注册
	VBH_CLIENT_C_SDK_EXPORT int unreg_vbh_user(const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen);

	//查询用户
	VBH_CLIENT_C_SDK_EXPORT int query_vbh_user(const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen,
		char* pCryptKey, const unsigned int nCryptKeyLen);

	//发起提案
	VBH_CLIENT_C_SDK_EXPORT int vbh_propose(const short int nCcID, const unsigned int nAction, 
		const unsigned int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen,
		char* pProposal, const unsigned int nProposalLen, 
		char* pCryptUserKey, const unsigned int nCryptUserKeyLen);

	//查询事务内容
	VBH_CLIENT_C_SDK_EXPORT int query_vbh_trans(int nChannelID, 
		char* pUserKey, const unsigned int nUserKeyLen, 
		char* pCryptUserKey, int nCryptUserKeyLen, 
		char* pTransKey, int nTransKeyLen);
	//===============

#ifdef __cplusplus 
}
#endif


#endif
