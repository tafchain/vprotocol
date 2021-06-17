#ifndef _IDC_COMMON_MSG_H_2417085051150945835
#define _IDC_COMMON_MSG_H_2417085051150945835

#include "ace/Basic_Types.h"
#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "idc_common/idc_common_id_def.h"

namespace IDC
{
	class CIdcCommonReqDataWrapper
	{
	public:
		CIdcCommonReqDataWrapper(DSC::CDscBlob& data, DSC::CDscBlob& nonce) :m_data(data), m_nonce(nonce){}

	public:
		DSC_BIND_ATTR(m_data, m_nonce);

	public:
		DSC::CDscBlob& m_data; 
		DSC::CDscBlob& m_nonce; //通信用随机数
	};
	class CIdcCommonApiCltReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_COMMON_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nType, m_nReqID, m_info);

	public:
		ACE_UINT32 m_nType;
		ACE_UINT32 m_nReqID;
		DSC::CDscBlob m_info;
	};

	class CIdcCommonCltSrvReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_COMMON_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nType, m_nReqID, m_nSessionID, m_info);

	public:
		ACE_UINT32 m_nType;
		ACE_UINT32 m_nReqID;
		ACE_UINT32 m_nSessionID;
		DSC::CDscBlob m_info;
	};

	class CIdcCommonSrvCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_COMMON_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nReturnCode, m_nType, m_nReqID, m_info);

	public:
		ACE_UINT32 m_nReturnCode;
		ACE_UINT32 m_nType;
		ACE_UINT32 m_nReqID;
		DSC::CDscBlob m_info;
	};

	class CInitClientSdk
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_INIT_CLIENT_SDK_MSG
		};

	public:
		DSC_BIND_ATTR(m_pCallBack);

	public:
		void* m_pCallBack = NULL;
	};



	class CIdcRegisterUserCltSrvReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_REGISTER_USER_MSG
		};
		DSC_BIND_ATTR(m_nId, m_info);
	public:
		ACE_UINT64 m_nId;
		DSC::CDscBlob m_info;
	};
	class CIdcRegisterUserSrvCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_REGISTER_USER_MSG
		};
		DSC_BIND_ATTR(m_nId, m_cryptoKey);
	public:
		ACE_UINT64 m_nId;
		DSC::CDscBlob m_cryptoKey;
	};

	class CIdcRegisterUserSrvDbReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_REGISTER_USER_MSG
		};
		DSC_BIND_ATTR(m_nId, m_info);
	public:
		ACE_UINT64 m_nId;
		DSC::CDscBlob m_info;
	};
	class CIdcRegisterUserDbSrvRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_REGISTER_USER_MSG
		};
		DSC_BIND_ATTR(m_nReturnCode);
	public:
		ACE_UINT32 m_nReturnCode;
	};

	class CIdcLoginCltSrvReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_LOGIN_MSG
		};
		DSC_BIND_ATTR(m_nId, m_cryptoKey);
	public:
		ACE_UINT64 m_nId;
		DSC::CDscBlob m_cryptoKey;
	};
	class CIdcLoginSrvCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_LOGIN_MSG
		};
		DSC_BIND_ATTR(m_nToken, m_nSessionId);
	public:
		ACE_UINT64 m_nToken;
		ACE_UINT32 m_nSessionId;
	};
	class CIdcQueryUserInfoCltSrvReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_QUERY_USER_INFO_MSG
		};
		DSC_BIND_ATTR(m_nSessionId, m_nToken);
	public:
		ACE_UINT32 m_nSessionId;
		ACE_UINT64 m_nToken;
	};
	class CIdcQueryUserInfoSrvCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_QUERY_USER_INFO_MSG
		};
		DSC_BIND_ATTR(m_nToken, m_info);
	public:
		ACE_UINT64 m_nToken;
		DSC::CDscBlob m_info;
	};
	class CIdcUpdateUserInfoCltSrvReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_UPDATE_USER_INFO_MSG
		};
		DSC_BIND_ATTR(m_nSessionId, m_nToken);
	public:
		ACE_UINT32 m_nSessionId;
		ACE_UINT64 m_nToken;
		DSC::CDscBlob m_info;
	};
	class CIdcUpdateUserInfoSrvCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_UPDATE_USER_INFO_MSG
		};
		DSC_BIND_ATTR(m_nToken);
	public:
		ACE_UINT64 m_nToken;
	};
	class CIdcUpdateTokenCltSrvReq
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_UPDATE_TOKEN_MSG
		};
		DSC_BIND_ATTR(m_nToken, m_nSessionId);
	public:
		ACE_UINT64 m_nToken;
		ACE_UINT32 m_nSessionId;
	};
	class CIdcUpdateTokenSrvCltRsp
	{
	public:
		enum
		{
			EN_MSG_ID = IDC::EN_IDC_UPDATE_TOKEN_MSG
		};
		DSC_BIND_ATTR(m_nToken, m_nSessionId);
	public:
		ACE_UINT64 m_nToken;
		ACE_UINT32 m_nSessionId;
	};

} // namespace IDC_SDK


#endif // !_IDC_COMMON_MSG_H_2417085051150945835




