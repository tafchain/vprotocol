#include "dsc/dsc_log.h"

#include "mq_client_sdk/mq_client_sdk_common_def.h"

MQ::CMqMsgCallback::CMqMsgCallback()
{
}

void MQ::CMqMsgCallback::Init(void)
{
	this->BindMqMsg();
}

void MQ::CMqMsgCallback::OnMqMsg(MQ::CAddrMqMsg& rMqMsg)
{
	mq_msg_proc_type::iterator it = m_mapMqMsgProc.find(rMqMsg.m_nTopicID);
	if (it == m_mapMqMsgProc.end())
	{
		DSC_RUN_LOG_WARNING("cann't find  msg proc, topic id:%d", rMqMsg.m_nTopicID);
	}
	else
	{
		if (it->second->OnMqMsg(rMqMsg))
		{
			DSC_RUN_LOG_WARNING("decode msg error, topic id:%d", rMqMsg.m_nTopicID);
		}
	}
}

void MQ::CMqMsgCallback::BindMqMsg(void)
{
}

void MQ::CMqMsgCallback::RegistMqMsgProc(const unsigned int nTopicID, MQ::IMqMsgProc* pMqMsgProc)
{
	mq_msg_proc_type::iterator it = m_mapMqMsgProc.find(nTopicID);
	if (it == m_mapMqMsgProc.end())
	{
		m_mapMqMsgProc.insert(mq_msg_proc_type::value_type(nTopicID, pMqMsgProc));
	}
	else
	{
		DSC_RUN_LOG_WARNING("repeat mq msg proc regist, topic id:%d", nTopicID);
	}
}
