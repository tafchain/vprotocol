#ifndef _IDC_CLIENT_H_781234501239872958
#define _IDC_CLIENT_H_781234501239872958

#include "idc/idc_sdk/idc_sdk_export.h"
#include "idc/idc_sdk/idc_msg_callback.h"

namespace IDC_SDK
{
	
	class IDC_SDK_EXPORT CIdcClient
	{
	public:
		CIdcClient();
		~CIdcClient();
		int Init(CIdcMsgCallback* callbacks);
		int RegisterUser(int reqID, unsigned long long IDNum, const char*userInfo, size_t userInfoLen);
		int Login(int reqID, unsigned long long IDNum, const char* key, size_t keyLen);
		int QueryUserInfo(int reqID);
		int UpdateUserInfo(int reqID, const char* userInfo, size_t userInfoLen);
		

		
	private:
		friend class CIdcMsgCallback;

		void OnGetSessionId(ACE_UINT32 nSession);
		void OnRefreshToken(ACE_UINT64 nToken);
		void RefreshToken();
		ACE_UINT32 m_nSessionId;
		ACE_UINT64 m_nToken;
		ACE_UINT64 m_nUid;
		CIdcMsgCallback* m_callbacks;
	};
	
}









#endif // !_IDC_CLIENT_H_781234501239872958



