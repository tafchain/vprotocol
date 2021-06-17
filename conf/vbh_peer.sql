/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 80016
 Source Host           : localhost:3306
 Source Schema         : vbh_peer

 Target Server Type    : MySQL
 Target Server Version : 80016
 File Encoding         : 65001

 Date: 16/09/2019 19:44:41
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for as_cfg
-- ----------------------------
DROP TABLE IF EXISTS `as_cfg`;
CREATE TABLE `as_cfg`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `NODE_ID` int(11) NULL DEFAULT NULL,
  `AS_ID` int(11) NULL DEFAULT NULL,
  `CH_ID` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'anchor service的配置表，强制1个anchor管理1个channel' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for cc_endorse_cfg
-- ----------------------------
DROP TABLE IF EXISTS `cc_endorse_cfg`;
CREATE TABLE `cc_endorse_cfg`  (
  `CC_ID` int(11) NOT NULL,
  `ENDORSE_POLICY` blob NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for cms_addr
-- ----------------------------
DROP TABLE IF EXISTS `cms_addr`;
CREATE TABLE `cms_addr`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `ORD_ID` int(11) NULL DEFAULT NULL COMMENT 'order 节点的ID，在使用raft协议时会用到',
  `IP_ADDR` varchar(16) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `PORT` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'order cms服务的地址配置' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for es_cfg
-- ----------------------------
DROP TABLE IF EXISTS `es_cfg`;
CREATE TABLE `es_cfg`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `NODE_ID` int(11) NULL DEFAULT NULL,
  `ES_ID` int(11) NULL DEFAULT NULL,
  `ES_IP_ADDR` varchar(16) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `ES_PORT` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'endorser service配置表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for t_connect
-- ----------------------------
DROP TABLE IF EXISTS `t_connect`;
CREATE TABLE `t_connect`  (
  `NODE_TYPE` int(11) NOT NULL,
  `NODE_ID` int(11) NOT NULL,
  `REMOTE_IP_ADDR` int(11) NOT NULL,
  `REMOTE_PORT` int(11) NOT NULL,
  `LOCAL_IP_ADDR` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `LOCAL_PORT` int(11) NOT NULL,
  PRIMARY KEY (`NODE_TYPE`, `NODE_ID`, `REMOTE_IP_ADDR`, `REMOTE_PORT`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'DSC库使用的连接关系描述表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for t_dsc_param_config
-- ----------------------------
DROP TABLE IF EXISTS `t_dsc_param_config`;
CREATE TABLE `t_dsc_param_config`  (
  `CONFIG_NAME` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `CONFIG_VALUE` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `NODE_TYPE` int(11) NOT NULL,
  `NODE_ID` int(11) NOT NULL,
  PRIMARY KEY (`CONFIG_NAME`, `NODE_TYPE`, `NODE_ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = '底层DSC组件库的配置' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for t_listen
-- ----------------------------
DROP TABLE IF EXISTS `t_listen`;
CREATE TABLE `t_listen`  (
  `NODE_TYPE` int(11) NOT NULL,
  `NODE_ID` int(11) NOT NULL,
  `IP_ADDR` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `PORT` int(11) NOT NULL,
  PRIMARY KEY (`NODE_TYPE`, `NODE_ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'DSC底层库使用的侦听表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for t_log_config
-- ----------------------------
DROP TABLE IF EXISTS `t_log_config`;
CREATE TABLE `t_log_config`  (
  `NODE_TYPE` int(11) NOT NULL,
  `NODE_ID` int(11) NOT NULL,
  `LOG_TYPE_NAME` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `MAX_FILE_SIZE` int(11) NOT NULL,
  `MAX_BACKUP_INDEX` int(11) NOT NULL,
  `BUFFER_SIZE` int(11) NOT NULL,
  `LOG_GRADE` int(11) NOT NULL,
  `BUFFERED_IO_GRADE` int(11) NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for t_plugin
-- ----------------------------
DROP TABLE IF EXISTS `t_plugin`;
CREATE TABLE `t_plugin`  (
  `NODE_TYPE` int(11) NOT NULL,
  `NODE_ID` int(11) NOT NULL,
  `PLUGIN_NAME` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `PRI` int(11) NOT NULL DEFAULT 0,
  `REMARK` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `INUSE` int(11) NOT NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for vbh_sys_cfg
-- ----------------------------
DROP TABLE IF EXISTS `vbh_sys_cfg`;
CREATE TABLE `vbh_sys_cfg`  (
  `CONFIG_NAME` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `CONFIG_VALUE` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `NODE_TYPE` int(11) NOT NULL,
  `NODE_ID` int(11) NOT NULL,
  `COMMENT` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL COMMENT '配置项的注释',
  PRIMARY KEY (`CONFIG_NAME`, `NODE_TYPE`, `NODE_ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = '超级账本层面的系统配置;USER_GROUP_COUNT的取值，必须是2的幂次' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of vbh_sys_cfg
-- ----------------------------
INSERT INTO `vbh_sys_cfg` VALUES ('AS_BLOCK_CACHE_COUNT', '10', 0, 0, 'anchor service上block-cache的个数');
INSERT INTO `vbh_sys_cfg` VALUES ('HDB_BLOCK_CACHE_NUM', '128', 999, 0, '历史数据库数据block在内存中的cache数');
INSERT INTO `vbh_sys_cfg` VALUES ('HDB_BLOCK_SIZE', '2', 0, 0, '历史数据库块大小，单位MB');
INSERT INTO `vbh_sys_cfg` VALUES ('HDB_MAX_BLOCK_COUNT_IN_FILE', '1024', 999, 0, '历史数据库采用文件存储时，一个文件中最多存放数据块的最多个数');
INSERT INTO `vbh_sys_cfg` VALUES ('MAX_BLOCKCHAIN_FILE_SIZE', '4096', 0, 0, '基于文件系统的区块链文件（存放多个区块）最大size，单位:MB');
INSERT INTO `vbh_sys_cfg` VALUES ('PEER_ID', '1', 0, 0, '当前peer的 PEER ID值');
INSERT INTO `vbh_sys_cfg` VALUES ('PEER_PRIVATE_KEY', 'aaaaaaaaaaaa', 0, 0, '和客户端进行注册等 【无事前分配对称秘钥】通信时，peer端发布时生成的私钥；用于和客户端进行对称秘钥交换');
INSERT INTO `vbh_sys_cfg` VALUES ('PEER_PUBLIC_KEY', 'aaaaaaaaaa', 0, 0, '和客户端进行注册等 【无事前分配对称秘钥】通信时，peer端发布时生成的公钥；用于和客户端进行对称秘钥交换');

-- ----------------------------
-- Table structure for xcs_cfg
-- ----------------------------
DROP TABLE IF EXISTS `xcs_cfg`;
CREATE TABLE `xcs_cfg`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `NODE_ID` int(11) NULL DEFAULT NULL COMMENT 'node id',
  `XCS_ID` int(11) NULL DEFAULT NULL COMMENT 'service id',
  `CH_ID` int(11) NULL DEFAULT NULL COMMENT 'channel id',
  `IP_ADDR` varchar(45) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL COMMENT '作为HTS服务时，侦听的IP地址',
  `PORT` int(11) NULL DEFAULT NULL COMMENT '作为HTS服务，侦听的端口号',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'x committer service 的配置表; x committer = transaction committer + user-committer + information-committer' ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
