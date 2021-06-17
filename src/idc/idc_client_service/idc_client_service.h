#ifndef _IDC_CLIENT_SERVICE_H_8871034890123458
#define _IDC_CLIENT_SERVICE_H_8871034890123458

#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/dsc_msg_bind_proc_macros.h"

#include "idc_common/idc_common_msg.h"
#include "idc_sdk/idc_client.h"
#include "vbh/common/vbh_comm_id_def.h"

namespace IDC_SDK
{

	class PLUGIN_EXPORT CIdcClientService : public CDscHtsClientService
	{
	public:
		enum
		{
			EN_SERVICE_TYPE = IDC::EN_ID_CHAIN_APP_TYPE
		};

	public:
		ACE_INT32 OnInit(void);
		ACE_INT32 OnExit(void);

	protected:
		BEGIN_BIND_DSC_MESSAGE
		DSC_BIND_MESSAGE(IDC::CInitClientSdk)
		DSC_BIND_MESSAGE(IDC::CIdcCommonApiCltReq)
		END_BIND_DSC_MESSAGE

	public:
		void OnDscMsg(IDC::CInitClientSdk& rInitSdk, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);
		void OnDscMsg(IDC::CIdcCommonApiCltReq& rCommonReq, const CDscMsg::CDscMsgAddr& rSrcMsgAddr);

	protected:
		BEGIN_HTS_MESSAGE_BIND
		BIND_HTS_MESSAGE(IDC::CIdcCommonSrvCltRsp)
		END_HTS_MESSAGE_BIND

	public:
		ACE_INT32 OnHtsMsg(IDC::CIdcCommonSrvCltRsp& rIDomCommonRsp, CMcpHandler* pMcpHandler);


	private:
		enum
		{
#if defined(_ANDROID_PLATFORM_) || defined(_ARM_LINUX_PLATFORM_) || defined(_IOS_PLATFORM_)
			EN_HASH_MAP_BITES = 6,
#else
			EN_HASH_MAP_BITES = 16,
#endif
			EN_SESSION_TIMEOUT_VALUE = 60
		};

		class CIDcServer
		{
		public:
			CDscString m_strIDcServerIpAddr;
			ACE_INT32 m_nIDcServerPort = 0;
			ACE_UINT32 m_nHandleID = 0;
		};

		class CIDcPeer
		{
		public:
			ACE_INT32 m_nPeerID = 0;
			DSC::CDscList <CIDcServer> m_lstIDcServer;
		};

		using CMcpHandlerPtr = CMcpHandler *;

		class CIDcPeerSession
		{
		public:
			CMcpHandlerPtr* m_ppMcpHandler = NULL;
			ACE_INT32 m_nPeerID = 0;
			ACE_UINT32 m_nHandleIDNum = 0;
			ACE_UINT32 m_nIndex = 0;
		};

		class CIDcKernelSession
		{
		public:
			CIDcPeerSession* m_parrPeerSession = NULL;
			ACE_UINT32 m_nPeerSessionNum = 0;
			ACE_UINT32 m_nIndex = 0;
		};

		class CCommonSession : public CDscServiceTimerHandler
		{
		public:
			CCommonSession(CIdcClientService& rRegistUserService):m_rClientService(rRegistUserService){}

		public:
			virtual void OnTimer(void);
			int m_nReqId;
			int m_nType;

		public:
			ACE_UINT32 m_nIDomSessionID = 0;

		public:
			ACE_UINT32 m_nKey = 0;
			CDscString m_strNonce;
			CCommonSession* m_pPrev = NULL;
			CCommonSession* m_pNext = NULL;
		protected:
			CIdcClientService& m_rClientService;
		};

		using common_map_type = CBareHashMap<ACE_UINT32, CCommonSession, EN_HASH_MAP_BITES>;

		ACE_INT32 LoadPeerInfo(void);
		CMcpHandler* GetMcpHandler(void);
		ACE_INT32 CommonApiReq(IDC::CIdcCommonApiCltReq& rIDomCommonApiCltReq);
		void EncodeCommonReq(IDC::CIdcCommonReqDataWrapper &wrapper, const unsigned char* envolopeKey, char*&outBuf, ACE_UINT32 & outLen);
		void OnCommonRsp(ACE_INT32 nReqID, ACE_INT32 nReturnCode, ACE_INT32 nType, DSC::CDscBlob& data);

		common_map_type m_mapCommonSession;

		bool m_bReady = false;
		bool m_bLogin = false;
		ACE_UINT32 m_nSessionID = 0;
		IDC_SDK::CIdcMsgCallback* m_pCallback = NULL;
		ACE_UINT32 m_nRandSeed = 0;

		CDscString m_peerEnvelopeKey;
		CDscString m_userEnvelopeKey;

		DSC::CDscList <CIDcPeer> m_lstIDcPeer;
		CIDcKernelSession m_idcKernelSession;

	};


}
#endif // !_IDC_CLIENT_SERVICE_H_8871034890123458



