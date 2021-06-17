#include "dsc/dsc_base64.h"

#include "common/vbh_comm_wrap_msg_def.h"
#include "common/crypt_key_codec.h"
#include "common/vbh_comm_func.h"

//togo 王耿写注释
//细节：参考下面ACE_INT32 VBH::DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, COrderGenerateKey& genUserKey)实现， m_genUserKey一定要第一个绑定
class CChannelWrapper
{
public:
	CChannelWrapper(ACE_UINT32& nChannelID)
		: m_nChannelID(nChannelID)
	{
	}

public:
	DSC_BIND_ATTR(m_nChannelID);

public:
	ACE_UINT32& m_nChannelID;
};

class CChannelKeyWrapper
{
public:
	CChannelKeyWrapper(ACE_UINT32& nChannelID, VBH::COrderGenerateKey& genUserKey)
	: m_nChannelID(nChannelID)
	, m_genUserKey(genUserKey)
	{
	}

public:
	DSC_BIND_ATTR(m_nChannelID, m_genUserKey);

public:
	ACE_UINT32& m_nChannelID;
	VBH::COrderGenerateKey& m_genUserKey;
};

class CClientCryptKeyWrapper //客户端端存放秘钥数据的wrapper
{
public:
	CClientCryptKeyWrapper(ACE_UINT32& nChannelID, VBH::COrderGenerateKey& genUserKey, CDscString& pubCryptoKey, CDscString& priCryptoKey, CDscString& envelopeKey)
	: m_nChannelID(nChannelID)
	, m_genUserKey(genUserKey)
	, m_pubCryptoKey(pubCryptoKey)
	, m_priCryptoKey(priCryptoKey)
	, m_envelopeKey(envelopeKey)
	{
	}

public:
	DSC_BIND_ATTR(m_nChannelID, m_genUserKey, m_pubCryptoKey, m_priCryptoKey, m_envelopeKey);

public:
	ACE_UINT32& m_nChannelID;
	VBH::COrderGenerateKey& m_genUserKey;
	CDscString& m_pubCryptoKey;
	CDscString& m_priCryptoKey;
	CDscString& m_envelopeKey;
};

ACE_INT32 VBH::EncodeGenerateUserKey(CDscString& stEncodeGenUserKey, ACE_UINT32 nChannelID, COrderGenerateKey& genUserKey)
{
	char* pBuffer;
	size_t nBufSize;
	CChannelKeyWrapper wrapper(nChannelID, genUserKey);

	DEF_ENCODE(wrapper, pBuffer, nBufSize);
	int nBase64StringSize = ::Base64encode_len(nBufSize);
	char* pBase64String = DSC_THREAD_SIZE_MALLOC(nBase64StringSize);

	Base64encode(pBase64String, pBuffer, nBufSize);
	DSC_THREAD_SIZE_FREE(pBuffer, nBufSize);

	stEncodeGenUserKey.assign(pBase64String, nBase64StringSize);
	DSC_THREAD_SIZE_FREE(pBase64String, nBase64StringSize);

	return 0;
}

