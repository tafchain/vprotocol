#ifndef VBH_ENCRYPT_LIB_H_7FAC6382B1DD11E9B5D660F18A3A20D1
#define VBH_ENCRYPT_LIB_H_7FAC6382B1DD11E9B5D660F18A3A20D1

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/sha.h>

#include "dsc/codec/codec_base/dsc_codec_base.h"

#include "common/vbh_comm_def_export.h"

#define VBH_ENVELOPE_KEY_MAX_LENGTH (EVP_MAX_KEY_LENGTH * 2) //TODO: 待修改
//TODO: 添加生成秘钥的工具函数

//哈希摘要的长度
#ifdef VBH_USE_SMX
#define VBH_BLOCK_DIGEST_LENGTH EVP_MAX_MD_SIZE 
#else
#define VBH_BLOCK_DIGEST_LENGTH SHA256_DIGEST_LENGTH 
#endif

namespace VBH
{
	//初始化openssl库
	VBH_COMM_DEF_EXPORT void InitOpenSsl(void);

	VBH_COMM_DEF_EXPORT EC_GROUP* vbhEcGroup(void);
	VBH_COMM_DEF_EXPORT EC_KEY* vbhCreateKey(EC_GROUP* pGroup);
	VBH_COMM_DEF_EXPORT char* vbhGetHexFromPublicKey(EC_KEY* key);
	VBH_COMM_DEF_EXPORT char* vbhGetHexFromPrivateKey(EC_KEY* key);
	VBH_COMM_DEF_EXPORT EC_KEY* vbhCreateKeyFromPublicHex(const char* hex, EC_GROUP* pGroup);
	VBH_COMM_DEF_EXPORT EC_KEY* vbhCreateKeyFromPrivateHex(const char* hex, EC_GROUP* pGroup);

	//对称加密函数; 返回的指针在使用完毕后需要调用 DSC_THREAD_FREE 进行释放
	VBH_COMM_DEF_EXPORT char* vbhEncrypt(const unsigned char* envelope_key, int& nOutLen, char* data, const int nDataLen);

	//对称解密函数; 返回的指针在使用完毕后需要调用 DSC_THREAD_FREE 进行释放
	VBH_COMM_DEF_EXPORT char* vbhDecrypt(const unsigned char* envelope_key, int& nOutLen, char* data, const int nDataLen);



#ifdef VBH_USE_SMX //使用国密时需要导出符号
	//签名函数；//pEcKey 在 vbh_sign 中会被释放
	VBH_COMM_DEF_EXPORT char* vbhSign(EC_KEY* pEcKey, char* buff, int buff_len, unsigned int& nSignature);

	//验签函数；//pEcKey 在 vbh_verify_sign 中会被释放
	VBH_COMM_DEF_EXPORT bool vbhVerifySign(EC_KEY* pEcKey, char* buff, size_t buff_len, char* pSignature, const int nSignature);

	//计算哈希摘要函数; 内联，不用导出
	void vbhDigest(const char* data, size_t n, char* digest);

#else //使用aes-gcm时，需要内联
	//签名函数；//pEcKey 在 vbh_sign 中会被释放
	char* vbhSign(EC_KEY* pEcKey, char* buff, int buff_len, unsigned int& nSignature);

	//验签函数；//pEcKey 在 vbh_verify_sign 中会被释放
	bool vbhVerifySign(EC_KEY* pEcKey, char* buff, size_t buff_len, char* pSignature, const int nSignature);

	//计算哈希摘要函数; 内联，不用导出
	void vbhDigest(const char* data, size_t n, char* digest);

#endif

}

#include "common/vbh_encrypt_lib.inl"

#endif
