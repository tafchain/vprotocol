const vbhClientSDK = require("./vbh_client_sdk_event");
const fs = require("fs");

let cryptUserKey = fs.readFileSync('C:\\work\\ca\\bc.key', 'utf8');
console.log(cryptUserKey);

let clientcb = new vbhClientSDK.vbh_ClientSdkCallback();
clientcb.on('on_client_sdk_ready', () => {
   setImmediate(() =>{
       //for(let i=0; i< 2; i++){
        vbhClientSDK.vbh_query_user(111, "123", 3, cryptUserKey, cryptUserKey.length );
      // }
   });
});

clientcb.on('on_client_sdk_abnormal',() => {
    console.log('on_sdk_abnormal');
});

clientcb.on('on_client_sdk_exit',() => {
    console.log('on_sdk_exit');
});

let numMsg = 0;
clientcb.on('on_client_sdk_query',(nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen) => {
    setImmediate(() =>{
        vbhClientSDK.vbh_query_user(nChannelID, pUserKey, nUserKeyLen, cryptUserKey, cryptUserKey.length);
    });
    numMsg++;
});

clientcb.on('on_client_sdk_update',(nReturnCode, nChannelID, pUserKey, nUserKeyLen) => {
    console.log(nReturnCode, nChannelID, pUserKey, nUserKeyLen);
});

vbhClientSDK.vbh_init(1, clientcb);

function timer() {
    console.log('msgNum = ' + numMsg);
    numMsg = 0;
}
setInterval(timer, 1000);
