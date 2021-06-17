
-- =======================
-- 配置说明
-- order manager app的NODE_TYPE = 1
-- order service app的NODE_TYPE = 2
-- 仅包含1个order, 
-- 其 order manager app 的 node-id = 1
-- 其 order service app 的 node-id = 2
-- channel x 对应的 cps 的 cps-id 为x, cps 的 端口号为 3000 + x
-- =======================

-- =======================
-- CPS(channel process service)的配置
-- =======================
DELETE FROM `CPS_CFG` WHERE 1=1;
INSERT INTO `CPS_CFG`(`NODE_ID`,`CPS_ID`,`CPS_IP_ADDR`,`CPS_PORT`,`CH_ID`) VALUES (2, 1, '127.0.0.1', 3001, 1);

-- =======================
-- VBH系统基础配置
-- =======================
DELETE FROM `VBH_SYS_CFG` WHERE 1=1;
INSERT INTO `VBH_SYS_CFG` VALUES ('CMS_IP_ADDR', '127.0.0.1', 0, 0, 'channel-manage-service的地址，全order唯一');
INSERT INTO `VBH_SYS_CFG` VALUES ('CMS_PORT', '2000', 0, 0, 'channel-manage-service的端口号');
INSERT INTO `VBH_SYS_CFG` VALUES ('MAX_CACHE_BLOCK_COUNT', '10', 0, 0, 'order中可以缓存未发出的区块的最多个数，超过此个数则不能再接收提交的事务');
INSERT INTO `VBH_SYS_CFG` VALUES ('MIN_AGREED_PEER_COUNT', '2', 0, 0, '达成共识的最小节点数');
INSERT INTO `VBH_SYS_CFG` VALUES ('ORDER_ID', '1', 0, 0, '指定node节点的order-id');
INSERT INTO `VBH_SYS_CFG` VALUES ('PACK_MAX_COUNT', '100', 0, 0, '定量打包数，一个区块中最多打包个数');
INSERT INTO `VBH_SYS_CFG` VALUES ('PACK_TIMEOUT', '60', 0, 0, '定时打包数，最多间隔多久打包一次');
INSERT INTO `VBH_SYS_CFG` VALUES ('PEER_COUNT', '3', 0, 0, '系统中peer的个数');


-- =======================
-- 日志数据库的配置
-- =======================
DELETE FROM `T_LOG_CONFIG` WHERE 1=1;
INSERT INTO `T_LOG_CONFIG` VALUES (0, 0, 'RUN', 10, 10, 1, 4, 5);
INSERT INTO `T_LOG_CONFIG` VALUES (0, 0, 'INTERFACE', 10, 10, 1, 4, 5);
INSERT INTO `T_LOG_CONFIG` VALUES (0, 0, 'STATISTIC', 10, 10, 1, 4, 5);


-- =======================
-- 插件数据库的配置
-- =======================
DELETE FROM `T_PLUGIN` WHERE 1=1;
INSERT INTO `T_PLUGIN` VALUES (1, 0, 'ord_cms', 1, 'ord_cms', 1);
INSERT INTO `T_PLUGIN` VALUES (2, 0, 'ord_cps', 1, 'ord_cps', 1);


-- =======================
-- DSC基础配置
-- =======================
DELETE FROM `T_DSC_PARAM_CONFIG` WHERE 1=1;
INSERT INTO `T_DSC_PARAM_CONFIG` VALUES ('DSC_SERVICE_CONTAINER_NUM', '16', 0, 0);
INSERT INTO `T_DSC_PARAM_CONFIG` VALUES ('LOG_CREATE_LOG_PERIORD_SWITCH', '1', 0, 0);
INSERT INTO `T_DSC_PARAM_CONFIG` VALUES ('LOG_CREATE_LOG_PERIORD', '1', 0, 0);
INSERT INTO `T_DSC_PARAM_CONFIG` VALUES ('LOG_ARCHIVE_SWITCH', '0', 0, 0);
INSERT INTO `T_DSC_PARAM_CONFIG` VALUES ('HEART_BEAT_INTERVAL', '10', 0, 0);
INSERT INTO `T_DSC_PARAM_CONFIG` VALUES ('HEART_BEAT_TIMEOUT_COUNT', '10', 0, 0);


-- =======================

-- =======================
-- =======================

-- =======================
-- =======================

-- =======================
-- =======================

-- =======================

