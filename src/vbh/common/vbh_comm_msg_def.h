#ifndef COMM_MSG_DEF_H_423809423798432798479843286468432849894865484684386456
#define COMM_MSG_DEF_H_423809423798432798479843286468432849894865484684386456

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/mem_mng/dsc_stl_type.h"

#include "common/vbh_comm_id_def.h"
#include "common/vbh_comm_def_export.h"

namespace VBH
{
	//===============================ͨ�ã������Ϣ����==================================

	//��ʾorder�����keyֵ
	class COrderGenerateKey
	{
	public:
		DSC_BIND_ATTR(m_nOrderID, m_nSequenceNumber, m_nSystemID);

	public:
		ACE_UINT16 m_nOrderID = 0; //order��id
		ACE_UINT32 m_nSequenceNumber = 0; //ʱ����ˮ��
		ACE_UINT64 m_nSystemID = 0; //ϵͳ����� �û�ID ��information-ID
	};

	//����ʱʹ�õ�key�� user-key�� information-key
	class COnCreateKey
	{
	public:
		DSC_BIND_ATTR(m_genKey, m_realKey);

	public:
		VBH::COrderGenerateKey m_genKey; //vbhϵͳΪ��key�����ϵͳ����
		DSC::CDscBlob m_realKey; //����ʱʵ��ʹ�õ�key
	};

	//��ʾϵͳ�� verion>0 ��user
	class CVbhUser
	{
	public:
		DSC_BIND_ATTR(m_nVersion, m_genUserKey, m_userInfo);

	public:
		ACE_UINT32 m_nVersion; //�û����ݵİ汾�ţ�ȡֵ�϶� > 0
		VBH::COrderGenerateKey m_genUserKey; //vbhϵͳΪuser-key�����ϵͳ����
		DSC::CDscBlob m_userInfo;
	};

	//��ʾϵͳ���´�����informaiton //ʹ�ô˽ṹʱ����ʾ��δΪ��information���� ϵͳkey
	class CVbhOnCreateInformation
	{
	public:
		DSC_BIND_ATTR(m_key, m_value);

	public:
		DSC::CDscBlob m_key;//informationԭʼ��key��
		DSC::CDscBlob m_value;
	};

	//��ʾϵͳ�е�information (���еİ汾��information�������ô˽ṹ��ʾ��
	class CVbhInformation
	{
	public:
		DSC_BIND_ATTR(m_nVersion, m_key, m_value);

	public:
		ACE_UINT32 m_nVersion; //value�İ汾�ţ�0��ʾ��ʼֵ
		DSC::CDscBlob m_key;//version=0ʱ������Ϊ��COnCreatKey��verion>0ʱ������Ϊ��COrderGenerateKey��
		DSC::CDscBlob m_value;
	};


	class COrderChannelProcessServiceAddr //order��channel �������ĵ�ַ��Ϣ
	{
	public:
		DSC_BIND_ATTR(m_strIpAddr, m_nPort, m_nChannelID);

	public:
		CDscString m_strIpAddr;
		ACE_UINT16 m_nPort = 0;
		ACE_UINT32 m_nChannelID = 0;
	};

	class CCcEndorsePolicy //CC �ı������
	{
	public:
		DSC_BIND_ATTR(m_lstPeerID);

	public:
		DSC::CDscShortList<ACE_INT16> m_lstPeerID;
	};

	template <typename TYPE>
	class CSimpleVector
	{
	public: //��ͨ���ʺ���
		//���ô洢�ռ��С
		ACE_INT32 Open(const ACE_UINT32 nSize);
		void Close(void);

	public: //��ͨ���ʺ���
		//��ȡԪ�ظ���
		ACE_UINT32 GetSize(void);
		ACE_UINT32 GetBufLen(void);

		//�����βһ��Ԫ��
		ACE_INT32 PushBack(TYPE& t);

		//����[]�����
		TYPE& operator[](const ACE_UINT32 idx);

		//��մ洢��Ԫ��
		void Clear(void);

	public: //����뺯��
		//��ȡ������С
		void GetEncodeSize(CDscGetSizeCodeState& rCodeStateControl);

		//����
		void Encode(CDscEncodeState& rCodeStateControl);

		//����
		ACE_INT32 Decode(CDscDecodeState& rCodeStateControl);

	private:
		ACE_UINT32 m_nBufLen = 0; //����������
		ACE_UINT32 m_nItemCount = 0; //Ԫ�ظ���

		TYPE* m_pItemBuf = NULL; //Ԫ�ػ�����
	};

	//===============================Transaction�����Ϣ����==================================

	//transaction��key���� //order����
	class COrderGenerateTransactionKey
	{
	public:
		DSC_BIND_ATTR(m_nOrderID, m_nSequenceNumber, m_nTransIdx, m_nBlockID);

	public://У���õ�ID
		ACE_UINT16 m_nOrderID; //�������ʱ��orderID
		ACE_UINT32 m_nSequenceNumber; //�������ʱ����ˮ��

	public://�������û�ID
		ACE_UINT32 m_nTransIdx; //�����������е���ţ� ��1��ʼ
		ACE_UINT64 m_nBlockID; //����ID
	};

