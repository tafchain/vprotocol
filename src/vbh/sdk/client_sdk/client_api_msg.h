#ifndef CLIENT_API_MSG_H_221F987CAF6911E9A08860F18A3A20D1
#define CLIENT_API_MSG_H_221F987CAF6911E9A08860F18A3A20D1

#include "ace/Basic_Types.h"


namespace VBH_SDK
{
	// ע���û��������Ӧ
	class CRegistUserReq
	{
	public:
		bool m_bWaitNotify = false; //�Ƿ�ȴ�����ע��֪ͨ, ��һ��Ӧ��ɹ�ʱ�����к�����֪ͨ //Ĭ�ϲ��ȴ�֪ͨ
		ACE_INT16 m_nCcID; 
		ACE_UINT32 m_nChannelID; //�û�����channel

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen;

		char* m_pUserInfo;
		size_t m_nUserInfoLen;
	};

	class CRegistUserRsp
	{
	public:
		ACE_INT32 m_nReturnCode;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pCryptUserKey; //ע��ɹ�ʱ���ص��û���Կ���ݣ�//�û���������ϵͳʱ��ʹ�ø�key
		size_t m_nCryptUserKeyLen;

		char* m_pGenUserKey;
		size_t m_nGenUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key //���᰸�з����û�ʱʹ�ø�key //�������һ���ֶ�ID
	};


	//ע���û��������Ӧ
	class CUnRegistUserReq
	{
	public:
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID; //�û�����channel

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key
	};

	class CUnRegistUserRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key
	};

	//�����᰸�������Ӧ
	class CProposeReq
	{
	public:
		bool m_bWaitNotify = false; //�Ƿ�ȴ������᰸֪ͨ�������Ƿ�ɹ��洢��, ��һ��Ӧ��ɹ�ʱ�����к�����֪ͨ //Ĭ�ϲ��ȴ�֪ͨ
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nAction;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key

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

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key

		char* m_pTransKey;
		size_t m_nTransKeyLen; //�ɹ�ʱ��������transaction-uuid
	};

	//��ѯ�û��������Ӧ
	class CGetUserInfoReq
	{
	public:
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key

		char* m_pCryptUserKey;
		size_t m_nCryptUserKeyLen;
	};

	class CGetUserInfoRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key

		char* m_pUserUserInfo;
		size_t m_nUserUserInfoLen;
	};


	//��ѯ�����������Ӧ
	class CGetTransactionInfoReq
	{
	public:
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key

		char* m_pCryptUserKey;
		size_t m_nCryptUserKeyLen;

		char* m_pTransKey;
		size_t m_nTransKeyLen; //Ҫ��ѯ��transaction-uuid
	};

	class CGetTransactionInfoRsp
	{
	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nChannelID;

		ACE_UINT32 m_nSrcRequestID; //���������ID���������ϲ�ҵ���sessionID

		char* m_pUserKey;
		size_t m_nUserKeyLen; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key

		char* m_pTransKey;
		size_t m_nTransKeyLen;

		char* m_pTransInfo;
		size_t m_nTransInfoLen; //���𵽵�����
	};

}

#endif
