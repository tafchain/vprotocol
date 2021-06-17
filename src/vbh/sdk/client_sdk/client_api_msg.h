#ifndef CLIENT_API_MSG_H_221F987CAF6911E9A08860F18A3A20D1
#define CLIENT_API_MSG_H_221F987CAF6911E9A08860F18A3A20D1

#include "ace/Basic_Types.h"


namespace VBH_SDK
{
	// 注册用户请求和响应
	class CRegistUserReq
	{
	public:
		bool m_bWaitNotify = false; //是否等待最终注册通知, 第一次应答成功时，才有后续的通知 //默认不等待通知
		ACE_INT16 m_nCcID; 
		ACE_UINT32 m_nChannelID; //用户所属channel

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen;

		char* m_pUserInfo;
		size_t m_nUserInfoLen;
	};

	class CRegistUserRsp
	{
	public:
		ACE_INT32 m_nReturnCode;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pCryptUserKey; //注册成功时带回的用户秘钥数据，//用户单独访问系统时，使用该key
		size_t m_nCryptUserKeyLen;

		char* m_pGenUserKey;
		size_t m_nGenUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key //在提案中访问用户时使用该key //解码后是一个分段ID
	};


	//注销用户请求和响应
	class CUnRegistUserReq
	{
	public:
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID; //用户所属channel

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key
	};

	class CUnRegistUserRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key
	};

	//发起提案请求和响应
	class CProposeReq
	{
	public:
		bool m_bWaitNotify = false; //是否等待最终提案通知（事务是否成功存储）, 第一次应答成功时，才有后续的通知 //默认不等待通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nAction;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key

		char* m_pProposal;
		size_t m_nProposalLen;

		char* m_pCryptUserKey;
		size_t m_nCryptUserKeyLen;
	};

	class CProposeRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key

		char* m_pTransKey;
		size_t m_nTransKeyLen; //成功时带回来的transaction-uuid
	};

	//查询用户请求和响应
	class CGetUserInfoReq
	{
	public:
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key

		char* m_pCryptUserKey;
		size_t m_nCryptUserKeyLen;
	};

	class CGetUserInfoRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key

		char* m_pUserUserInfo;
		size_t m_nUserUserInfoLen;
	};


	//查询事务请求和响应
	class CGetTransactionInfoReq
	{
	public:
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key

		char* m_pCryptUserKey;
		size_t m_nCryptUserKeyLen;

		char* m_pTransKey;
		size_t m_nTransKeyLen; //要查询的transaction-uuid
	};

	class CGetTransactionInfoRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //本次请求的ID，可以是上层业务的sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //注册成功时带回的，系统为该注册用户分配的 user-key

		char* m_pTransKey;
		size_t m_nTransKeyLen;

		char* m_pTransInfo;
		size_t m_nTransInfoLen; //插叙到的内容
	};

}

#endif
