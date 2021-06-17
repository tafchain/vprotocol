/*
 Navicat Premium Data Transfer

 Source Server         : dsc
 Source Server Type    : SQLite
 Source Server Version : 3017000
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3017000
 File Encoding         : 65001

 Date: 30/03/2019 14:47:32
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Records of "T_LISTEN"
-- ----------------------------
delete from "T_LISTEN";
INSERT INTO "T_LISTEN" VALUES (999, 1, '127.0.0.1', 24978);

-- ----------------------------
-- Records of "T_CONNECT"
-- ----------------------------
delete from "T_CONNECT";
INSERT INTO "T_CONNECT" VALUES (666, 1, '127.0.0.1', 24978, '127.0.0.1', -1);

-- ----------------------------
-- Records of "T_DSC_PARAM_CONFIG"
-- ----------------------------
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('MQ_SCHEDULER_SERVICE_IP_ADDR', '127.0.0.1', 0, 0);
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('MQ_SCHEDULER_SERVICE_PORT', 14978, 0, 0);
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('MQ_FORWARDER_IP_ADDR', '127.0.0.1', 666, 1);
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('MQ_FORWARDER_PORT', 4978, 666, 0);
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('TCP_NODELAY_FLAG', 0, 0, 0);
INSERT INTO "T_DSC_PARAM_CONFIG" VALUES ('SOCKET_BUF', 128, 0, 0);

-- ----------------------------
-- Records of "T_PLUGIN"
-- ----------------------------
delete from "T_PLUGIN";
INSERT INTO "T_PLUGIN" VALUES (999, 0, 'mq_scheduler_service', 1, 'mq_scheduler_service', 1);
INSERT INTO "T_PLUGIN" VALUES (666, 0, 'mq_forwarder_service', 1, 'mq_forwarder_service', 1);

PRAGMA foreign_keys = true;
