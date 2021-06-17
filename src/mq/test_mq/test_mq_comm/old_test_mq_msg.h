#pragma once

#include "codec/old_bind_attr.h"

class CTestNestMqMsg
{
public:
	BIND_ATTR4(m_nIntAttr, m_strAttr, m_datetimeAttr, m_optAttrl);

public:
	uint32_t m_nIntAttr;
	std::string m_strAttr;
	CODEC::CCodecDateTime m_datetimeAttr;
	CODEC::COptType<std::string> m_optAttrl;
};

class CTestMqMsgReq1
{
public:
	enum 
	{
		EN_TOPIC_ID = 1
	};

public:
	BIND_ATTR5(m_nIntAttr, m_strAttr, m_datetimeAttr, m_optAttrl, m_lstAttr);

public:
	uint32_t m_nIntAttr;
	std::string m_strAttr;
	CODEC::CCodecDateTime m_datetimeAttr;
	CODEC::COptType<std::string> m_optAttrl;
	std::list<CTestNestMqMsg> m_lstAttr;
};

class CTestMqMsgRsp1
{
public:
	enum
	{
		EN_TOPIC_ID = 2
	};

public:
	BIND_ATTR5(m_nIntAttr, m_strAttr, m_datetimeAttr, m_optAttrl, m_lstAttr);

public:
	uint32_t m_nIntAttr;
	std::string m_strAttr;
	CODEC::CCodecDateTime m_datetimeAttr;
	CODEC::COptType<std::string> m_optAttrl;
	std::list<CTestNestMqMsg> m_lstAttr;
};

class CTestMqMsgReq2
{
public:
	enum
	{
		EN_TOPIC_ID = 11
	};

public:
	BIND_ATTR4(m_nIntAttr, m_strAttr, m_datetimeAttr, m_optAttrl);

public:
	uint32_t m_nIntAttr;
	std::string m_strAttr;
	CODEC::CCodecDateTime m_datetimeAttr;
	CODEC::COptType<std::string> m_optAttrl;
	std::list<CTestNestMqMsg> m_lstAttr;
};

class CTestMqMsgRsp2
{
public:
	enum
	{
		EN_TOPIC_ID = 22
	};

public:
	BIND_ATTR5(m_nIntAttr, m_strAttr, m_datetimeAttr, m_optAttrl, m_lstAttr);

public:
	uint32_t m_nIntAttr;
	std::string m_strAttr;
	CODEC::CCodecDateTime m_datetimeAttr;
	CODEC::COptType<std::string> m_optAttrl;
	std::list<CTestNestMqMsg> m_lstAttr;
};
