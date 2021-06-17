#ifndef COMM_MSG_DEF_H_423809423798432798479843286468432849894865484684386456
#define COMM_MSG_DEF_H_423809423798432798479843286468432849894865484684386456

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/mem_mng/dsc_stl_type.h"

#include "common/vbh_comm_id_def.h"
#include "common/vbh_comm_def_export.h"

namespace VBH
{
	//===============================通用（杂项）消息定义==================================

	//表示order分配的key值
	class COrderGenerateKey
	{
	public:
		DSC_BIND_ATTR(m_nOrderID, m_nSequenceNumber, m_nSystemID);

	public:
		ACE_UINT16 m_nOrderID = 0; //order的id
		ACE_UINT32 m_nSequenceNumber = 0; //时间流水号
		ACE_UINT64 m_nSystemID = 0; //系统分配的 用户ID 或information-ID
	};

	//创建时使用的key， user-key， information-key
	class COnCreateKey
	{
	public:
		DSC_BIND_ATTR(m_genKey, m_realKey);

	public:
		VBH::COrderGenerateKey m_genKey; //vbh系统为该key分配的系统索引
		DSC::CDscBlob m_realKey; //创建时实际使用的key
	};

	//表示系统中 verion>0 的user
	class CVbhUser
	{
	public:
		DSC_BIND_ATTR(m_nVersion, m_genUserKey, m_userInfo);

	public:
		ACE_UINT32 m_nVersion; //用户数据的版本号；取值肯定 > 0
		VBH::COrderGenerateKey m_genUserKey; //vbh系统为user-key分配的系统索引
		DSC::CDscBlob m_userInfo;
	};

	//表示系统中新创建的informaiton //使用此结构时，表示尚未为此information分配 系统key
	class CVbhOnCreateInformation
	{
	public:
		DSC_BIND_ATTR(m_key, m_value);

	public:
		DSC::CDscBlob m_key;//information原始的key，
		DSC::CDscBlob m_value;
	};

	//表示系统中的information (所有的版本的information都可以用此结构表示）
	class CVbhInformation
	{
	public:
		DSC_BIND_ATTR(m_nVersion, m_key, m_value);

	public:
		ACE_UINT32 m_nVersion; //value的版本号；0表示初始值
		DSC::CDscBlob m_key;//version=0时，解释为：COnCreatKey；verion>0时，解释为：COrderGenerateKey；
		DSC::CDscBlob m_value;
	};


	class COrderChannelProcessServiceAddr //order端channel 服务器的地址信息
	{
	public:
		DSC_BIND_ATTR(m_strIpAddr, m_nPort, m_nChannelID);

	public:
		CDscString m_strIpAddr;
		ACE_UINT16 m_nPort = 0;
		ACE_UINT32 m_nChannelID = 0;
	};

	class CCcEndorsePolicy //CC 的背书策略
	{
	public:
		DSC_BIND_ATTR(m_lstPeerID);

	public:
		DSC::CDscShortList<ACE_INT16> m_lstPeerID;
	};

	template <typename TYPE>
	class CSimpleVector
	{
	public: //普通访问函数
		//重置存储空间大小
		ACE_INT32 Open(const ACE_UINT32 nSize);
		void Close(void);

	public: //普通访问函数
		//获取元素个数
		ACE_UINT32 GetSize(void);
		ACE_UINT32 GetBufLen(void);

		//放入队尾一个元素
		ACE_INT32 PushBack(TYPE& t);

		//重载[]运算符
		TYPE& operator[](const ACE_UINT32 idx);

		//清空存储的元素
		void Clear(void);

	public: //编解码函数
		//获取编码后大小
		void GetEncodeSize(CDscGetSizeCodeState& rCodeStateControl);

		//编码
		void Encode(CDscEncodeState& rCodeStateControl);

		//解码
		ACE_INT32 Decode(CDscDecodeState& rCodeStateControl);

	private:
		ACE_UINT32 m_nBufLen = 0; //缓冲区长度
		ACE_UINT32 m_nItemCount = 0; //元素个数

		TYPE* m_pItemBuf = NULL; //元素缓冲区
	};

	//===============================Transaction相关消息定义==================================

	//transaction的key定义 //order生成
	class COrderGenerateTransactionKey
	{
	public:
		DSC_BIND_ATTR(m_nOrderID, m_nSequenceNumber, m_nTransIdx, m_nBlockID);

