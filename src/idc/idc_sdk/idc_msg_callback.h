#ifndef _IDC_MSG_CALLBACK_H_81028345710235
#define  _IDC_MSG_CALLBACK_H_81028345710235

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "idc/idc_common/idc_common_msg.h"

namespace IDC_SDK
{
#define BEGIN_CLIENT_BIND_TYPE_PROC \
	void TypeProc(const ACE_UINT32 nType, ACE_UINT32 nReqID, DSC::CDscBlob& info) \
	{ \
		switch(nType) \
		{ \

#define CLIENT_BIND_TYPE_PROC(PROPOSAL_TYPE) \
		case PROPOSAL_TYPE::EN_MSG_ID: \
		{ \
			PROPOSAL_TYPE prop; \
			if (DSC_UNLIKELY(DSC::Decode(prop, info.GetBuffer(), info.GetSize()))) \
			{ \
				return ; \
			} \
			else \
			{ \
				return this->OnTypeProc(nReqID, prop); \
			} \
            break; \
		}

#define END_CLIENT_BIND_TYPE_PROC \
		default: \
		{ \
			return ; \
		} \
		} \
	}

	class CIdcClient;
	class CIdcMsgCallback
	{
	public:
		CIdcMsgCallback(CIdcClient& clt) : m_client(clt){}
		virtual ~CIdcMsgCallback() {}
		virtual void OnReady() {}

		virtual void OnRegisterUserRsp(int nReqID, int nReturnCode, const char* cryptoUserKey, size_t cryptoUserKeyLen) {}
		virtual void OnLoginRsp(int nReqID, int nReturnCode) {}
		virtual void OnQueryUserInfoRsp(int nReqID, int nReturnCode, const char* userInfo, size_t userInfoLen) {}

		virtual void OnCommonRsp(ACE_INT32 nReqID, ACE_INT32 nReturnCode, ACE_INT32 nType, DSC::CDscBlob& data);


	private:
		BEGIN_CLIENT_BIND_TYPE_PROC
		CLIENT_BIND_TYPE_PROC(IDC::CIdcRegisterUserSrvCltRsp)
		CLIENT_BIND_TYPE_PROC(IDC::CIdcLoginSrvCltRsp)
		CLIENT_BIND_TYPE_PROC(IDC::CIdcQueryUserInfoSrvCltRsp)
		END_CLIENT_BIND_TYPE_PROC

		void OnTypeProc(ACE_UINT32 nReqID, IDC::CIdcRegisterUserSrvCltRsp& rsp);
		void OnTypeProc(ACE_UINT32 nReqID, IDC::CIdcLoginSrvCltRsp& rsp);
		void OnTypeProc(ACE_UINT32 nReqID, IDC::CIdcQueryUserInfoSrvCltRsp& rsp);

	private:
		CIdcClient& m_client;
	};


} // namespace IDC_SDK




#endif