	//transaction������
	class CTransactionType
	{
	public:
		enum
		{
			EN_SIZE = sizeof(ACE_UINT8),
			EN_REGIST_USER_TRANSACTION_TYPE = 1, //ע���û�����������
			EN_PROPOSE_TRANSACTION_TYPE = 2, //�����᰸����������
		};

	public:
		DSC_BIND_ATTR(m_nTransType);

	public:
		ACE_UINT8 m_nTransType = 0; //���������
	};

	//transaction-key��У�鱾���� m_nOrderID + m_nSequenceNumber ��������ɣ���Ӧ�ô����transaction��
	//Ϊ�˽�ʡ�洢�ռ䣬��m_nOrderIDͳһ��ȡ�������������ͷ�У��� m_nSerialNumser ���� transaction ��
	class CTransactionHeader
	{
	public:
		DSC_BIND_ATTR(m_nSequenceNumber, m_nCompletionTime);

	public:
		ACE_UINT32 m_nSequenceNumber; //�������ʱ����ˮ�� //transaction-key��У�飬�����������
		time_t m_nCompletionTime; //��������ʱ��
	};

	//ע���û�����������
	class CRegistUserTransaction
	{
	public:
		DSC_BIND_ATTR(m_nSequenceNumber, m_nUserID, m_userKey, m_userInfo, m_cltPubKey, m_svrPriKey, m_envelopeKey);

	public:
		ACE_UINT32 m_nSequenceNumber; //�������ʱΪע���û��������ˮ�ţ�����У�� //transaction-key��У�飬�����������
		ACE_UINT64 m_nUserID; //Ϊ�û�����Ψһ����ˮ�ţ�����ƾ��ú���У��λ�����û���Ϣ //m_nOrderID + m_nSerialNumser + m_nSystemIndex == user-key
		DSC::CDscBlob m_userKey; //�û�����ע��ʱ�ύ������key
		DSC::CDscBlob m_userInfo;
		DSC::CDscBlob m_cltPubKey;
		DSC::CDscBlob m_svrPriKey;
		DSC::CDscBlob m_envelopeKey; //ע���������Ҫ��������Կ
	};

	//�����᰸����������
	class CProposeTransaction
	{

	public:
		DSC_BIND_ATTR(m_header, m_proposal, m_transProcessResult, m_vecUser, m_vecInfo);

	public:
		CTransactionHeader m_header;
		DSC::CDscBlob m_proposal;
		DSC::CDscBlob m_transProcessResult;
		DSC::CDscShortVector<VBH::CVbhUser> m_vecUser;
		DSC::CDscShortVector<VBH::CVbhInformation> m_vecInfo;
	};

	template <typename TRANS_TYPE>
	class CTransactionEncoder
	{
	public:
		DSC_BIND_ATTR(m_type, m_trans);
	public:
		CTransactionType m_type;
		TRANS_TYPE m_trans;
	};

	//===============================��ʼ������=��Ϣ==================================

	class CQueryPtsAddrTasCmsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_PTS_ADDR_TAS_CMS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nPeerID, m_nNodeID, m_nServiceID);

	public:
		ACE_UINT16 m_nPeerID;
		ACE_UINT16 m_nNodeID;
		ACE_UINT16 m_nServiceID;
	};

	class CQueryPtsAddrCmsTasRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_PTS_ADDR_CMS_TAS_RSP
		};

	public:
		DSC_BIND_ATTR(m_lstPtsAddr);
	public:
		DSC::CDscShortList<VBH::COrderChannelProcessServiceAddr> m_lstPtsAddr;
	};

	class CQueryCbsAddrAsCmsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_CBS_ADDR_AS_CMS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nPeerID, m_nNodeID, m_nServiceID, m_nChannelID);

	public:
		ACE_UINT16 m_nPeerID;
		ACE_UINT16 m_nNodeID;
		ACE_UINT16 m_nServiceID;
		ACE_UINT32 m_nChannelID;
	};

	class CQueryCbsAddrCmsAsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_CBS_ADDR_CMS_AS_RSP
		};

	public:
		DSC_BIND_ATTR(m_cbsAddr);

	public:
		VBH::COrderChannelProcessServiceAddr m_cbsAddr;
	};

	class CRegisterAsCpsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGISTER_AS_CPS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nPeerID);

	public:
		ACE_UINT16 m_nPeerID;
	};

	//===============================ע������=��Ϣ==================================

	//api -> client service
	class CRegistUserApiCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_API_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_nCcID, m_nChannelID, m_userKey, m_userInfo);

	public:
		bool m_bWaitNotify; //�Ƿ�ȴ�����ע��֪ͨ
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID; //�û�����channel
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo;
	};

	//client�ϲ�ҵ���߼� -> client service
	class CRegistUserCltCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_CLT_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_nCcID, m_nChannelID, m_nSrcSessionID, m_userKey, m_userInfo);

	public:
		bool m_bWaitNotify; //�Ƿ�ȴ�����ע��֪ͨ
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID; //�û�����channel
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ص�sessionID
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo;
	};


	//client service -> client�ϲ�ҵ���߼�
	class CRegistUserCltCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_CLT_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_bAllowRegist, m_nReturnCode, m_nSrcSessionID, m_genUserKey, m_cryptUserKey);

	public:
		bool m_bAllowRegist; //�Ƿ�����ע��
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ͷ���sessionID
		DSC::CDscBlob m_genUserKey; //VBHϵͳΪע���û����ɵ�ϵͳKey //��key������Ϊ CChannelKeyWrapper V�������base64���� //ר�������᰸��
		DSC::CDscBlob m_cryptUserKey; //VBHϵͳΪע���û����ɵ�ϵͳKey + ��Կ������˹�Կ���ͻ���˽Կ���Գ���Կ�� //��key������Ϊ CClientCryptKeyWrapper V�������base64���룬 //ר�������û�����VBHϵͳ
	};

	//client service -> client�ϲ�ҵ���߼� //�ϲ��߼�������ʱ���趨�ȴ�֪ͨ���Ż��ں����յ�����Ϣ
	class CRegistUserCltCltNotify
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_CLT_CLT_NOTIFY
		};

	public:
		DSC_BIND_ATTR(m_bSuccess, m_nReturnCode, m_nSrcSessionID);

	public:
		bool m_bSuccess; //�Ƿ�����ע��ɹ�
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ͷ���sessionID
	};

	// client -> endorser 
	class CCryptRegistUserCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_REGIST_USER_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_userData);

	public:
		ACE_UINT32 m_nCltSessionID;
		DSC::CDscBlob m_userData; //bWaitNotify + cc-id + channel-id + m_userKey + user-info
	};

	// endorser -> client
	class CCryptRegistUserEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_REGIST_USER_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_nEsSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nEsSessionID; //endorser �˵�session id
		DSC::CDscBlob m_userData; //���ܺ���û����ݣ��Ƿ�����ע�� + �û���ʼ��Ϣ + ����� + �û���������Կ������˹�Կ���ͻ���˽Կ���������öԳ���Կ��
	};

	// client -> endorser 
	class CRegistUserCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_bSubmitNode, m_nCcID, m_nCltSessionID, m_nChannelID, m_userKey, m_userInfo);

	public:
		bool m_bWaitNotify; //�ͻ����Ƿ�ȴ�����֪ͨ
		bool m_bSubmitNode; //����Ƿ�ѡ�е�
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nChannelID; //�û�����channel
		DSC::CDscBlob m_userKey; //�û�key
		DSC::CDscBlob m_userInfo; //�û���Ϣ
	};

	// endorser -> client
	class CRegistUserEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_bAllowRegist, m_nReturnCode, m_nCltSessionID, m_nEsSessionID, m_userInitInfo);

	public:
		bool m_bAllowRegist; //�Ƿ�����ע��
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nEsSessionID; //endorser �˵�session id
		DSC::CDscBlob m_userInitInfo; //����ע��ʱ�ĳ�ʼ��Ϣ
	};

	// endorser -> cc
	class CRegistUserEsCcReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_ES_CC_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_nChannelID, m_userKey, m_userInfo);

	public:
		ACE_UINT32 m_nEsSessionID; //endorser �˵�sessionID
		ACE_UINT32 m_nChannelID; //�û�����channel
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo; //ע��ʱЯ���û���Ϣ
	};

	//cc -> endorser
	class CRegistUserCcEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_REGIST_USER_CC_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_bAllowRegist, m_nReturnCode, m_nEsSessionID, m_userInitInfo);

	public:
		bool m_bAllowRegist; //�Ƿ�����ע�ᣬ ����������
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userInitInfo; //ע���ĳ�ʼ��Ϣ������������
	};

	//client -> endorser //����ͨ���������ύ�û�ע�����������
	class CCryptSubmitRegistTransactionCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBMIT_REGIST_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_userData);

	public:
		ACE_UINT32 m_nEsSessionID; //endorser �˵�sessionID
		DSC::CDscBlob m_userData; //���ܺ�: �����
	};

	//endorser -> client
	class CCryptSubmitRegistTransactionEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBMIT_REGIST_TRANSACTION_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID;
		DSC::CDscBlob m_userData; //���ܺ�����: nonce + trans-key + COrderGenerateKey(�´����� user-key��Ӧ��ϵͳ���)
	};

	//client -> endorser //����ͨ���������ύ�û�ע�������
	class CSubmitRegistTransactionCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_REGIST_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID);

	public:
		ACE_UINT32 m_nEsSessionID; //endorser �˵�sessionID
	};

	//endorser -> client
	//���ظ��ͻ��˵���Կ���ӳٵ��ͻ��˱���ͨ������endorser������
	class CSubmitRegistTransactionEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_REGIST_TRANSACTION_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_genUserKey, m_svrPubCryptKey, m_cltPriCryptKey, m_envelopeKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID;
		VBH::COrderGenerateKey m_genUserKey;
		DSC::CDscBlob m_svrPubCryptKey;
		DSC::CDscBlob m_cltPriCryptKey;
		DSC::CDscBlob m_envelopeKey; //�ͻ���Ҫ�洢����Կ��
	};

	//endorser -> x committer
	//enddorser�ȴ���ע�������ִ�н��
	class CSubscribeTransactionResultEsXcsRegUsrReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBSCRIBE_TRANSACTION_RESULT_ES_XCS_REGUSR_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_genTransKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	//x committer -> endorser
	class CSubscribeTransactionResultXcsEsRegUsrRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBSCRIBE_TRANSACTION_RESULT_XCS_ES_REGUSR_RSP
		};

	public:
		DSC_BIND_ATTR(m_bTransResult, m_nReturnCode, m_nEsSessionID);

	public:
		bool m_bTransResult;
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
	};

	//client -> endorser 
	class CCryptCancelRegistUserCltEsReq //ȡ���û�ע�ᣬɾ��endorser��session
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_CANCEL_REGIST_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_userData);

	public:
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userData; //���ܺ���û����ݣ������
	};

	//client -> endorser 
	class CCancelRegistUserCltEsReq //ȡ���û�ע�ᣬɾ��endorser��session
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CANCEL_REGIST_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID);

	public:
		ACE_UINT32 m_nEsSessionID;
	};

	// endorser -> sdk ע��������֪ͨ
	class CCryptSubscribeRegistTransactionEsCltNotify
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBSCRIBE_REGIST_TRANSACTION_ES_CLT_NOTIFY
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID; //client session id
		DSC::CDscBlob m_userData; //���ܺ�����: nonce + trans-result(�����Ƿ�ִ�гɹ���m_nReturnCode==VBH::EN_OK_TYPEʱ������)
	};

	// endorser -> sdk ע��������֪ͨ
	class CSubscribeRegistTransactionEsCltNotify
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBSCRIBE_REGIST_TRANSACTION_ES_CLT_NOTIFY
		};

	public:
		DSC_BIND_ATTR(m_bTransResult, m_nReturnCode, m_nCltSessionID);

	public:
		bool m_bTransResult; //�����Ƿ�ִ�гɹ���m_nReturnCode==VBH::EN_OK_TYPEʱ������
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID; //client session id
	};

	// endorser service -> transform agent service
	class CSubmitRegistTransactionEsTasReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_REGIST_TRANSACTION_ES_TAS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_nChannelID, m_userKey, m_userInfo, m_cltPubKey, m_svrPriKey, m_envelopeKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		ACE_UINT32 m_nChannelID;

		DSC::CDscBlob m_userKey; //��������  -//ע��ʱ�û����͵�user-key
		DSC::CDscBlob m_userInfo; //�������� -//ccΪ�û����ɵĳ�ʼ��Ϣ
		DSC::CDscBlob m_cltPubKey; //��������  -//ע��ʱ������Կ�� �ͻ��˹�Կ
		DSC::CDscBlob m_svrPriKey; //��������  -//ע��ʱ������Կ�������˽Կ
		DSC::CDscBlob m_envelopeKey; //��������  -//ע��ʱ������Կ���Գ���Կ
	};

	//transform agent service -> endorser service  �ύע������Ӧ��
	class CSubmitRegistTransactionTasEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_REGIST_TRANSACTION_TAS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nOrderID, m_nReturnCode, m_nEsSessionID, m_nSequenceNumber, m_nTransIdx, m_nBlockID, m_nUserID);

	public:
		//��ʼ����У����ذ���VBH::CTransactionKey �� VBH::COrderGenerateKey, Ϊ��С��Ϣ�ռ䣬�������ṹ�𿪣��ϲ�������
		// m_nOrderID m_nSequenceNumber Ϊ�����ṹ�е�������
		ACE_UINT16 m_nOrderID; //�������ʱ��orderID��m_nOrderID + m_nSerialNumser ����У�� 
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		ACE_UINT32 m_nSequenceNumber; //�������ʱ����ˮ�� ��m_nOrderID + m_nSerialNumser ����У��
		ACE_UINT32 m_nTransIdx; //�����������е���ţ� ��1��ʼ
		ACE_UINT64 m_nBlockID; //����ID //m_nOrderID + m_nSerialNumser + m_nBlockID + m_nTransIdx  == trans-key
		ACE_UINT64 m_nUserID; //Ϊ�û�����Ψһ����ˮ�ţ�����ƾ��ú���У��λ�����û���Ϣ //m_nOrderID + m_nSerialNumser + m_nUserID == user-key
	};

	//transform agent service -> order-cps
	class CSubmitRegistTransactionTasCpsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_REGIST_TRANSACTION_TAS_CPS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nTasSessionID, m_userKey, m_userInfo, m_cltPubKey, m_svrPriKey, m_envelopeKey);

	public:
		ACE_UINT32 m_nTasSessionID;

		DSC::CDscBlob m_userKey; //��������  -//ע��ʱ�û����͵�user-key
		DSC::CDscBlob m_userInfo; //�������� -//ccΪ�û����ɵĳ�ʼ��Ϣ
		DSC::CDscBlob m_cltPubKey; //��������  -//ע��ʱ������Կ�� �ͻ��˹�Կ
		DSC::CDscBlob m_svrPriKey; //��������  -//ע��ʱ������Կ�������˽Կ
		DSC::CDscBlob m_envelopeKey; //��������  -//ע��ʱ������Կ���Գ���Կ
	};

	//order-cps -> transform agent service  �ύע������Ӧ��
	class CSubmitRegistTransactionCpsTasRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_REGIST_TRANSACTION_CPS_TAS_RSP
		};

	public:
		DSC_BIND_ATTR(m_nOrderID, m_nReturnCode, m_nTasSessionID, m_nSequenceNumber, m_nTransIdx, m_nBlockID, m_nUserID);

	public:
		//��ʼ����У����ذ���VBH::CTransactionKey �� VBH::COrderGenerateKey, Ϊ��С��Ϣ�ռ䣬�������ṹ�𿪣��ϲ�������
		// m_nOrderID m_nSequenceNumber Ϊ�����ṹ�е�������
		ACE_UINT16 m_nOrderID; //�������ʱ��orderID��m_nOrderID + m_nSerialNumser ����У�� 
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nTasSessionID;
		ACE_UINT32 m_nSequenceNumber; //�������ʱ����ˮ�� ��m_nOrderID + m_nSerialNumser ����У��
		ACE_UINT32 m_nTransIdx; //�����������е���ţ� ��1��ʼ
		ACE_UINT64 m_nBlockID; //����ID //m_nOrderID + m_nSerialNumser + m_nBlockID + m_nTransIdx  == trans-key
		ACE_UINT64 m_nUserID; //Ϊ�û�����Ψһ����ˮ�ţ�����ƾ��ú���У��λ�����û���Ϣ //m_nOrderID + m_nSerialNumser + m_nUserID == user-key
	};

	//�ύ���������ͬ�᰸����

	//===============================ע������=��Ϣ==================================
	class CCryptUnRegistUserReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_UNREGIST_USER_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nChannelID, m_nUserSystemIndex);

	public:
		ACE_UINT32 m_nChannelID;
		ACE_UINT64 m_nUserSystemIndex; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ID
	};

	class CCryptUnRegistUserRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_UNREGIST_USER_RSP_MSG
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nUserSystemIndex);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT64 m_nUserSystemIndex; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ID
	};

	class CUnRegistUserReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_UNREGIST_USER_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nChannelID, m_nUserSystemIndex);

	public:
		ACE_UINT32 m_nChannelID;
		ACE_UINT64 m_nUserSystemIndex; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ID
	};

	class CUnRegistUserRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_UNREGIST_USER_RSP_MSG
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nUserSystemIndex);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT64 m_nUserSystemIndex; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ID
	};

	//===============================��ѯ�û�����=��Ϣ==================================
	//api -> client service 
	class CQueryUserInfoApiCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_API_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_nChannelID, m_userKey, m_cryptUserKey);

	public:
		ACE_UINT32 m_nChannelID;
		DSC::CDscBlob m_userKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key
		DSC::CDscBlob m_cryptUserKey; //��������Կ
	};

	//client�ϲ�ҵ�� -> client service 
	class CQueryUserInfoCltCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_CLT_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_nSrcSessionID, m_cryptUserKey);

	public:
		ACE_UINT32 m_nSrcSessionID;
		DSC::CDscBlob m_cryptUserKey; //��������Կ
	};

	//client service  -> client�ϲ�ҵ��
	class CQueryUserInfoCltCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_CLT_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nSrcSessionID, m_userInfo);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID;
		DSC::CDscBlob m_userInfo;
	};

	// client -> endorser
	class CCryptQueryUserInfoCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_QUERY_USER_INFO_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_nChannelID, m_genUserKey, m_userData);

	public:
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nChannelID;

		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
		DSC::CDscBlob m_userData; //���ܺ����ݣ�nonce
	};

	//endorser -> client
	class CCryptQueryUserInfoEdsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_QUERY_USER_INFO_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSessionID;
		DSC::CDscBlob m_userData; //���ܺ����ݣ�nonce + user-info // CQueryUserInfoRspWrapper���
	};

	// client -> endorser
	class CQueryUserInfoCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_nChannelID, m_genUserKey);

	public:
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nChannelID;
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
	};

	//endorser -> client
	class CQueryUserInfoEdsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nSessionID, m_userInfo);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSessionID;
		DSC::CDscBlob m_userInfo;
	};

	//endorser -> x committer service //��ѯ�û��� ��Ϣ����Կ
	class CQueryUserFullInfoEsXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_FULL_INFO_ES_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_genUserKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
	};

	//x committer service -> endorser
	class CQueryUserFullInfoXcsEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_FULL_INFO_XCS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_userInfo, m_envelopeKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userInfo;
		DSC::CDscBlob m_envelopeKey;  //�õ��ĶԳ���Կ //ֻ���ض�Ӧ��Կ����
	};

	//endorser -> x committer service //��ѯ�û��� ��Ϣ
	class CQueryUserInfoEsXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_ES_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_genUserKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
	};

	//x committer service -> endorser
	class CQueryUserInfoXcsEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_XCS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_userInfo);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userInfo;
	};

	//===============================����Transaction����=��Ϣ==================================
	//api -> client service
	class CProposeApiCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_PROPOSE_API_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_nCcID, m_nActionID, m_nChannelID, m_userKey, m_proposal, m_cryptUserKey);

	public://TODO: �ο������ʵ�֣�ɾ�����е��ֶ�
		bool m_bWaitNotify; //�Ƿ�ȴ���������֪ͨ
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nActionID;
		ACE_UINT32 m_nChannelID;
		DSC::CDscBlob m_userKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key
		DSC::CDscBlob m_proposal;
		DSC::CDscBlob m_cryptUserKey;
	};

	//client�ϲ�ҵ���߼� -> client service
	class CProposeCltCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_PROPOSE_CLT_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_nCcID, m_nActionID, m_nSrcSessionID, m_proposal, m_cryptUserKey);

	public:
		bool m_bWaitNotify; //�Ƿ�ȴ���������֪ͨ
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nActionID;
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ص�sessionID
		DSC::CDscBlob m_proposal;
		DSC::CDscBlob m_cryptUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key
	};

	//client service -> client�ϲ�ҵ���߼�
	class CProposeCltCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_PROPOSE_CLT_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nSrcSessionID, m_genTransKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ͷ���sessionID
		DSC::CDscBlob m_genTransKey; //ϵͳΪtransaction���ɵ�key
	};

	//client service -> client�ϲ�ҵ���߼�
	class CProposeCltCltNotify
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_PROPOSE_CLT_CLT_NOTIFY
		};

	public:
		DSC_BIND_ATTR(m_bSuccess, m_nReturnCode, m_nSrcSessionID);

	public:
		bool m_bSuccess; //�����Ƿ�ִ�гɹ�
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ͷ���sessionID
	};

	//client -> endorser
	class CCryptSubmitProposalCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBMIT_PROPOSAL_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nChannelID, m_nCltSessionID, m_genUserKey, m_userData);

	public:
		ACE_UINT32 m_nChannelID;
		ACE_UINT32 m_nCltSessionID; //�ͻ���sessionID
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
		DSC::CDscBlob m_userData; //���ܺ����ݣ�nonce + proposal + cc-id + action-id
	};

	//endorser -> client
	class CCryptSubmitProposalEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBMIT_PROPOSAL_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_nCltSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID; //endorser session id
		ACE_UINT32 m_nCltSessionID; //client session id
		DSC::CDscBlob m_userData; //���ܺ����ݣ� nonce + transaction-content(transaction��contentnt�������ʽ���)
	};

	//client -> endorser
	class CSubmitProposalCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_bWaitNotify, m_bSubmitNode, m_nCcID, m_nChannelID, m_nCltSessionID, m_nAction, m_proposal);

	public:
		bool m_bWaitNotify; //�Ƿ�ȴ���������֪ͨ
		bool m_bSubmitNode; //֪ͨendorser����session�����ں���commit transaction
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID;
		ACE_UINT32 m_nCltSessionID; //�ͻ���sessionID
		ACE_UINT32 m_nAction;
		DSC::CDscBlob m_proposal;
	};

	//endorser -> client
	//��cc���ɵ��������ݾ��Ǵ���õģ�ֻ�Ǵ��ʱ���½�information�͸���information���ڲ�ͬ�Ķ��У�ά��Ŀǰ�������е�״̬��
	class CSubmitProposalEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_nCltSessionID, m_transContent);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID; //endorser session id
		ACE_UINT32 m_nCltSessionID; //client session id
		DSC::CDscBlob m_transContent; //��������
	};

	//endoser -> x committer service
	class CQueryCryptKeyProposeEsXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_CRYPT_KEY_PROPOSE_ES_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_genUserKey, m_nEsSessionID);

	public:
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
		ACE_UINT32 m_nEsSessionID;
	};

	//x committer service -> endorser
	class CQueryCryptKeyProposeXcsEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_CRYPT_KEY_PROPOSE_XCS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_envelopeKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_envelopeKey;  //�õ��ĶԳ���Կ
	};

	//endorser -> cc
	class CSubmitProposalEsCcReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_ES_CC_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_nAction, m_nChannelID, m_proposal);

	public:
		ACE_UINT32 m_nEsSessionID;
		ACE_UINT32 m_nAction;
		ACE_UINT32 m_nChannelID;
		DSC::CDscBlob m_proposal;//�ͻ����ύ���᰸
	};

	//cc -> endorser
	class CSubmitProposalCcEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_CC_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_transContent);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_transContent; //�������� ��Ӧ CRoughProposalTransactionWrapper �ṹ�� Ϊ�����÷�װ�����Ч�ʣ�����ɢ�����ݴ�������Ǹô���������������е�����
	};

	//cc -> x committer service
	class CQueryUserInfoCcXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_CC_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_genUserKey, m_nCcSessionID);

	public:
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
		ACE_UINT64 m_nCcSessionID; //���CC-SessionID����������ID������һ���ϳɵľ�������ص�CCʱ��Ҫ��������ʹ��
	};

	//x committer service -> cc
	class CQueryUserInfoXcsCcRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_USER_INFO_XCS_CC_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nUserInfoVersion, m_nCcSessionID, m_nUserSysIndex, m_userInfo);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nUserInfoVersion;
		ACE_UINT64 m_nCcSessionID; //���CC-SessionID����������ID������һ���ϳɵľ�������ص�CCʱ��Ҫ��������ʹ��
		ACE_UINT64 m_nUserSysIndex; //�û�ע��ʱ�����ϵͳID
		DSC::CDscBlob m_userInfo;
	};


	//cc -> x committer service
	class CQueryInformationCcXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_INFORMATION_CC_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_genInfoKey, m_nCcSessionID);

	public:
		VBH::COrderGenerateKey m_genInfoKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��information�����information-key
		ACE_UINT64 m_nCcSessionID; //���CC-SessionID����������ID������һ���ϳɵľ�������ص�CCʱ��Ҫ��������ʹ��
	};

	//x committer service -> cc
	class CQueryInformationXcsCcRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_INFORMATION_XCS_CC_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nVersion, m_nCcSessionID, m_nInfoSysIndex, m_value);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nVersion; //valueֵ�İ汾��
		ACE_UINT64 m_nCcSessionID; //���CC-SessionID����������ID������һ���ϳɵľ�������ص�CCʱ��Ҫ��������ʹ��
		ACE_UINT64 m_nInfoSysIndex; //���� information ʱϵͳ�����ID
		DSC::CDscBlob m_value;
	};

	// client -> endorser
	class CCryptSubmitProposalTransactionCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBMIT_PROPOSAL_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_userData);

	public:
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userData; //���ܺ����ݣ�nonce
	};

	// endorser -> sdk �ύ�᰸�����Ӧ��
	class CCryptSubmitProposalTransactionEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBMIT_PROPOSAL_TRANSACTION_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID; //client session id
		DSC::CDscBlob m_userData; //���ܺ�����: nonce + gen-trans-key
	};

	// client -> endorser
	class CSubmitProposalTransactionCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID);

	public:
		ACE_UINT32 m_nEsSessionID;
	};

	// endorser -> sdk �ύ�᰸�����Ӧ��
	class CSubmitProposalTransactionEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_TRANSACTION_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_genTransKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID; //client session id
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	//endorser -> x committer
	//enddorser�ȴ����᰸�����ִ�н��
	class CSubscribeTransactionResultEsXcsProposeReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBSCRIBE_TRANSACTION_RESULT_ES_XCS_PROPOSE_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_genTransKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	//x committer -> endorser
	class CSubscribeTransactionResultXcsEsProposeRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBSCRIBE_TRANSACTION_RESULT_XCS_ES_PROPOSE_RSP
		};

	public:
		DSC_BIND_ATTR(m_bTransResult, m_nReturnCode, m_nEsSessionID);

	public:
		bool m_bTransResult;
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
	};

	//client -> endorser 
	class CCryptCancelProposalTransactionCltEsReq //ȡ���᰸����ɾ��endorser��session
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_CANCEL_PROPOSAL_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_userData);

	public:
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userData; //���ܺ���û����ݣ������
	};

	//client -> endorser 
	class CCancelProposalTransactionCltEsReq //ȡ���᰸����ɾ��endorser��session
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CANCEL_PROPOSAL_TRANSACTION_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID);

	public:
		ACE_UINT32 m_nEsSessionID;
	};

	// endorser -> sdk �᰸������֪ͨ
	class CCryptSubscribeProposeTransactionEsCltNotify
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_SUBSCRIBE_PROPOSE_TRANSACTION_ES_CLT_NOTIFY
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nCltSessionID, m_userData);

	public:
		ACE_INT32 m_nReturnCode; //
		ACE_UINT32 m_nCltSessionID; //client session id
		DSC::CDscBlob m_userData; //���ܺ�����: nonce + transaction-result(����ִ�н���Ƿ�ɹ��� �� m_nReturnCode == VBH::EN_OK_TYPEʱ��Ч)
	};

	// endorser -> sdk �᰸������֪ͨ
	class CSubscribeProposeTransactionEsCltNotify
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBSCRIBE_PROPOSE_TRANSACTION_ES_CLT_NOTIFY
		};

	public:
		DSC_BIND_ATTR(m_bTransResult, m_nReturnCode, m_nCltSessionID);

	public:
		bool m_bTransResult; //����ִ�н���Ƿ�ɹ��� �� m_nReturnCode == VBH::EN_OK_TYPEʱ��Ч
		ACE_INT32 m_nReturnCode; //
		ACE_UINT32 m_nCltSessionID; //client session id
	};

	// endorser service -> transform agent service
	class CSubmitProposalTransactionEsTasReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_TRANSACTION_ES_TAS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_nChannelID, m_transContent);

	public:
		ACE_UINT32 m_nEsSessionID;
		ACE_UINT32 m_nChannelID;
		DSC::CDscBlob m_transContent; //��������
	};

	//transform agent service -> endorser service �ύ�᰸����Ӧ��
	class CSubmitProposalTransactionTasEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_TRANSACTION_TAS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_genTransKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	//transform agent service -> order-cps
	class CSubmitProposalTransactionTasCpsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_TRANSACTION_TAS_CPS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nTasSessionID, m_transContent);

	public:
		ACE_UINT32 m_nTasSessionID;
		DSC::CDscBlob m_transContent; //��������
	};

	//order-cps -> transform agent service  �ύ�᰸����Ӧ��
	class CSubmitProposalTransactionCpsTasRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_SUBMIT_PROPOSAL_TRANSACTION_CPS_TAS_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nTasSessionID, m_genTransKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nTasSessionID;
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	// order-cps -> anchor-as
	class CDistributeBlockCpsAsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_DISTRIBUTE_BLOCK_CPS_AS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nBlockID, m_vbhBlock);

	public:
		ACE_UINT64 m_nBlockID; //����ID
		DSC::CDscBlob m_vbhBlock; //��������
	};

	// anchor-as -> order-cps //anchor�洢�ɹ��Ż�Ӧ�𣬴洢ʧ�ܲ���Ӧ�� //order���ڳ�ʱ���ط�
	class CDistributeBlockAsCpsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_DISTRIBUTE_BLOCK_AS_CPS_RSP
		};

	public:
		DSC_BIND_ATTR(m_nPeerID, m_nBlockID);

	public:
		ACE_UINT16 m_nPeerID;
		ACE_UINT64 m_nBlockID; //����ID
	};

	//anchor -> x committer service
	class CDistributeBlockAsXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_DISTRIBUTE_BLOCK_AS_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nBlockID, m_vbhBlock);

	public:
		ACE_UINT64 m_nBlockID;
		DSC::CDscBlob m_vbhBlock; //��������
	};

	//x  committer service -> anchor
	class CDistributeBlockXcsAsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_DISTRIBUTE_BLOCK_XCS_AS_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode);

	public:
		ACE_INT32 m_nReturnCode; //����д�����һ����ϵͳ�쳣����
	};


	//===============================��ѯTransaction����(��ѯ����CC)=��Ϣ==================================
	// api -> client service
	class CQueryTransInfoApiCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_API_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_nChannelID, m_userKey, m_cryptUserKey, m_transKey);

	public:
		ACE_UINT32 m_nChannelID;
		DSC::CDscBlob m_userKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������ user-key
		DSC::CDscBlob m_cryptUserKey;
		DSC::CDscBlob m_transKey;
	};

	// client �ϲ�ҵ�� -> client service
	class CQueryTransInfoCltCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_CLT_CLT_REQ
		};

	public:
		DSC_BIND_ATTR(m_nSrcSessionID, m_cryptUserKey, m_transKey);

	public:
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ص�sessionID
		DSC::CDscBlob m_cryptUserKey;
		DSC::CDscBlob m_transKey;
	};

	// client service -> client �ϲ�ҵ��
	class CQueryTransInfoCltCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_CLT_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nSrcSessionID, m_transInfo);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // �ϲ�ҵ��service��������ʱ�����ص�sessionID
		DSC::CDscBlob m_transInfo;
	};


	// client -> endorser
	class CCryptQueryTransInfoCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_QUERY_TRANS_INFO_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_nChannelID, m_genUserKey, m_userData);

	public:
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nChannelID;
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
		DSC::CDscBlob m_userData; //���ܺ�� nonce + transKey
	};

	//endorser -> client
	class CCryptQueryTransInfoEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_CRYPT_QUERY_TRANS_INFO_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_nReturnCode, m_userData);

	public:
		ACE_UINT32 m_nCltSessionID;
		ACE_INT32 m_nReturnCode;
		DSC::CDscBlob m_userData; //���ܺ�� nonce + transInfo
	};

	// client -> endorser
	class CQueryTransInfoCltEsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_CLT_ES_REQ
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_nChannelID, m_genTransKey);

	public:
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nChannelID;
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	//endorser -> client
	class CQueryTransInfoEsCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_ES_CLT_RSP
		};

	public:
		DSC_BIND_ATTR(m_nCltSessionID, m_nReturnCode, m_transInfo);

	public:
		ACE_UINT32 m_nCltSessionID;
		ACE_INT32 m_nReturnCode;
		DSC::CDscBlob m_transInfo;
	};

	//endorser -> x committer service
	class CQueryCryptKeyGetTransEsXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_CRYPT_KEY_GET_TRANS_ES_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_genUserKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateKey m_genUserKey; //ע��ɹ�ʱ���صģ�ϵͳΪ��ע���û������user-key
	};

	//x committer service -> endorser
	class CQueryCryptKeyGetTransXcsEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_CRYPT_KEY_GET_TRANS_XCS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_envelopeKey);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_envelopeKey;  //�Գ���Կ
	};

	//endorser -> x committer service
	class CQueryTransInfoEsXcsReq
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_ES_XCS_REQ
		};

	public:
		DSC_BIND_ATTR(m_nEsSessionID, m_genTransKey);

	public:
		ACE_UINT32 m_nEsSessionID;
		VBH::COrderGenerateTransactionKey m_genTransKey;
	};

	//x committer service -> endorser
	class CQueryTransInfoXcsEsRsp
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_QUERY_TRANS_INFO_XCS_ES_RSP
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nEsSessionID, m_transInfo);

	public:
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_transInfo;
	};


	//===============================MISC=��Ϣ==================================
	class CInitBcUserSdk
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_INIT_VBH_USER_SDK_MSG
		};

	public:
		DSC_BIND_ATTR(m_pCallBack);

	public:
		void* m_pCallBack = NULL;
	};

	class CBlockchainStatisticsMsg
	{
	public:
		enum
		{
			EN_MSG_ID = VBH::EN_BLOCKCHAIN_STATISTICS_MSG
		};

	public:
		DSC_BIND_ATTR(m_nStatistics);

	public:
		ACE_INT32 m_nStatistics;
	};
}

template< typename TYPE >
void DscVCodecBindAttr(VBH::CSimpleVector<TYPE>& rSimpleVec, CDscGetSizeCodeState& rCodeStateControl);

template< typename TYPE >
void DscVCodecBindAttr(VBH::CSimpleVector<TYPE>& rSimpleVec, CDscEncodeState& rCodeStateControl);

template< typename TYPE >
ACE_INT32 DscVCodecBindAttr(VBH::CSimpleVector<TYPE>& rSimpleVec, CDscDecodeState& rCodeStateControl);


#include "common/vbh_comm_msg_def.inl"

#endif
