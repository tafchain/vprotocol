#ifndef MQ_CLIENT_SDK_H_23487923487923416823167234872437
#define MQ_CLIENT_SDK_H_23487923487923416823167234872437

#ifdef __cplusplus 
extern "C" {
#endif

#include "mq/mq_client_sdk/mq_client_sdk_export.h"

	typedef void(*on_ready) (void);
	typedef void(*on_abnormal) (void);
	typedef void(*on_exit) (void);	
	typedef void(*on_mq_msg) (const unsigned int nClientID, const unsigned int nTopicID, const char* pMsg, unsigned int nMsgSize);

	MQ_CLIENT_SDK_EXPORT int InitMqClientSdk(const unsigned int nClientID, unsigned int* pTopicID, const unsigned int nTopicIDNum,
		on_ready pOnReady, on_abnormal pOnAbnormal, on_exit pOnExit, on_mq_msg pOnMqMsg);
	MQ_CLIENT_SDK_EXPORT int ExitMqClientSdk(void);

	MQ_CLIENT_SDK_EXPORT int PubTopicPollMqMsg(const unsigned int nTopicID, const char* pMsg, const unsigned int nMsgSize);//发topic msg，多个app订阅同一topic时，轮询发送，达负载均衡效果
	MQ_CLIENT_SDK_EXPORT int PubTopicBroadcastMqMsg(const unsigned int nTopicID, const char* pMsg, const unsigned int nMsgSize);//发topic msg，多个app订阅同一topic时，轮询发送，达负载均衡效果
	MQ_CLIENT_SDK_EXPORT int PubAddrMqMsg(const unsigned int nClientID, const unsigned int nTopicID, const char* pMsg, const unsigned int nMsgSize);//定点派发

#ifdef __cplusplus 
}
#endif

#endif
