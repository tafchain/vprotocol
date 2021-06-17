#ifndef VBH_TEST_USER_INFO_DEF_H_47942384236842364368234
#define VBH_TEST_USER_INFO_DEF_H_47942384236842364368234

#include "dsc/codec/dsc_codec/dsc_codec.h"
#include "dsc/codec/json_codec/json_codec_bind_macros.h"
#include "vbh_test_comm_def.h"

enum
{
	EN_TEST_CHANNEL_ID = VBH_TEST::EN_TEST_UPDATE_USER_CHANNEL_ID
};


class CTestBcUserKey
{
public:
	bool operator== (const CTestBcUserKey& rTestUserKey) const
	{
		return IDNumber == rTestUserKey.IDNumber;
	}
public:
	DSC_BIND_ATTR(IDNumber)
	JSON_BIND_ATTR1(IDNumber)

public:
	ACE_UINT64 IDNumber;
};

class CTestBcUserInfo
{
public:
	DSC_BIND_ATTR(age, phoneNumber, userName, address)
	JSON_BIND_ATTR4(age, phoneNumber, userName, address)

public:
	ACE_UINT16 age;
	ACE_UINT64 phoneNumber;
	CDscString userName;
	CDscString address;
};

class CTestUser
{
public:
	DSC_BIND_ATTR(userKey, userInfo)
	JSON_BIND_ATTR2(userKey, userInfo)

public:
	CTestBcUserKey userKey;
	CTestBcUserInfo userInfo;
};

//用户A转币种n_nCoinType的币n_nCoinNum
class CTestUpdateUser
{
public:
	enum
	{
		EN_ACTION_ID = VBH_TEST::EN_TEST_UPDATE_USER_ACTION_ID
	};

public:
	DSC_BIND_ATTR(user)
	JSON_BIND_ATTR1(user)

public:
	CTestUser user;
};

#endif
