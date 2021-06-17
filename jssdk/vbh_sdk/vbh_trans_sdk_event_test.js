const vbhTransSDK = require("./vbh_trans_sdk_event");

let transcb = new vbhTransSDK.vbh_TransSdkCallback();

transcb.on('on_vbh_trans_ready',() => {
    console.log('on_vbh_trans_ready');
    vbhTransSDK.vbh_trans_issue(123, 111, "zhangsan", 8, 1000, "123", 3)
});

transcb.on('on_vbh_trans_abnormal',() => {
    console.log('on_vbh_trans_abnormal');
});

transcb.on('on_vbh_trans_exit',() => {
    console.log('on_vbh_trans_exit');
});

transcb.on('on_vbh_trans_query_rsp',(nReturnCode, nChannelID, pUserKey, nUserKeyLen, asset) => {
    console.log('on_vbh_trans_query_rsp', nReturnCode, nChannelID, pUserKey, nUserKeyLen, asset);
    vbhTransSDK.vbh_trans_transfer(123, 111,"zhangsan", 8 , "bc.wang", 7, 900, "1",1);
});

transcb.on('on_vbh_trans_transfer_rsp',(nReturnCode, nChannelID, userKey, userKeyLength) => {
    console.log('on_vbh_trans_transfer_rsp',nReturnCode, nChannelID, userKey, userKeyLength);
});

transcb.on('on_vbh_trans_issue_rsp',(nReturnCode, nChannelID, userKey, userKeyLength) => {
    console.log('on_vbh_trans_issue_rsp',nReturnCode, nChannelID, userKey, userKeyLength);
    vbhTransSDK.vbh_trans_query(111, "zhangsan", 8, "123",3);
});

vbhTransSDK.vbh_trans_sdk_init(1, transcb);