	public://校验用的ID
		ACE_UINT16 m_nOrderID; //打包事务时的orderID
		ACE_UINT32 m_nSequenceNumber; //打包事务时的流水号

	public://真正的用户ID
		ACE_UINT32 m_nTransIdx; //事务在区块中的序号， 从1开始
		ACE_UINT64 m_nBlockID; //区块ID
	};

	//transaction的类型
	class CTransactionType
	{
	public:
		enum
		{
			EN_SIZE = sizeof(ACE_UINT8),
			EN_REGIST_USER_TRANSACTION_TYPE = 1, //注册用户的事务类型
			EN_PROPOSE_TRANSACTION_TYPE = 2, //发起提案的事务类型
		};

	public:
		DSC_BIND_ATTR(m_nTransType);

	public:
		ACE_UINT8 m_nTransType = 0; //事务的类型
	};

	//transaction-key的校验本来由 m_nOrderID + m_nSequenceNumber 两部分组成，都应该存放在transaction中
	//为了节省存储空间，将m_nOrderID统一提取出来存放在区块头中，将 m_nSerialNumser 留在 transaction 中
	class CTransactionHeader
	{
	public:
		DSC_BIND_ATTR(m_nSequenceNumber, m_nCompletionTime);

	public:
		ACE_UINT32 m_nSequenceNumber; //打包事务时的流水号 //transaction-key的校验，存放在区块中
		time_t m_nCompletionTime; //事务的完成时间
	};

	//注册用户的事务类型
	class CRegistUserTransaction
	{
	public:
		DSC_BIND_ATTR(m_nSequenceNumber, m_nUserID, m_userKey, m_userInfo, m_cltPubKey, m_svrPriKey, m_envelopeKey);

	public:
		ACE_UINT32 m_nSequenceNumber; //打包事务时为注册用户分配的流水号，用于校验 //transaction-key的校验，存放在区块中
		ACE_UINT64 m_nUserID; //为用户分配唯一的流水号，后续凭借该号与校验位访问用户信息 //m_nOrderID + m_nSerialNumser + m_nSystemIndex == user-key
		DSC::CDscBlob m_userKey; //用户申请注册时提交上来的key
		DSC::CDscBlob m_userInfo;
		DSC::CDscBlob m_cltPubKey;
		DSC::CDscBlob m_svrPriKey;
		DSC::CDscBlob m_envelopeKey; //注册后服务端需要保留的秘钥
	};

	//发起提案的事务类型
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

	//===============================初始化流程=消息==================================

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

