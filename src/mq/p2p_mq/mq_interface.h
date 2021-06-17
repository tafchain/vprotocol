#ifndef MQ_COMM_DEF_H_23487923487923416823167234872437
#define MQ_COMM_DEF_H_23487923487923416823167234872437

#include <list>

#include "mq/p2p_mq/mq_export.h"
#include "mq/p2p_mq/mq_if_def.h"

P2P_MQ_EXPORT int InitP2pMqMsgSdk(CInitMsgParam& rParam);
P2P_MQ_EXPORT int ExitP2pMqMsgSdk(void);

P2P_MQ_EXPORT int SubP2pMqMsgID(std::list<int>& lstTopic);
P2P_MQ_EXPORT int PubBroadcastP2pMqMsg(CP2pMqMsg& rMsg);
P2P_MQ_EXPORT int PubPollP2pMqMsg(CP2pMqMsg& rMsg);
P2P_MQ_EXPORT int PubP2pMqAddrMsg(CP2pMqAddrMsg& rMsg);

#endif
