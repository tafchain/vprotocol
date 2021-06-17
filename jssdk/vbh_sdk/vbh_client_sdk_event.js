const ffi = require("ffi");
const path = require("path");
//const ref = require("ref-struct");
const EventEmitter = require('events');

let libPath = path.join(__dirname, "C:\\work\\lib");
process.env.Path = `${process.env.PATH}${path.delimiter}${libPath}`;
let sdkClientLib = ffi.Library('bc_client_c_sdk.dll',{
    'init_bc_c_sdk' : ['int', ['int', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer']],
    'query_bc_user':['int', ['int', 'string', 'int', 'string', 'int']],
    'update_bc_user':['int',['int','int', 'string', 'int', 'string', 'int', 'string', 'int']]
});

class vbh_ClientSdkCallback extends EventEmitter {
    SendReadyEventNotice(){
        this.emit('on_client_sdk_ready');
    }
    SendAbnormalEventNotice(){
        this.emit('on_client_sdk_abnormal');
    }
    SendExitEventNotice(){
        this.emit('on_client_sdk_exit');
    }
    SendQueryUserRspEventNotice(nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen){
        this.emit('on_client_sdk_query', nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen);
    }
    SendUpdateUserRspEventNotice(nReturnCode, nChannelID, pUserKey, nUserKeyLen){
        this.emit('on_client_sdk_update',nReturnCode, nChannelID, pUserKey, nUserKeyLen);
    }
}

let clientsdkcb;
on_ready = ffi.Callback('void', ['void'], function(){
    clientsdkcb.SendReadyEventNotice();
});
on_abnormal = ffi.Callback('void', ['void'], function(){
    clientsdkcb.SendAbnormalEventNotice();
});
on_exit = ffi.Callback('void', ['void'], function(){
    clientsdkcb.SendExitEventNotice();
});
on_query_user_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen){
    clientsdkcb.SendQueryUserRspEventNotice(nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen);
});
on_update_user_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen){
    clientsdkcb.SendUpdateUserRspEventNotice(nReturnCode, nChannelID, pUserKey, nUserKeyLen);
});

vbh_init = (nodeID, clientcb) => {
    clientsdkcb = clientcb;
    sdkClientLib.init_bc_c_sdk(nodeID,on_ready, on_abnormal, on_exit,on_query_user_rsp,  on_update_user_rsp );
};

vbh_update_user = (nCCID, nChannelId, userKey, userKeyLength, userInfo, userInfoLength, cryptUserKey, cryptUserKeyLength ) =>{
    sdkClientLib.update_bc_user(nCCID, nChannelId, userKey, userKeyLength, userInfo, userInfoLength, cryptUserKey, cryptUserKeyLength);
};

vbh_query_user = (nChannelId, userKey, userKeyLength, cryptUserKey, cryptUserKeyLength) => {
    sdkClientLib.query_bc_user(nChannelId, userKey, userKeyLength, cryptUserKey, cryptUserKeyLength);
};

module.exports  = {
    vbh_ClientSdkCallback :vbh_ClientSdkCallback,
    vbh_init : vbh_init,
    vbh_update_user : vbh_update_user,
    vbh_query_user : vbh_query_user,
};