	//===============================注册流程=消息==================================

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
		bool m_bWaitNotify; //是否等待最终注册通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID; //用户所属channel
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo;
	};

	//client上层业务逻辑 -> client service
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
		bool m_bWaitNotify; //是否等待最终注册通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID; //用户所属channel
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，本地的sessionID
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo;
	};


	//client service -> client上层业务逻辑
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
		bool m_bAllowRegist; //是否允许注册
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，发送方的sessionID
		DSC::CDscBlob m_genUserKey; //VBH系统为注册用户生成的系统Key //此key的内容为 CChannelKeyWrapper V编码后，再base64编码 //专门用于提案中
		DSC::CDscBlob m_cryptUserKey; //VBH系统为注册用户生成的系统Key + 秘钥（服务端公钥，客户端私钥，对称秘钥） //此key的内容为 CClientCryptKeyWrapper V编码后，再base64编码， //专门用于用户访问VBH系统
	};

	//client service -> client上层业务逻辑 //上层逻辑在请求时，设定等待通知，才会在后续收到该消息
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
		bool m_bSuccess; //是否最终注册成功
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，发送方的sessionID
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
		ACE_UINT32 m_nEsSessionID; //endorser 端的session id
		DSC::CDscBlob m_userData; //加密后的用户数据：是否允许注册 + 用户初始信息 + 随机数 + 用户三部分秘钥（服务端公钥，客户端私钥，计算所得对称秘钥）
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
		bool m_bWaitNotify; //客户端是否等待最终通知
		bool m_bSubmitNode; //标记是否被选中的
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nChannelID; //用户所属channel
		DSC::CDscBlob m_userKey; //用户key
		DSC::CDscBlob m_userInfo; //用户信息
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
		bool m_bAllowRegist; //是否允许注册
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nCltSessionID;
		ACE_UINT32 m_nEsSessionID; //endorser 端的session id
		DSC::CDscBlob m_userInitInfo; //允许注册时的初始信息
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
		ACE_UINT32 m_nEsSessionID; //endorser 端的sessionID
		ACE_UINT32 m_nChannelID; //用户所属channel
		DSC::CDscBlob m_userKey;
		DSC::CDscBlob m_userInfo; //注册时携带用户信息
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
		bool m_bAllowRegist; //是否允许注册， 待背书内容
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		DSC::CDscBlob m_userInitInfo; //注册后的初始信息，待背书内容
	};

	//client -> endorser //背书通过后最终提交用户注册事务的请求
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
		ACE_UINT32 m_nEsSessionID; //endorser 端的sessionID
		DSC::CDscBlob m_userData; //加密后: 随机数
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
		DSC::CDscBlob m_userData; //加密后数据: nonce + trans-key + COrderGenerateKey(新创建的 user-key对应的系统编号)
	};

	//client -> endorser //背书通过后最终提交用户注册的请求
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
		ACE_UINT32 m_nEsSessionID; //endorser 端的sessionID
	};

	//endorser -> client
	//返回给客户端的秘钥，延迟到客户端背书通过后在endorser端生成
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
		DSC::CDscBlob m_envelopeKey; //客户端要存储的秘钥组
	};

	//endorser -> x committer
	//enddorser等待该注册事务的执行结果
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
	class CCryptCancelRegistUserCltEsReq //取消用户注册，删除endorser端session
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
		DSC::CDscBlob m_userData; //加密后的用户数据：随机数
	};

	//client -> endorser 
	class CCancelRegistUserCltEsReq //取消用户注册，删除endorser端session
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

	// endorser -> sdk 注册事务订阅通知
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
		DSC::CDscBlob m_userData; //加密后数据: nonce + trans-result(事务是否执行成功，m_nReturnCode==VBH::EN_OK_TYPE时有意义)
	};

	// endorser -> sdk 注册事务订阅通知
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
		bool m_bTransResult; //事务是否执行成功，m_nReturnCode==VBH::EN_OK_TYPE时有意义
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

		DSC::CDscBlob m_userKey; //事务内容  -//注册时用户发送的user-key
		DSC::CDscBlob m_userInfo; //事务内容 -//cc为用户生成的初始信息
		DSC::CDscBlob m_cltPubKey; //事务内容  -//注册时生成秘钥， 客户端公钥
		DSC::CDscBlob m_svrPriKey; //事务内容  -//注册时生成秘钥，服务端私钥
		DSC::CDscBlob m_envelopeKey; //事务内容  -//注册时生成秘钥，对称秘钥
	};

	//transform agent service -> endorser service  提交注册事务应答
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
		//初始设计中，返回包括VBH::CTransactionKey 和 VBH::COrderGenerateKey, 为减小消息空间，将两个结构拆开，合并冗余项
		// m_nOrderID m_nSequenceNumber 为两个结构中的冗余项
		ACE_UINT16 m_nOrderID; //打包事务时的orderID，m_nOrderID + m_nSerialNumser 用于校验 
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nEsSessionID;
		ACE_UINT32 m_nSequenceNumber; //打包事务时的流水号 ，m_nOrderID + m_nSerialNumser 用于校验
		ACE_UINT32 m_nTransIdx; //事务在区块中的序号， 从1开始
		ACE_UINT64 m_nBlockID; //区块ID //m_nOrderID + m_nSerialNumser + m_nBlockID + m_nTransIdx  == trans-key
		ACE_UINT64 m_nUserID; //为用户分配唯一的流水号，后续凭借该号与校验位访问用户信息 //m_nOrderID + m_nSerialNumser + m_nUserID == user-key
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

		DSC::CDscBlob m_userKey; //事务内容  -//注册时用户发送的user-key
		DSC::CDscBlob m_userInfo; //事务内容 -//cc为用户生成的初始信息
		DSC::CDscBlob m_cltPubKey; //事务内容  -//注册时生成秘钥， 客户端公钥
		DSC::CDscBlob m_svrPriKey; //事务内容  -//注册时生成秘钥，服务端私钥
		DSC::CDscBlob m_envelopeKey; //事务内容  -//注册时生成秘钥，对称秘钥
	};

	//order-cps -> transform agent service  提交注册事务应答
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
		//初始设计中，返回包括VBH::CTransactionKey 和 VBH::COrderGenerateKey, 为减小消息空间，将两个结构拆开，合并冗余项
		// m_nOrderID m_nSequenceNumber 为两个结构中的冗余项
		ACE_UINT16 m_nOrderID; //打包事务时的orderID，m_nOrderID + m_nSerialNumser 用于校验 
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nTasSessionID;
		ACE_UINT32 m_nSequenceNumber; //打包事务时的流水号 ，m_nOrderID + m_nSerialNumser 用于校验
		ACE_UINT32 m_nTransIdx; //事务在区块中的序号， 从1开始
		ACE_UINT64 m_nBlockID; //区块ID //m_nOrderID + m_nSerialNumser + m_nBlockID + m_nTransIdx  == trans-key
		ACE_UINT64 m_nUserID; //为用户分配唯一的流水号，后续凭借该号与校验位访问用户信息 //m_nOrderID + m_nSerialNumser + m_nUserID == user-key
	};

	//提交打包后事务同提案流程

	//===============================注销流程=消息==================================
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
		ACE_UINT64 m_nUserSystemIndex; //注册成功时带回的，系统为该注册用户分配的ID
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
		ACE_UINT64 m_nUserSystemIndex; //注册成功时带回的，系统为该注册用户分配的ID
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
		ACE_UINT64 m_nUserSystemIndex; //注册成功时带回的，系统为该注册用户分配的ID
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
		ACE_UINT64 m_nUserSystemIndex; //注册成功时带回的，系统为该注册用户分配的ID
	};

	//===============================查询用户流程=消息==================================
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
		DSC::CDscBlob m_userKey; //注册成功时带回的，系统为该注册用户分配的 user-key
		DSC::CDscBlob m_cryptUserKey; //打包后的秘钥
	};

	//client上层业务 -> client service 
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
		DSC::CDscBlob m_cryptUserKey; //打包后的秘钥
	};

	//client service  -> client上层业务
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

		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
		DSC::CDscBlob m_userData; //加密后数据：nonce
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
		DSC::CDscBlob m_userData; //加密后数据：nonce + user-info // CQueryUserInfoRspWrapper解包
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
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

	//endorser -> x committer service //查询用户的 信息和秘钥
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
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
		DSC::CDscBlob m_envelopeKey;  //得到的对称秘钥 //只返回对应秘钥即可
	};

	//endorser -> x committer service //查询用户的 信息
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
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

	//===============================发起Transaction流程=消息==================================
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

	public://TODO: 参考下面的实现，删除该有的字段
		bool m_bWaitNotify; //是否等待最终事务通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nActionID;
		ACE_UINT32 m_nChannelID;
		DSC::CDscBlob m_userKey; //注册成功时带回的，系统为该注册用户分配的 user-key
		DSC::CDscBlob m_proposal;
		DSC::CDscBlob m_cryptUserKey;
	};

	//client上层业务逻辑 -> client service
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
		bool m_bWaitNotify; //是否等待最终事务通知
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nActionID;
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，本地的sessionID
		DSC::CDscBlob m_proposal;
		DSC::CDscBlob m_cryptUserKey; //注册成功时带回的，系统为该注册用户分配的 user-key
	};

	//client service -> client上层业务逻辑
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
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，发送方的sessionID
		DSC::CDscBlob m_genTransKey; //系统为transaction生成的key
	};

	//client service -> client上层业务逻辑
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
		bool m_bSuccess; //事务是否执行成功
		ACE_INT32 m_nReturnCode;
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，发送方的sessionID
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
		ACE_UINT32 m_nCltSessionID; //客户端sessionID
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
		DSC::CDscBlob m_userData; //加密后数据：nonce + proposal + cc-id + action-id
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
		DSC::CDscBlob m_userData; //加密后数据： nonce + transaction-content(transaction的contentnt按拆分形式打包)
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
		bool m_bWaitNotify; //是否等待最终事务通知
		bool m_bSubmitNode; //通知endorser保留session，用于后续commit transaction
		ACE_INT16 m_nCcID;
		ACE_UINT32 m_nChannelID;
		ACE_UINT32 m_nCltSessionID; //客户端sessionID
		ACE_UINT32 m_nAction;
		DSC::CDscBlob m_proposal;
	};

	//endorser -> client
	//从cc生成的事务内容就是打包好的，只是打包时，新建information和更新information处于不同的队列（维持目前三个队列的状态）
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
		DSC::CDscBlob m_transContent; //事务内容
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
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
		DSC::CDscBlob m_envelopeKey;  //得到的对称秘钥
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
		DSC::CDscBlob m_proposal;//客户端提交的提案
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
		DSC::CDscBlob m_transContent; //事务内容 对应 CRoughProposalTransactionWrapper 结构， 为了做好封装和提高效率，将离散的数据打包，但是该打包并非最终区块中的事务
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
		ACE_UINT64 m_nCcSessionID; //这个CC-SessionID并非真正的ID，而是一个合成的句柄，返回到CC时，要处理后才能使用
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
		ACE_UINT64 m_nCcSessionID; //这个CC-SessionID并非真正的ID，而是一个合成的句柄，返回到CC时，要处理后才能使用
		ACE_UINT64 m_nUserSysIndex; //用户注册时分配的系统ID
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
		VBH::COrderGenerateKey m_genInfoKey; //注册成功时带回的，系统为该information分配的information-key
		ACE_UINT64 m_nCcSessionID; //这个CC-SessionID并非真正的ID，而是一个合成的句柄，返回到CC时，要处理后才能使用
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
		ACE_UINT32 m_nVersion; //value值的版本号
		ACE_UINT64 m_nCcSessionID; //这个CC-SessionID并非真正的ID，而是一个合成的句柄，返回到CC时，要处理后才能使用
		ACE_UINT64 m_nInfoSysIndex; //创建 information 时系统分配的ID
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
		DSC::CDscBlob m_userData; //加密后数据：nonce
	};

	// endorser -> sdk 提交提案事务的应答
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
		DSC::CDscBlob m_userData; //加密后数据: nonce + gen-trans-key
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

	// endorser -> sdk 提交提案事务的应答
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
	//enddorser等待该提案事务的执行结果
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
	class CCryptCancelProposalTransactionCltEsReq //取消提案事务，删除endorser端session
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
		DSC::CDscBlob m_userData; //加密后的用户数据：随机数
	};

	//client -> endorser 
	class CCancelProposalTransactionCltEsReq //取消提案事务，删除endorser端session
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

	// endorser -> sdk 提案事务订阅通知
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
		DSC::CDscBlob m_userData; //加密后数据: nonce + transaction-result(事务执行结果是否成功， 在 m_nReturnCode == VBH::EN_OK_TYPE时有效)
	};

	// endorser -> sdk 提案事务订阅通知
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
		bool m_bTransResult; //事务执行结果是否成功， 在 m_nReturnCode == VBH::EN_OK_TYPE时有效
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
		DSC::CDscBlob m_transContent; //事务内容
	};

	//transform agent service -> endorser service 提交提案事务应答
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
		DSC::CDscBlob m_transContent; //事务内容
	};

	//order-cps -> transform agent service  提交提案事务应答
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
		ACE_UINT64 m_nBlockID; //区块ID
		DSC::CDscBlob m_vbhBlock; //区块数据
	};

	// anchor-as -> order-cps //anchor存储成功才回应答，存储失败不回应答 //order会在超时后重发
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
		ACE_UINT64 m_nBlockID; //区块ID
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
		DSC::CDscBlob m_vbhBlock; //区块数据
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
		ACE_INT32 m_nReturnCode; //如果有错误，则一定是系统异常错误
	};


	//===============================查询Transaction流程(查询不经CC)=消息==================================
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
		DSC::CDscBlob m_userKey; //注册成功时带回的，系统为该注册用户分配的 user-key
		DSC::CDscBlob m_cryptUserKey;
		DSC::CDscBlob m_transKey;
	};

	// client 上层业务 -> client service
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
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，本地的sessionID
		DSC::CDscBlob m_cryptUserKey;
		DSC::CDscBlob m_transKey;
	};

	// client service -> client 上层业务
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
		ACE_UINT32 m_nSrcSessionID; // 上层业务service发起请求时，本地的sessionID
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
		DSC::CDscBlob m_userData; //加密后的 nonce + transKey
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
		DSC::CDscBlob m_userData; //加密后的 nonce + transInfo
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
		VBH::COrderGenerateKey m_genUserKey; //注册成功时带回的，系统为该注册用户分配的user-key
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
		DSC::CDscBlob m_envelopeKey;  //对称秘钥
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


	//===============================MISC=消息==================================
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
