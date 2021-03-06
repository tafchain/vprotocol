#ifndef _IDC_COMMON_ID_DEF_H_134750235656
#define _IDC_COMMON_ID_DEF_H_134750235656


namespace IDC
{
	enum
	{
		EN_IDC_SUCCESS = 0,
		EN_IDC_ERROR_TIMEOUT = 1,
	};

	// msg type
	enum {
		EN_IDC_COMMON_REQ_MSG = 1,
		EN_IDC_INIT_CLIENT_SDK_MSG,
		EN_IDC_REGISTER_USER_MSG,
		EN_IDC_PUB_CRYPTO_KEY_MSG,
		EN_IDC_LOGIN_MSG,
		EN_IDC_UPDATE_TOKEN_MSG,
		EN_IDC_QUERY_USER_INFO_MSG,
		EN_IDC_UPDATE_USER_INFO_MSG,
	};

	enum 
	{
		EN_IDC_REQ_TYPE_REGISTER_USER = 100,
	};

	enum
	{
		EN_ID_CHAIN_APP_TYPE = 789,
		EN_IDC_SERVER_TYPE = 791,
		EN_ID_I_DOM_CLIENT_TEST_TYPE = 792,
		EN_IDC_DB_SERVER_APP_TYPE = 793,
		EN_IDC_DB_SERVER_SERVICE_TYPE = 794,
	};

	
}


#endif
