const ffi = require("ffi");
const path = require("path");

let libPath = path.join(__dirname, "C:\\work\\lib");
process.env.Path = `${process.env.PATH}${path.delimiter}${libPath}`;
let sdkTransLib = ffi.Library('bc_trans_client_c_sdk.dll',{
    'init_trans_bc_c_sdk' : ['int', ['int', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer']],
    'query_trans_bc_user':['int', ['int', 'string', 'int', 'string', 'int']],
    'trans_bc_user':['int',['int','int', 'string', 'int', 'string', 'int', 'int', 'string', 'int']],
    'issue_bc_user':['int',['int','int', 'string', 'int', 'int', 'string', 'int',]]
});

on_vbh_trans_ready = ffi.Callback('void', ['void'], function(){
    console.log("on_vbh_trans_ready");
});
on_vbh_trans_abnormal = ffi.Callback('void', ['void'], function(){
    console.log("on_vbh_trans_abnormal");
});
on_vbh_trans_exit = ffi.Callback('void', ['void'], function(){
    console.log("on_vbh_trans_exit");
});
on_vbh_trans_query_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, UserKey, nUserKeyLen, userInfo, userInfoLength){
    console.log("on_vbh_trans_query_rsp: ", nReturnCode, nChannelID, UserKey, nUserKeyLen, userInfo, userInfoLength);
});
on_vbh_trans_transfer_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, userKey, userKeyLength){
    console.log("on_vbh_trans_transfer_rsp: ", nReturnCode, nChannelID, userKey, userKeyLength);
});
on_vbh_trans_issue_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, userKey, userKeyLength){
    console.log("on_vbh_trans_issue_rsp: ", nReturnCode, nChannelID, userKey, userKeyLength);
});













