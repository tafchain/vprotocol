#ifndef MQ_CLIENT_SDK_COMMON_DEF_H_578234809234897243872341685
#define MQ_CLIENT_SDK_COMMON_DEF_H_578234809234897243872341685

#include <unordered_map>

#include "mq/mq_client_sdk/mq_client_sdk_export.h"
namespace MQ
{
	class CTopicMqMsg
	{
	public:
		unsigned int m_nTopicID;
		unsigned int m_nMsgSize;
		char* m_pMsg;
	};

	class CAddrMqMsg
	{
	public:
		unsigned int m_nClientID;//消息发布者ID，回消息应答用addr
		unsigned int m_nTopicID;
		unsigned int m_nMsgSize;
		char* m_pMsg;
	};

	class IMqMsgCallback
	{
	public:
		virtual void OnReady(void) = 0;
		virtual void OnAbnormal(void) = 0;
		virtual void OnMqMsg(MQ::CAddrMqMsg& rMqMsg) = 0;
		virtual void OnExit(void) = 0;
	};

	class CInitMqClientSdkParam
	{
	public:
		CInitMqClientSdkParam()
			: m_pMqMsgCallback(nullptr)
			, m_nClientID(0)
		{
		}

	public:
		unsigned int m_nClientID;//约束：要求每个与mq相连的客户端都必须有个clientID，该ID不能变
		MQ::IMqMsgCallback* m_pMqMsgCallback;
		unsigned int* m_pTopicID;
		unsigned int m_nTopicIDNum;
	};

	class IMqMsgProc
	{
	public:
		virtual int OnMqMsg(MQ::CAddrMqMsg& rMqMsg) = 0;
	};

	template<typename MQ_MSG_TYPE, typename MQ_MSG_PROC>
	class CMqMsgProc : public IMqMsgProc
	{
	public:
		CMqMsgProc(MQ_MSG_PROC& rMqMsgProc);

	public:
		int OnMqMsg(MQ::CAddrMqMsg& rMqMsg);

	private:
		MQ_MSG_PROC& m_rMqMsgProc;
	};

	template<typename MQ_MSG_TYPE, typename MQ_MSG_PROC>
	IMqMsgProc* CreateMqMsgProc(MQ_MSG_TYPE& rMsg, MQ_MSG_PROC& m_rMqMsgProc);

	class MQ_CLIENT_SDK_EXPORT CMqMsgCallback : public IMqMsgCallback
	{
	public:
		CMqMsgCallback();

	public:
		void Init(void);
		virtual void BindMqMsg(void);
		void OnMqMsg(MQ::CAddrMqMsg& rMqMsg);

	protected:
		void RegistMqMsgProc(const unsigned int nTopicID, MQ::IMqMsgProc* pMqMsgProc);

	private:
		typedef std::unordered_map<unsigned int, IMqMsgProc*> mq_msg_proc_type;

	private:
		mq_msg_proc_type m_mapMqMsgProc;
	};
}

#define BEGIN_BIND_MQ_MESSAGE \
	virtual void BindMqMsg(void) \
	{ \

#define BIND_MQ_MESSAGE(MQ_MSG_TYPE) \
		{ \
			MQ_MSG_TYPE msg; \
			MQ::IMqMsgProc* pMqMsgProc = MQ::CreateMqMsgProc(msg, *this); \
			this->RegistMqMsgProc(MQ_MSG_TYPE::EN_TOPIC_ID, pMqMsgProc); \
		}

#define END_BIND_MQ_MESSAGE \
	}

#include "mq_client_sdk/mq_client_sdk_common_def.inl"

#endif
