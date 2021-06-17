const ffi = require("ffi");
const path = require("path");
const EventEmitter = require('events');

let libPath = path.join(__dirname, "C:\\work\\lib");
process.env.Path = `${process.env.PATH}${path.delimiter}${libPath}`;
let sdkTransLib = ffi.Library('bc_trans_client_c_sdk.dll',{
    'init_trans_bc_c_sdk' : ['int', ['int', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer', 'pointer']],
    'query_trans_bc_user':['int', ['int', 'string', 'int', 'string', 'int']],
    'trans_bc_user':['int',['int','int', 'string', 'int', 'string', 'int', 'int', 'string', 'int']],
    'issue_bc_user':['int',['int','int', 'string', 'int', 'int', 'string', 'int',]]
});

class vbh_TransSdkCallback extends EventEmitter {
    SendReadyEventNotice(){
        this.emit('on_vbh_trans_ready');
    }
    SendAbnormalEventNotice(){
        this.emit('on_vbh_trans_abnormal');
    }
    SendExitEventNotice(){
        this.emit('on_vbh_trans_exit');
    }

    SendTransQueryRsp(nReturnCode, nChannelID, pUserKey, nUserKeyLen, asset){
        this.emit('on_vbh_trans_query_rsp', nReturnCode, nChannelID, pUserKey, nUserKeyLen, asset );
    }

    SendTransTransferRsp(nReturnCode, nChannelID, userKey, userKeyLength){
        this.emit('on_vbh_trans_transfer_rsp', nReturnCode, nChannelID, userKey, userKeyLength);
    }

    SendTransIssueRsp(nReturnCode, nChannelID, userKey, userKeyLength){
        this.emit('on_vbh_trans_issue_rsp', nReturnCode, nChannelID, userKey, userKeyLength);
    }
}

let transsdkcb;
on_trans_ready = ffi.Callback('void', ['void'], function(){
    transsdkcb.SendReadyEventNotice();
});
on_trans_abnormal = ffi.Callback('void', ['void'], function(){
    transsdkcb.SendAbnormalEventNotice();
});
on_trans_exit = ffi.Callback('void', ['void'], function(){
    transsdkcb.SendExitEventNotice();
});
on_trans_query_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int', 'int' ], function(nReturnCode, nChannelID, pUserKey, nUserKeyLen, asset){
    transsdkcb.SendTransQueryRsp(nReturnCode, nChannelID, pUserKey, nUserKeyLen, asset);
});
on_trans_transfer_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, userKey, userKeyLen){
    transsdkcb.SendTransTransferRsp(nReturnCode, nChannelID, userKey, userKeyLen);
});

on_trans_issie_rsp = ffi.Callback('void', ['int', 'int', 'string', 'int' ], function(nReturnCode, nChannelID, userKey, userKeyLen) {
    transsdkcb.SendTransIssueRsp(nReturnCode, nChannelID, userKey, userKeyLen);
});

vbh_trans_sdk_init = (nodeID, transcb) => {
    transsdkcb = transcb;
    sdkTransLib.init_trans_bc_c_sdk(nodeID, on_trans_ready, on_trans_abnormal, on_trans_exit, on_trans_query_rsp, on_trans_transfer_rsp, on_trans_issie_rsp);
};

vbh_trans_query = (channelID, userKey, userKeyLen, cryptUserKey,cryptUserKeyLen) =>{
    sdkTransLib.query_trans_bc_user(channelID, userKey, userKeyLen, cryptUserKey,cryptUserKeyLen);
};

vbh_trans_transfer = (ccID, channelID, userKey,userKeyLen, targetUserKey, targetUserKeyLen, quantity, cryptUserKey, cryptUserKeyLen) => {
    sdkTransLib.trans_bc_user(ccID, channelID, userKey,userKeyLen, targetUserKey, targetUserKeyLen, quantity, cryptUserKey, cryptUserKeyLen);
};

vbh_trans_issue = (ccID, channelID,targetUserKey,targetUserKeyLen, quantity, cryptUserKey, cryptUserKeyLen) => {
    sdkTransLib.issue_bc_user(ccID, channelID,targetUserKey,targetUserKeyLen, quantity, cryptUserKey, cryptUserKeyLen);
};

module.exports  = {
    vbh_TransSdkCallback :vbh_TransSdkCallback,
    vbh_trans_sdk_init: vbh_trans_sdk_init,
    vbh_trans_query : vbh_trans_query,
    vbh_trans_transfer : vbh_trans_transfer,
    vbh_trans_issue : vbh_trans_issue,
};
