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
-- Records of "T_LISTEN"
-- ----------------------------
delete from "T_LISTEN";
INSERT INTO "T_LISTEN" VALUES (999, 1, '127.0.0.1', 2001);
INSERT INTO "T_LISTEN" VALUES (222, 1, '127.0.0.1', 30000);


-- ----------------------------
-- Records of "T_CONNECT"
-- ----------------------------
delete from "T_CONNECT";
INSERT INTO "T_CONNECT" VALUES (888, 1, '127.0.0.1', 2001, '127.0.0.1', -1);
INSERT INTO "T_CONNECT" VALUES (111, 1, '127.0.0.1', 30000, '127.0.0.1', -1);
INSERT INTO "T_CONNECT" VALUES (111, 11, '127.0.0.1', 30000, '127.0.0.1', -1);


-- ----------------------------
-- Table structure for T_BC_CA_ADDR
-- ----------------------------
DROP TABLE IF EXISTS "T_BC_CA_ADDR";
CREATE TABLE "T_BC_CA_ADDR" (
  "IP_ADDR" varchar(255) DEFAULT NULL,
  "PORT" int(255) DEFAULT NULL,
  "NODE_TYPE" INTEGER NOT NULL,
  "NODE_ID" INTEGER NOT NULL
);

-- ----------------------------
-- Records of "T_BC_CA_ADDR"
-- ----------------------------
delete from "T_BC_CA_ADDR";
INSERT INTO "T_BC_CA_ADDR" VALUES ('127.0.0.1', 5001, 888, 1);

-- ----------------------------
-- Table structure for T_BC_KERNEL_ADDR
-- ----------------------------
DROP TABLE IF EXISTS "T_BC_KERNEL_ADDR";
CREATE TABLE "T_BC_KERNEL_ADDR" (
  "IP_ADDR" varchar(255) DEFAULT NULL,
  "PORT" int(255) DEFAULT NULL,
  "NODE_TYPE" INTEGER NOT NULL,
  "NODE_ID" INTEGER NOT NULL,
  "PEER_ID" INTEGER NOT NULL
);

-- ----------------------------
-- Records of "T_BC_KERNEL_ADDR"
-- ----------------------------
delete from "T_BC_KERNEL_ADDR";
INSERT INTO "T_BC_KERNEL_ADDR" VALUES ('127.0.0.1', 6001, 999, 1, 1);

-- ----------------------------
-- Records of "T_DSC_PARAM_CONFIG"
-- ----------------------------
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('SCHEDULER_NUM', '1', 999, 0); --调度节点的个数
-- committer的节点数 由 T_COMMITTER_PARAM 的行数决定
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('ENDORSER_NUM', '1', 0, 0); --提交背书提案的节点数
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('BLOCK_SIZE', '2', 999, 0); --区块链中，块的大小，单位MB
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('BLOCK_CACHE_NUM', '128', 999, 0);--区块缓存个数，
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('MAX_BLOCK_COUNT_IN_FILE', '1024', 999, 0);--使用文件存储区块链时，一个文件中的区块个数
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('LEVELDB_CACHE_SIZE', '100', 999, 0); --单位：M


-- ----------------------------
-- Table structure for T_COMMITTER_PARAM
-- committer的配置信息表；节点的commiter个数
-- ----------------------------
DROP TABLE IF EXISTS "T_COMMITTER_PARAM";
CREATE TABLE "T_COMMITTER_PARAM" (
"IS_DIRECT_IO" INTEGER NOT NULL,
"IS_RAW_DEVICE" INTEGER NOT NULL,
"RAW_BLOCK_DEVICE_NAME" varchar(255) DEFAULT NULL,
"COMMITTER_ID" INTEGER NOT NULL, -- //committer ID只能从1开始连续分配，个数只能为2的指数幕
"COMMITTER_TYPE" varchar(255) NOT NULL, -- //commiter类型，取值只能是 "user", "transaction"两种（忽略大小写）;类型不同时，ID可以相同
"NODE_TYPE" INTEGER NOT NULL,
"NODE_ID" INTEGER NOT NULL,
CONSTRAINT "sqlite_autoindex_T_COMMITTER_PARAM_1" PRIMARY KEY ("NODE_TYPE", "NODE_ID", "COMMITTER_TYPE", "COMMITTER_ID")
);

-- ----------------------------
-- Records of "T_COMMITTER_PARAM"
-- ----------------------------
-- user-committer 和 transaction-committer的个数可以不一样
-- user-committer 的个数必须是2的倍数
-- transaction-committer 的个数必须是2的倍数
INSERT INTO "T_COMMITTER_PARAM" VALUES (0, 0, '/dev/sdb', 1, 'user', 0, 0);
INSERT INTO "T_COMMITTER_PARAM" VALUES (0, 0, '/dev/sdc', 1, 'transaction', 0, 0);

-- ----------------------------
-- Records of "T_PLUGIN"
-- ----------------------------
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'bc_committer_service', 1, 'bc_committer_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'bc_scheduler_service', 1, 'bc_scheduler_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'bc_anchor_service', 1, 'bc_anchor_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'bc_endorser_service', 1, 'bc_endorser_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'cc_demo_service', 1, 'cc_demo_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'cc_json_demo_service', 1, 'cc_json_demo_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'cc_update_user_json_demo_service', 1, 'cc_update_user_json_demo_service', 1);
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'cc_update_user_demo_service', 1, 'cc_update_user_demo_service', 1);
INSERT INTO "T_PLUGIN" VALUES (888, 0, 'bc_ca_server_service', 1, 'bc_ca_server_service', 1);
INSERT INTO "T_PLUGIN" VALUES (666, 0, 'bc_ca_client_service', 1, 'bc_ca_client_service', 1);
INSERT INTO "T_PLUGIN" VALUES (666, 0, 'bc_client_service', 1, 'bc_client_service', 1);

PRAGMA foreign_keys = true;
