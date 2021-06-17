#ifdef VBH_USE_SMX //  π”√ecdsa

void VBH::vbhDigest(const char* data, size_t n, char* digest)
{
	EVP_MD_CTX* mdCtx = EVP_MD_CTX_new();
	EVP_MD* md = EVP_sm3();
	unsigned int mdLen = VBH_BLOCK_DIGEST_LENGTH;

	EVP_DigestInit_ex(mdCtx, md, NULL);
	EVP_DigestUpdate(mdCtx, data, n);
	EVP_DigestFinal_ex(mdCtx, (unsigned char*)digest, &mdLen);
	EVP_MD_CTX_free(mdCtx);
}

#else

inline char* VBH::vbhSign(EC_KEY* pEcKey, char* buff, int buff_len, unsigned int& nSignature)
{
	unsigned char* pSignature = (unsigned char*)DSC_THREAD_MALLOC(ECDSA_size(pEcKey));

	if (ECDSA_sign(0, (unsigned char*)buff, buff_len, pSignature, &nSignature, pEcKey) != 1)
	{
		DSC_THREAD_FREE((char*)pSignature);
		pSignature = NULL;
	}

	EC_KEY_free(pEcKey);

	return (char*)pSignature;
}

inline bool VBH::vbhVerifySign(EC_KEY* pEcKey, char* buff, size_t buff_len, char* pSignature, const int nSignature)
{
	int nRet = ECDSA_verify(0, (unsigned char*)buff, buff_len, (unsigned char*)pSignature, nSignature, pEcKey);

	EC_KEY_free(pEcKey);

	return (1 == nRet);
}

inline void VBH::vbhDigest(const char* data, size_t n, char* digest)
{
	SHA256((const unsigned char*)data, n, (unsigned char*)digest);
}

#endif
