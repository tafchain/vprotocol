#ifndef CRYPT_KEY_CODEC_H_ODHFSHFOSDIHFDSKLLKFHSDLILJLKF
#define CRYPT_KEY_CODEC_H_ODHFSHFOSDIHFDSKLLKFHSDLILJLKF

#include "common/vbh_comm_def_export.h"
#include "common/vbh_encrypt_lib.h"
#include "common/vbh_comm_msg_def.h"

namespace VBH
{
	//≥ı ºªØopensslø‚
	VBH_COMM_DEF_EXPORT ACE_INT32 EncodeGenerateUserKey(CDscString& stEncodeGenUserKey, ACE_UINT32 nChannelID, COrderGenerateKey& genUserKey);
	VBH_COMM_DEF_EXPORT ACE_INT32 DecodeGenerateUserKey(DSC::CDscBlob& encodeGenUserKey, ACE_UINT32& nChannelID, COrderGenerateKey& genUserKey);

	VBH_COMM_DEF_EXPORT ACE_INT32 EncodeClientCryptKey(CDscString& strClientCryptKey, ACE_UINT32 nChannelID, COrderGenerateKey& genUserKey, CDscString& pubCryptoKey, CDscString& priCryptoKey, CDscString& envelopeKey);
	VBH_COMM_DEF_EXPORT ACE_INT32 DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, ACE_UINT32& nChannelID, COrderGenerateKey& genUserKey, CDscString& pubCryptoKey, CDscString& priCryptoKey, CDscString& envelopeKey);
	VBH_COMM_DEF_EXPORT ACE_INT32 DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, ACE_UINT32& nChannelID, COrderGenerateKey& genUserKey);
	VBH_COMM_DEF_EXPORT ACE_INT32 DecodeClientCryptKey(DSC::CDscBlob& userCryptoKey, ACE_UINT32& nChannelID);
}

#endif
