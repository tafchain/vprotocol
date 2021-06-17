#ifndef COMM_MSG_DEF_H_423809423798432798479843286468432849894865484684386456
#define COMM_MSG_DEF_H_423809423798432798479843286468432849894865484684386456

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/codec/json_codec/json_codec_bind_macros.h"

#include "demo/common/comm_id_def.h"

namespace NS_DEMO_MSG_DEF
{
	class CDemoNestMag
	{
	public:
		DSC_BIND_ATTR(m_lstAttr, m_strAttr, m_dtAttr);

	public:
		DSC::CDscList<ACE_INT32> m_lstAttr;
		CDscString m_strAttr;
		DSC::CDscDateTime m_dtAttr;
	};

	//class CDemoReqMsg
	//{
	//public:
	//	enum
	//	{
	//		EN_MSG_ID = NS_DEMO_ID_DEF::EN_DEMO_REQ_MSG
	//	};

	//public:
	//	DSC_BIND_ATTR(m_lstdemoNestMag, m_demoNestMag, m_lstAttr, m_strAttr, m_dtAttr);

	//public:
	//	DSC::CDscList<NS_DEMO_MSG_DEF::CDemoNestMag> m_lstdemoNestMag;
	//	NS_DEMO_MSG_DEF::CDemoNestMag m_demoNestMag;
	//	DSC::CDscList<ACE_INT32> m_lstAttr;
	//	CDscString m_strAttr;
	//	DSC::CDscDateTime m_dtAttr;
	//};

	//class CDemoRspMsg
	//{
	//public:
	//	enum
	//	{
	//		EN_MSG_ID = NS_DEMO_ID_DEF::EN_DEMO_RSP_MSG
	//	};

	//public:
	//	DSC_BIND_ATTR(m_lstdemoNestMag, m_demoNestMag, m_lstAttr, m_strAttr, m_dtAttr);

	//public:
	//	DSC::CDscList<NS_DEMO_MSG_DEF::CDemoNestMag> m_lstdemoNestMag;
	//	NS_DEMO_MSG_DEF::CDemoNestMag m_demoNestMag;
	//	DSC::CDscList<ACE_INT32> m_lstAttr;
	//	CDscString m_strAttr;
	//	DSC::CDscDateTime m_dtAttr;
	//};

	class CDemoReqMsg
	{
	public:
		enum
		{
			EN_MSG_ID = NS_DEMO_ID_DEF::EN_DEMO_REQ_MSG
		};

	public:
		DSC_BIND_ATTR(m_nAttr);

	public:
		ACE_INT32 m_nAttr;
	};

	class CDemoRspMsg
	{
	public:
		enum
		{
			EN_MSG_ID = NS_DEMO_ID_DEF::EN_DEMO_RSP_MSG
		};

	public:
		DSC_BIND_ATTR(m_nAttr);

	public:
		ACE_INT32 m_nAttr;
	};

	class CDemoJsonNestMsg
	{
	public:
		JSON_BIND_ATTR(m_lstAttr, m_strAttr, m_dtAttr);

	public:
		DSC::CDscList<ACE_INT32> m_lstAttr;
		CDscString m_strAttr;
		DSC::CDscDateTime m_dtAttr;
	};

	class CDemoJsonReqMsg
	{
	public:
		JSON_BIND_ATTR(m_lstDemoJsonNestMag, m_demoJsonNestMag, m_lstAttr, m_strAttr, m_dtAttr);

	public:
		DSC::CDscList<CDemoJsonNestMsg> m_lstDemoJsonNestMag;
		CDemoJsonNestMsg m_demoJsonNestMag;
		DSC::CDscList<ACE_INT32> m_lstAttr;
		CDscString m_strAttr;
		DSC::CDscDateTime m_dtAttr;
	};

	class CDemoJsonRspMsg
	{
	public:
		enum
		{
			EN_MSG_ID = NS_DEMO_ID_DEF::EN_DEMO_JSON_RSP_MSG
		};

	public:
		JSON_BIND_ATTR(m_lstDemoJsonNestMag, m_demoJsonNestMag, m_lstAttr, m_strAttr, m_dtAttr);

	public:
		DSC::CDscList<CDemoJsonNestMsg> m_lstDemoJsonNestMag;
		CDemoJsonNestMsg m_demoJsonNestMag;
		DSC::CDscList<ACE_INT32> m_lstAttr;
		CDscString m_strAttr;
		DSC::CDscDateTime m_dtAttr;
	};

	class COptSumMsg
	{
	public:
		enum
		{
			EN_MSG_ID = NS_DEMO_ID_DEF::EN_OPT_SUM_MSG
		};

	public:
		DSC_BIND_ATTR(m_nSummary);

	public:
		ACE_UINT64 m_nSummary;
	};

	class CJsonSumMsg
	{
	public:
		enum
		{
			EN_MSG_ID = NS_DEMO_ID_DEF::EN_JSON_SUM_MSG
		};

	public:
		DSC_BIND_ATTR(m_nSummary);

	public:
		ACE_INT32 m_nSummary;
	};

	class CMcpSumMsg
	{
	public:
		enum
		{
			EN_MSG_ID = NS_DEMO_ID_DEF::EN_MCP_SUM_MSG
		};

	public:
		DSC_BIND_ATTR(m_nSummary);

	public:
		ACE_INT32 m_nSummary;
	};
}

#endif