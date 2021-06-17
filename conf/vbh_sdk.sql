/*
 Navicat Premium Data Transfer

 Source Server         : dsc
 Source Server Type    : SQLite
 Source Server Version : 3017000
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3017000
 File Encoding         : 65001

 Date: 26/12/2018 13:30:40
*/

-- 插入数据时，NODETYPE取0, 表示任意 NodeType
-- 插入数据时，NODEID取0，表示任意 NODEID

-- 端口号的分段规则：
-- T_LISTEN 表：
-- kenel: 2001 - 5000
-- 测试工具(222)： 固定值 30000
-- --------------------
-- T_BC_CA_ADDR 客户端访问用
-- ca:    5001 - 6000

-- T_BC_KERNEL_ADDR 客户端和kernel之间的连接
-- kernel: 6001-10000


PRAGMA foreign_keys = false;


-- ----------------------------
-- Table structure for CC_ENDORSE_CFG
-- CC背书策略表
-- ----------------------------
DROP TABLE IF EXISTS "CC_ENDORSE_CFG";
CREATE TABLE "CC_ENDORSE_CFG" (
  "CC_ID" INTEGER NOT NULL, -- CC ID
  "ENDORSE_POLICY" BLOB NOT NULL -- 背书策略，为V编码后的数据
);


-- ----------------------------
-- Table structure for ES_CFG
-- endorser service 的地址列表
-- ----------------------------
DROP TABLE IF EXISTS "ES_CFG";
CREATE TABLE "ES_CFG" (
  "PEER_ID" INTEGER NOT NULL,
  "ES_IP_ADDR" INTEGER NOT NULL,
  "ES_PORT" INTEGER NOT NULL
);

-- ----------------------------
-- Table structure for VBH_SYS_CFG
-- VBH SDK的系统配置
-- ----------------------------
DROP TABLE IF EXISTS "VBH_SYS_CFG";
CREATE TABLE "VBH_SYS_CFG" (
  "CONFIG_NAME" VARCHAR(128) NOT NULL,
  "CONFIG_VALUE" VARCHAR(256),
  "NODE_TYPE" INTEGER NOT NULL,
  "NODE_ID" INTEGER NOT NULL,
  PRIMARY KEY ("CONFIG_NAME", "NODE_TYPE", "NODE_ID")
);

-- ----------------------------
-- Records of "VBH_SYS_CFG"
-- ----------------------------
INSERT INTO "VBH_SYS_CFG" VALUES ('ES_CFG_VERSION', '1', 0, 0); -- endorser service 地址列表的版本号，用于从服务器端同步 endorser service 地址列表
INSERT INTO "VBH_SYS_CFG" VALUES ('CC_ENDORSE_CFG_VERSION', '1', 0, 0); -- CC 背书策略的版本号， 用于从服务器端同步 CC背书策略列表
INSERT INTO "VBH_SYS_CFG" VALUES ('PEER_PUBLIC_KEY', '030FDA589111DB1DA5A289598325B587913DB86B3849A03A4F56FE4F91FD9BC6DE', 0, 0); -- peer endorser通信通道的公钥
INSERT INTO "VBH_SYS_CFG" VALUES ('PEER_ENVELOPE_KEY', 'aaaaaaaaaa', 0, 0); -- SDK 和 endorser通信时使用的对称秘钥；TODO: 这个为临时增加的变量，待将来sdk和endorser的秘钥交换通道打通后就会扔掉
INSERT INTO "VBH_SYS_CFG" VALUES ('ENDORSE_PEER_COUNT', '1', 0, 0); --提交背书提案的节点数, 暂时先用最简单的背书策略：所有的背书策略都一样，随机选择背书节点；后期版本要调整背书策略，把背书策略数据库使用起来


-- ----------------------------
-- Records of "T_PLUGIN"
-- ----------------------------
INSERT INTO "T_PLUGIN" VALUES (0, 0, 'sdk_client_service', 1, 'sdk_client_service', 1);


PRAGMA foreign_keys = true;
