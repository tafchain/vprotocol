const vbhCaSDK = require("./vbh_ca_client_sdk_event");
const vbhClientSDK = require("./vbh_client_sdk_event");
const vbhtools = require("./vbh_tools");

let cacb = new vbhCaSDK.vbh_CaSdkCallback();
let g_cryptUserKeylist = [];
let nodeId = process.argv.slice(2,3)[0] >>> 0;
let userNum = process.argv.slice(3,4)[0] >>> 0;
console.log(nodeId, userNum);

cacb.on('on_vbh_ca_ready', () => {
    console.log('on_vbh_ca_ready');
    //vbhCaSDK.vbh_reg_user(111,"123", 3, "userinfo", 8);
    for (let i = 0;i<userNum; i++)
        vbhCaSDK.vbh_unreg_user(111,i.toString(), i.toString().length);
});

cacb.on('on_vbh_ca_abnormal',() => {
    console.log('ca_on_abnormal');
});

cacb.on('on_vbh_ca_exit',() => {
    console.log('ca_on_exit');
});

cacb.on('on_vbh_ca_regrsp',(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength) => {
    console.log(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
    g_cryptUserKeylist[UserKey>>> 0] = cryptUserKey;
    vbhClientSDK.vbh_query_user(nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
});

cacb.on('on_vbh_ca_unregrsp',(nReturnCode, nChannelID, UserKey, nUserKeyLen) => {
    console.log(nReturnCode, nChannelID, UserKey.slice(0,nUserKeyLen), nUserKeyLen);
    vbhCaSDK.vbh_reg_user(nChannelID,UserKey, nUserKeyLen, "userinfo", 8);
});

let clientcb = new vbhClientSDK.vbh_ClientSdkCallback();
clientcb.on('on_client_sdk_ready', () => {
    console.log("on_client_sdk_ready");
});

clientcb.on('on_client_sdk_abnormal',() => {
    console.log('on_sdk_abnormal');
});

clientcb.on('on_client_sdk_exit',() => {
    console.log('on_sdk_exit');
});

let numMsg = 0;
clientcb.on('on_client_sdk_query',(nReturnCode, nChannelID, UserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen) => {
    cryptUserKey = g_cryptUserKeylist[UserKey>>> 0];
    vbhClientSDK.vbh_query_user(nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKey.length);
    numMsg++;
});

clientcb.on('on_client_sdk_update',(nReturnCode, nChannelID, pUserKey, nUserKeyLen) => {
    console.log(nReturnCode, nChannelID, pUserKey, nUserKeyLen);
});

vbhCaSDK.vbh_init_ca(nodeId, cacb);
vbhClientSDK.vbh_init(nodeId, clientcb);

function timer() {
    console.log('msgNum = ' + numMsg);
    numMsg = 0;
}
setInterval(timer, 1000);