VBH_COMM_DEF_EXPORT ACE_INT32 VBH::DecodeGenerateUserKey(DSC::CDscBlob& encodeGenUserKey, ACE_UINT32& nChannelID, COrderGenerateKey& genUserKey)
{
	if (encodeGenUserKey.GetSize() > 0)
	{
		CChannelKeyWrapper wrapper(nChannelID, genUserKey);

		char* pBuffer = (char*)alloca(encodeGenUserKey.GetSize()); //解码后码流大小 小于原先base码流 //待解码数据大小已知，且比较小，可以从栈上开辟空间
		size_t nBufferLen = ::Base64decode(pBuffer, encodeGenUserKey.GetBuffer());

		if (DSC::Decode(wrapper, pBuffer, nBufferLen))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

ACE_INT32 VBH::EncodeClientCryptKey(CDscString& strClientCryptKey, ACE_UINT32 nChannelID, COrderGenerateKey& genUserKey, CDscString& pubCryptoKey, CDscString& priCryptoKey, CDscString& envelopeKey)
{
	char* pBuffer;
	size_t nBufSize;
	CClientCryptKeyWrapper wrapper(nChannelID, genUserKey, pubCryptoKey, priCryptoKey, envelopeKey);

	DSC::Encode(wrapper, pBuffer, nBufSize);

	if (pBuffer)
	{
		int nBase64StringSize = ::Base64encode_len(nBufSize);
		char* pBase64String = DSC_THREAD_SIZE_MALLOC(nBase64StringSize);

		Base64encode(pBase64String, pBuffer, nBufSize);
		DSC_THREAD_SIZE_FREE(pBuffer, nBufSize);

		strClientCryptKey.assign(pBase64String, nBase64StringSize);
		DSC_THREAD_SIZE_FREE(pBase64String, nBase64StringSize);

		return 0;
	}
	else
	{
		return -1;
	}
}

ACE_INT32 VBH::DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, ACE_UINT32& nChannelID, COrderGenerateKey& genUserKey, CDscString& pubCryptoKey, CDscString& priCryptoKey, CDscString& envelopeKey)
{
	if (userCryptoKey.GetSize() > 0)
	{
		ACE_INT32 nReturnCode = 0;
		CClientCryptKeyWrapper wrapper(nChannelID, genUserKey, pubCryptoKey, priCryptoKey, envelopeKey);

		char* pBuffer = DSC_THREAD_SIZE_MALLOC(userCryptoKey.GetSize()); //解码后码流大小 小于原先base码流
		size_t nBufferLen = ::Base64decode(pBuffer, userCryptoKey.GetBuffer());

		if (DSC::Decode(wrapper, pBuffer, nBufferLen))
		{
			nReturnCode = -1;
		}
		else
		{
			nReturnCode = 0;
		}

		DSC_THREAD_SIZE_FREE(pBuffer, userCryptoKey.GetSize());

		return nReturnCode;
	}
	else
	{
		return -1;
	}
}

//这里算法成立约束:参考CClientCryptKeyWrappers设计，属性m_genUserKey第一个绑定，
ACE_INT32 VBH::DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, ACE_UINT32& nChannelID, COrderGenerateKey& genUserKey)
{
	if (userCryptoKey.GetSize() > 0)
	{
		char* pBuffer = DSC_THREAD_SIZE_MALLOC(userCryptoKey.GetSize()); //解码后码流大小 小于原先base码流
		size_t nBufferLen = ::Base64decode(pBuffer, userCryptoKey.GetBuffer());
		CChannelKeyWrapper key(nChannelID, genUserKey);

		if (DSC::Decode(key, pBuffer, nBufferLen))
		{
			DSC_THREAD_SIZE_FREE(pBuffer, userCryptoKey.GetSize());

			return -1;
		}
		else
		{
			DSC_THREAD_SIZE_FREE(pBuffer, userCryptoKey.GetSize());

			return 0;
		}
	}
	else
	{
		return -1;
	}
}

ACE_INT32 VBH::DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, ACE_UINT32& nChannelID)
{
	if (userCryptoKey.GetSize() > 0)
	{
		char* pBuffer = DSC_THREAD_SIZE_MALLOC(userCryptoKey.GetSize()); //解码后码流大小 小于原先base码流
		size_t nBufferLen = ::Base64decode(pBuffer, userCryptoKey.GetBuffer());
		CChannelWrapper ch(nChannelID);

		if (DSC::Decode(ch, pBuffer, nBufferLen))
		{
			DSC_THREAD_SIZE_FREE(pBuffer, userCryptoKey.GetSize());

			return -1;
		}
		else
		{
			DSC_THREAD_SIZE_FREE(pBuffer, userCryptoKey.GetSize());

			return 0;
		}
	}
	else
	{
		return -1;
	}
}
