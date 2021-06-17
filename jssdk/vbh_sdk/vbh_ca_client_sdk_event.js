const ffi = require("ffi");
const path = require("path");
const EventEmitter = require('events');

let libPath = path.join(__dirname, "C:\\work\\lib");
process.env.Path = `${process.env.PATH}${path.delimiter}${libPath}`;
let sdkCaLib = ffi.Library('bc_ca_client_c_sdk.dll',{
    'init_bc_ca_c_sdk' : ['int', ['int', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer']],
    'reg_bc_user':['int', ['int', 'string', 'int', 'string', 'int']],
    'unreg_bc_user':['int',['int', 'string', 'int']]
});

class vbh_CaSdkCallback extends EventEmitter {
    SendReadyEventNotice(){
        this.emit('on_vbh_ca_ready');
    }
    SendAbnormalEventNotice(){
        this.emit('on_vbh_ca_abnormal');
    }
    SendExitEventNotice(){
        this.emit('on_vbh_ca_exit');
    }
    SendRegBcRspEventNotice(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength){
        this.emit('on_vbh_ca_regrsp', nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
    }
    SendUnRegBcRspEventNotice(nReturnCode, nChannelID, pUserKey, nUserKeyLen){
        this.emit('on_vbh_ca_unregrsp',nReturnCode, nChannelID, pUserKey, nUserKeyLen);
    }
}

let casdkcb;

on_vbh_ca_ready = ffi.Callback('void', ['void'], function(){
    casdkcb.SendReadyEventNotice();
});
on_vbh_ca_abnormal = ffi.Callback('void', ['void'], function(){
    casdkcb.SendAbnormalEventNotice();
});
on_vbh_ca_exit = ffi.Callback('void', ['void'], function(){
    casdkcb.SendExitEventNotice();
});
on_vbh_ca_reg_bc_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength){
    casdkcb.SendRegBcRspEventNotice(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
});
on_vbh_ca_unreg_bc_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen){
    casdkcb.SendUnRegBcRspEventNotice(nReturnCode, nChannelID, pUserKey, nUserKeyLen);
});

vbh_init_ca = (nodeID,cacb) => {
    casdkcb = cacb;
    sdkCaLib.init_bc_ca_c_sdk(nodeID,on_vbh_ca_ready, on_vbh_ca_abnormal, on_vbh_ca_exit, on_vbh_ca_reg_bc_rsp,  on_vbh_ca_unreg_bc_rsp );
};

vbh_reg_user = (nChannelId, userKey, userKeyLength, userInfo, userInfoLength) =>{
    sdkCaLib.reg_bc_user(nChannelId, userKey, userKeyLength, userInfo, userInfoLength);
};

vbh_unreg_user = (nChannelId, userKey, userKeyLength) => {
    sdkCaLib.unreg_bc_user(nChannelId, userKey, userKeyLength);
};

module.exports.vbh_CaSdkCallback = vbh_CaSdkCallback;
module.exports.vbh_init_ca = vbh_init_ca;
module.exports.vbh_reg_user = vbh_reg_user;
module.exports.vbh_unreg_user = vbh_unreg_user;


