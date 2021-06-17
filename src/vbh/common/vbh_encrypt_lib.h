#ifndef VBH_ENCRYPT_LIB_H_7FAC6382B1DD11E9B5D660F18A3A20D1
#define VBH_ENCRYPT_LIB_H_7FAC6382B1DD11E9B5D660F18A3A20D1

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/sha.h>

#include "dsc/codec/codec_base/dsc_codec_base.h"

#include "common/vbh_comm_def_export.h"

#define VBH_ENVELOPE_KEY_MAX_LENGTH (EVP_MAX_KEY_LENGTH * 2) //TODO: ���޸�
//TODO: ���������Կ�Ĺ��ߺ���

//��ϣժҪ�ĳ���
#ifdef VBH_USE_SMX
#define VBH_BLOCK_DIGEST_LENGTH EVP_MAX_MD_SIZE 
#else
#define VBH_BLOCK_DIGEST_LENGTH SHA256_DIGEST_LENGTH 
#endif

namespace VBH
{
	//��ʼ��openssl��
	VBH_COMM_DEF_EXPORT void InitOpenSsl(void);

	VBH_COMM_DEF_EXPORT EC_GROUP* vbhEcGroup(void);
	VBH_COMM_DEF_EXPORT EC_KEY* vbhCreateKey(EC_GROUP* pGroup);
	VBH_COMM_DEF_EXPORT char* vbhGetHexFromPublicKey(EC_KEY* key);
	VBH_COMM_DEF_EXPORT char* vbhGetHexFromPrivateKey(EC_KEY* key);
	VBH_COMM_DEF_EXPORT EC_KEY* vbhCreateKeyFromPublicHex(const char* hex, EC_GROUP* pGroup);
	VBH_COMM_DEF_EXPORT EC_KEY* vbhCreateKeyFromPrivateHex(const char* hex, EC_GROUP* pGroup);

	//�ԳƼ��ܺ���; ���ص�ָ����ʹ����Ϻ���Ҫ���� DSC_THREAD_FREE �����ͷ�
	VBH_COMM_DEF_EXPORT char* vbhEncrypt(const unsigned char* envelope_key, int& nOutLen, char* data, const int nDataLen);

	//�Գƽ��ܺ���; ���ص�ָ����ʹ����Ϻ���Ҫ���� DSC_THREAD_FREE �����ͷ�
	VBH_COMM_DEF_EXPORT char* vbhDecrypt(const unsigned char* envelope_key, int& nOutLen, char* data, const int nDataLen);



#ifdef VBH_USE_SMX //ʹ�ù���ʱ��Ҫ��������
	//ǩ��������//pEcKey �� vbh_sign �лᱻ�ͷ�
	VBH_COMM_DEF_EXPORT char* vbhSign(EC_KEY* pEcKey, char* buff, int buff_len, unsigned int& nSignature);

	//��ǩ������//pEcKey �� vbh_verify_sign �лᱻ�ͷ�
	VBH_COMM_DEF_EXPORT bool vbhVerifySign(EC_KEY* pEcKey, char* buff, size_t buff_len, char* pSignature, const int nSignature);

	//�����ϣժҪ����; ���������õ���
	void vbhDigest(const char* data, size_t n, char* digest);

#else //ʹ��aes-gcmʱ����Ҫ����
	//ǩ��������//pEcKey �� vbh_sign �лᱻ�ͷ�
	char* vbhSign(EC_KEY* pEcKey, char* buff, int buff_len, unsigned int& nSignature);

	//��ǩ������//pEcKey �� vbh_verify_sign �лᱻ�ͷ�
	bool vbhVerifySign(EC_KEY* pEcKey, char* buff, size_t buff_len, char* pSignature, const int nSignature);

	//�����ϣժҪ����; ���������õ���
	void vbhDigest(const char* data, size_t n, char* digest);

#endif

}

#include "common/vbh_encrypt_lib.inl"

#endif
