const ffi = require("ffi");
const path = require("path");

let libPath = path.join(__dirname, "C:\\work\\lib");
process.env.Path = `${process.env.PATH}${path.delimiter}${libPath}`;
let sdkCaLib = ffi.Library('bc_ca_client_c_sdk.dll',{
    'init_bc_ca_c_sdk' : ['int', ['int', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer']],
    'reg_bc_user':['int', ['int', 'string', 'int', 'string', 'int']],
    'unreg_bc_user':['int',['int', 'string', 'int']]
});

on_vbh_ca_ready = ffi.Callback('void', ['void'], function(){
    console.log("bc_client_c_sdk on_ready");
    sdkCaLib.reg_bc_user(111, "456", 3, "ethon", 5);
});
on_vbh_ca_abnormal = ffi.Callback('void', ['void'], function(){
    console.log("bc_client_c_sdk on_abnormal");
});
on_vbh_ca_exit = ffi.Callback('void', ['void'], function(){
    console.log("bc_client_c_sdk on_exit");
});
on_vbh_ca_regist_user_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength){
    console.log("on_regist_user_rsp: ", nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
    sdkCaLib.unreg_bc_user(111,"456", 3);
});
on_vbh_ca_unregist_user_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen){
    console.log("on_unregist_user_rsp: ", nReturnCode, nChannelID, pUserKey, nUserKeyLen);
});

sdkCaLib.init_bc_ca_c_sdk(1,on_vbh_ca_ready, on_vbh_ca_abnormal, on_vbh_ca_exit,on_vbh_ca_regist_user_rsp,  on_vbh_ca_unregist_user_rsp );

