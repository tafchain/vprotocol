
-- CREATE DATABASE vbh_order;

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for cps_cfg
-- ----------------------------
DROP TABLE IF EXISTS `cps_cfg`;
CREATE TABLE `cps_cfg`  (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `NODE_ID` int(11) NULL DEFAULT NULL COMMENT '节点ID',
  `CPS_ID` int(11) NULL DEFAULT NULL COMMENT '处理单元的ID，即service-id',
  `CPS_IP_ADDR` varchar(16) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL COMMENT '主机IP地址',
  `CPS_PORT` int(11) NULL DEFAULT NULL COMMENT 'PTS侦听端口号',
  `CH_ID` int(11) NULL DEFAULT NULL COMMENT '所处理的channel ID.',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = 'CPS(channel process service)配置表，一个CPS单元只处理1个channel；' ROW_FORMAT = Dynamic;

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
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin COMMENT = '超级账本层面的系统配置。' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of vbh_sys_cfg
-- ----------------------------
INSERT INTO `vbh_sys_cfg` VALUES ('CMS_IP_ADDR', '127.0.0.1', 2, 0, 'channel-manage-service的地址，全order唯一');
INSERT INTO `vbh_sys_cfg` VALUES ('CMS_PORT', '2000', 2, 0, 'channel-manage-service的端口号');
INSERT INTO `vbh_sys_cfg` VALUES ('MAX_CACHE_BLOCK_COUNT', '10', 0, 0, 'order中可以缓存未发出的区块的最多个数，超过此个数则不能再接收提交的事务');
INSERT INTO `vbh_sys_cfg` VALUES ('MIN_AGREED_PEER_COUNT', '2', 0, 0, '达成共识的最小节点数');
INSERT INTO `vbh_sys_cfg` VALUES ('ORDER_ID', '1', 2, 1, '指定node节点的order-id');
INSERT INTO `vbh_sys_cfg` VALUES ('PACK_MAX_COUNT', '100', 0, 0, '定量打包数，一个区块中最多打包个数');
INSERT INTO `vbh_sys_cfg` VALUES ('PACK_TIMEOUT', '60', 0, 0, '定时打包数，最多间隔多久打包一次');
INSERT INTO `vbh_sys_cfg` VALUES ('PEER_COUNT', '3', 0, 0, '系统中peer的个数');

SET FOREIGN_KEY_CHECKS = 1;
