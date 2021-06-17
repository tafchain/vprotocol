#ifndef VBH_COMM_ID_NAME_RULE_H_F43DF08A866D11E9B17860F18A3A20D1
#define VBH_COMM_ID_NAME_RULE_H_F43DF08A866D11E9B17860F18A3A20D1

/** 流程名缩写
 *
 * 注册用户流程： RegUsr
 * 注销用户流程： UnregUsr
 * 发起交易流程： Propose
 * 查询用户流程： GetUsr
 * 查询交易流程： GetTrans
 */

/** 实体简写
 * 
 * 客户端API:		Api
 * 客户端服务:		Clt
 * Ca:				Ca
 * Endorser Service:		Es
 * Order PTS:		Pts
 * Order CBS:		Cbs
 * ChainCode:		Cc
 * Anchor Service:			AS
 * X Committer Service: Xcs
 */

/** 消息类名 命名规则
 * C 消息描述 [流程名] [源实体 目的实体] Req/Rsp
 *
 * 例外：通用消息消息类名 命名规则（超过2个地方使用的同一个消息）
 * C 消息描述 Req/Rsp
 */

/** 消息ID 命名规则
 * EN_源实体_目的实体_消息描述_REQ/RSP
 * 
 * 例外：通用消息消息ID 命名规则 
 * EN_消息描述_REQ/RSP
 */

#endif
