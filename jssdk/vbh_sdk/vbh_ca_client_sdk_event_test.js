const vbhCaSDK = require("./vbh_ca_client_sdk_event");

let cacb = new vbhCaSDK.vbh_CaSdkCallback();

cacb.on('on_vbh_ca_ready', () => {
    console.log('on_vbh_ca_ready');
    vbhCaSDK.vbh_reg_user(111,"123", 3, "userinfo", 8);
    //vbhCaSDK.vbh_unreg_user(111,"123", 3);
});

cacb.on('on_vbh_ca_abnormal',() => {
    console.log('ca_on_abnormal');
});

cacb.on('on_vbh_ca_exit',() => {
    console.log('ca_on_exit');
});

cacb.on('on_vbh_ca_regrsp',(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength) => {
    console.log(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
});

cacb.on('on_vbh_ca_unregrsp',(nReturnCode, nChannelID, UserKey, nUserKeyLen) => {
    console.log(nReturnCode, nChannelID, UserKey.slice(0,nUserKeyLen), nUserKeyLen);
});

vbhCaSDK.vbh_init_ca(1, cacb);
