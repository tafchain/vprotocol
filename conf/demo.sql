/*
 Navicat Premium Data Transfer

 Source Server         : dsc
 Source Server Type    : SQLite
 Source Server Version : 3017000
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3017000
 File Encoding         : 65001

 Date: 30/03/2019 11:22:11
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Records of "T_LISTEN"
-- ----------------------------
--ƽ̨ demo server
INSERT INTO "T_LISTEN" VALUES (986, 1, '127.0.0.1', 1986);

-- ----------------------------
-- Records of "T_CONNECT"
-- ----------------------------
--ƽ̨ demo client
INSERT INTO "T_CONNECT" VALUES (985, 1, '127.0.0.1', 1986, '127.0.0.1', -1);

-- ----------------------------
-- Records of "T_PLUGIN"
-- ----------------------------
INSERT INTO "T_PLUGIN" VALUES (986, 0, 'demo_server_service', 1, 'demo_server_service', 1);
-- INSERT INTO "T_PLUGIN" VALUES (986, 0, 'demo_server_json_service', 1, 'demo_server_json_service', 0);
-- INSERT INTO "T_PLUGIN" VALUES (986, 0, 'demo_mcp_server_service', 1, 'demo_mcp_server_service', 0);
-- INSERT INTO "T_PLUGIN" VALUES (986, 0, 'demo_db_service', 1, 'demo_db_service', 0);
INSERT INTO "T_PLUGIN" VALUES (985, 0, 'demo_client_static_service', 1, 'demo_client_static_service', 1);
INSERT INTO "T_PLUGIN" VALUES (985, 0, 'demo_client_service', 2, 'demo_client_service', 1);
-- INSERT INTO "T_PLUGIN" VALUES (985, 0, 'demo_client_json_service', 1, 'demo_client_json_service', 0);
-- INSERT INTO "T_PLUGIN" VALUES (985, 0, 'demo_mcp_client_service', 1, 'demo_mcp_client_service', 0);

-- ----------------------------
-- Table structure for T_USER
-- ----------------------------
-- DROP TABLE IF EXISTS "T_USER";
-- CREATE TABLE "T_USER" (
--   "user_id" INTEGER NOT NULL,
--   "user_name" TEXT NOT NULL,
--   "user_date" TEXT NOT NULL,
--   PRIMARY KEY ("user_id")
-- );

-- ----------------------------
-- Records of "T_USER"
-- ----------------------------
-- INSERT INTO "T_USER" VALUES (123, 'fw456', '2019-02-09 06:26:41');

PRAGMA foreign_keys = true;
