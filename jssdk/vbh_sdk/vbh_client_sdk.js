const ffi = require("ffi");
const path = require("path");
//const ref = require("ref-struct");
let libPath = path.join(__dirname, "C:\\work\\lib");
process.env.Path = `${process.env.PATH}${path.delimiter}${libPath}`;
let sdkClientLib = ffi.Library('bc_client_c_sdk.dll',{
    'init_bc_c_sdk' : ['int', ['int', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer']],
    'query_bc_user':['int', ['int', 'string', 'int', 'string', 'int']],
    'update_bc_user':['int',['int','int', 'string', 'int', 'string', 'int', 'string', 'int']]
});
const fs = require("fs");
let bckey = fs.readFileSync('C:\\work\\ca\\bc.key', 'utf8');
console.log(bckey);

on_vbh_client_ready = ffi.Callback('void', ['void'], function(){
    console.log("bc_client_c_sdk on_ready");
    sdkClientLib.update_bc_user(139, 111, "123", 3, "bc.wang",7, bckey, bckey.length);
});
on_vbh_client_abnormal = ffi.Callback('void', ['void'], function(){
    console.log("bc_client_c_sdk on_abnormal");
});
on_vbh_client_exit = ffi.Callback('void', ['void'], function(){
    console.log("bc_client_c_sdk on_exit");
});
on_query_user_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen){
    console.log("bc_client_c_sdk on_query_user_rsp: ", nReturnCode, nChannelID, pUserKey, nUserKeyLen, pUserUserInfo, nUserUserInfoLen);
});
on_update_user_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen){
    console.log("bc_client_c_sdk on_update_user_rsp: ", nReturnCode, nChannelID, pUserKey, nUserKeyLen);
    sdkClientLib.query_bc_user(111, "123", 3, bckey, bckey.length);
});
sdkClientLib.init_bc_c_sdk(1,on_vbh_client_ready, on_vbh_client_abnormal, on_vbh_client_exit,on_query_user_rsp,  on_update_user_rsp );
