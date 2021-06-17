const validator = require("validator");
const vbhCaSDK = require("./vbh_ca_client_sdk_event");
const vbhClientSDK = require("./vbh_client_sdk_event");
const vbhTransSDK = require("./vbh_trans_sdk_event");
const vbhtools = require("./vbh_tools");

let http = require('http').createServer(handler),
    io = require('socket.io').listen(http),
    fs = require('fs');

http.listen(8080);
io.set('log level', 1);//将socket.io中的debug信息关闭

let sock;

function handler (req, res) {
    fs.readFile(__dirname + '/index.html',function (err, data) {
        if (err) {
            res.writeHead(500);
            return res.end('Error loading index.html');
        }
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(data);
    });
}

// 空投一定的币
// 参数: {
//              "toUser":"usera",
//              "asset":100,
//              "cryptUserKey":"12312313123"
//       }
function handle_trans_issue(data) {
    if (data.hasOwnProperty("toUser")&& data.hasOwnProperty("asset")&& data.hasOwnProperty("cryptUserKey")) {
        vbhTransSDK.vbh_trans_issue(123, 111, data.toUser, data.toUser.length, data.asset, data.cryptUserKey, data.cryptUserKey.length);
    }
}

// 转账
// 参数: {
//          "fromUserKey":"from",
//          "toUserKey":"to",
//          "asset":100,
//          "cryptUserKey":"12312313123"
//       }
function handle_trans_transfer(data) {
    if (data.hasOwnProperty("fromUserKey") && data.hasOwnProperty("toUserKey")&&data.hasOwnProperty("asset"), data.hasOwnProperty("cryptUserKey")) {
        vbhTransSDK.vbh_trans_transfer(123, 111,data.fromUserKey, data.fromUserKey.length
            , data.toUserKey, data.toUserKey.length, data.asset, data.cryptUserKey,data.cryptUserKey.length);
    }
}

// 查询余额
// 参数: {
//              "userKey":""key,
//              "cryptUserKey":"11111"
// }
function handle_trans_query(data) {
    if (data.hasOwnProperty("userKey") && data.hasOwnProperty("cryptUserKey")) {
        vbhTransSDK.vbh_trans_query(111, data.userKey,data.userKey.length, data.cryptUserKey, data.cryptUserKey.length);
    }
}

//注册用户
//指令: register
//例子:  {
//           "channelID": 111,
//           "userKey":"key",
//           "userInfo":"ppppppppppppp"
//         }
function handle_register(data){
    console.log(data);
    if (data.hasOwnProperty("channelID") && data.hasOwnProperty("userKey") && data.hasOwnProperty("userInfo")){
        const base64UserInfo = vbhtools.base64_encode(data.userInfo);
        vbhCaSDK.vbh_reg_user(data.channelID, data.userKey, data.userKey.length, base64UserInfo, base64UserInfo.length);
    }
}

//注销用户
//指令: unregister
//例子:  {
//           "channelID": 111,
//           "userKey":"key"
//         }
function handle_unregister(data){
    console.log(data);
    if (data.hasOwnProperty("channelID") && data.hasOwnProperty("userKey")) {
        vbhCaSDK.vbh_unreg_user(data.channelID,data.userKey, data.userKey.length);
    }
}

// 查询用户信息
//例子: {
//           "channelID": 111,
//           "userKey":"key",
//           "cryptUserKey": "11111111111111"
//         }
function handle_query(data){
    console.log(data);
    if (data.hasOwnProperty("channelID") && data.hasOwnProperty("userKey") && data.hasOwnProperty("cryptUserKey")) {
        vbhClientSDK.vbh_query_user(data.channelID, data.userKey, data.userKey.length, data.cryptUserKey, data.cryptUserKey.length );
    }
}

//更新用户信息
//例子: {
//           "channelID": 111,
//           "userKey":"key",
//           "cryptUserKey": "11111111111111",
//           "userInfo":"yyyyyyyyyyyyyyyyyyy"
//          }
function handle_update(data){
    console.log(data);
    if (data.hasOwnProperty("channelID") && data.hasOwnProperty("userKey") && data.hasOwnProperty("userInfo")&& data.hasOwnProperty("cryptUserKey")) {
        //vbhClientSDK.vbh_update_user(139,data.channelID, data.userKey, data.userKey.length, data.userInfo, vbhtools.getBytesLength(data.userInfo), data.cryptUserKey, data.cryptUserKey.length )
        const base64UserInfo = vbhtools.base64_encode(data.userInfo);
        vbhClientSDK.vbh_update_user(139,data.channelID, data.userKey, data.userKey.length, base64UserInfo, base64UserInfo.length, data.cryptUserKey, data.cryptUserKey.length )
    }
}

io.sockets.on('connection', function (socket) {
    sock = socket;
    socket.emit('news', { hello: 'helloworld' });
    socket.on('register', function (data) {
        console.log(data.data);
        if (validator.isJSON(data.data)) {
            handle_register(JSON.parse(data.data));
        }
    });
    socket.on('unregister', function (data) {
        if (validator.isJSON(data.data)) {
            handle_unregister(JSON.parse(data.data));
        }
    });
    socket.on('query', function (data) {
        if (validator.isJSON(data.data)) {
            handle_query(JSON.parse(data.data));
        }
    });
    socket.on('update', function (data) {
        if (validator.isJSON(data.data)) {
            handle_update(JSON.parse(data.data));
        }
    });
    socket.on('trans_query', function (data) {
        if (validator.isJSON(data.data)) {
            console.log(data.data);
            handle_trans_query(JSON.parse(data.data));
        }
    });
    socket.on('trans_transfer', function (data) {
        if (validator.isJSON(data.data)) {
            handle_trans_transfer(JSON.parse(data.data));
        }
    });
    socket.on('trans_issue', function (data) {
        if (validator.isJSON(data.data)) {
            handle_trans_issue(JSON.parse(data.data));
        }
    })
});

let cacb = new vbhCaSDK.vbh_CaSdkCallback();
let nodeId = process.argv.slice(2,3)[0] >>> 0;

cacb.on('on_vbh_ca_ready', () => {
    console.log('on_vbh_ca_ready');
});

cacb.on('on_vbh_ca_abnormal',() => {
    console.log('ca_on_abnormal');
});

cacb.on('on_vbh_ca_exit',() => {
    console.log('ca_on_exit');
});

cacb.on('on_vbh_ca_regrsp',(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength) => {
    console.log(nReturnCode, nChannelID, UserKey, nUserKeyLen, cryptUserKey, cryptUserKeyLength);
    if (nReturnCode) {
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }else{
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
            "UserKey": UserKey.slice(0, nUserKeyLen),
            "cryptUserKey": cryptUserKey.slice(0, cryptUserKeyLength)
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }
});

cacb.on('on_vbh_ca_unregrsp',(nReturnCode, nChannelID, UserKey, nUserKeyLen) => {
    console.log(nReturnCode, nChannelID, UserKey.slice(0,nUserKeyLen), nUserKeyLen);
    if (nReturnCode){
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }else{
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
            "UserKey": UserKey.slice(0, nUserKeyLen)
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }
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

clientcb.on('on_client_sdk_query',(nReturnCode, nChannelID, UserKey, nUserKeyLen, userInfo, userInfoLen) => {
    console.log(nReturnCode, nChannelID, UserKey, nUserKeyLen, userInfo, userInfoLen);
    if (nReturnCode){
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
        };
        sock.emit('result', { data: JSON.stringify(result) })
    } else{
        const decodeUserInfo = vbhtools.base64_decode(userInfo);
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
            "UserKey": UserKey.slice(0, nUserKeyLen),
            "UserInfo": decodeUserInfo
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }
});

clientcb.on('on_client_sdk_update',(nReturnCode, nChannelID, UserKey, nUserKeyLen) => {
    console.log(nReturnCode, nChannelID, UserKey, nUserKeyLen);
    let result = {
        "ReturnCode": nReturnCode,
        "ChannelID": nChannelID,
        "UserKey": UserKey.slice(0, nUserKeyLen)
    };
    sock.emit('result', { data: JSON.stringify(result) })
});

let transcb = new vbhTransSDK.vbh_TransSdkCallback();

transcb.on('on_vbh_trans_ready',() => {
    console.log('on_vbh_trans_ready');
});

transcb.on('on_vbh_trans_abnormal',() => {
    console.log('on_vbh_trans_abnormal');
});

transcb.on('on_vbh_trans_exit',() => {
    console.log('on_vbh_trans_exit');
});

transcb.on('on_vbh_trans_query_rsp',(nReturnCode, nChannelID, userKey, userKeyLen, asset) => {
    console.log('on_vbh_trans_query_rsp', nReturnCode, nChannelID, userKey, userKeyLen, asset);
    if (nReturnCode){
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
        };
        sock.emit('result', { data: JSON.stringify(result) })
    } else{
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
            "UserKey": userKey.slice(0, userKeyLen),
            "asset": asset
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }
});

transcb.on('on_vbh_trans_transfer_rsp',(nReturnCode, nChannelID, userKey, userKeyLength) => {
    console.log('on_vbh_trans_transfer_rsp',nReturnCode, nChannelID, userKey, userKeyLength);
    if (nReturnCode){
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
        };
        sock.emit('result', { data: JSON.stringify(result) })
    } else{
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
            "UserKey": userKey.slice(0, userKeyLength),
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }
});

transcb.on('on_vbh_trans_issue_rsp',(nReturnCode, nChannelID, userKey, userKeyLength) => {
    console.log('on_vbh_trans_issue_rsp',nReturnCode, nChannelID, userKey, userKeyLength);
    if (nReturnCode){
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
        };
        sock.emit('result', { data: JSON.stringify(result) })
    } else{
        let result = {
            "ReturnCode": nReturnCode,
            "ChannelID": nChannelID,
            "UserKey": userKey.slice(0, userKeyLength),
        };
        sock.emit('result', { data: JSON.stringify(result) })
    }
});

vbhCaSDK.vbh_init_ca(nodeId, cacb);
vbhClientSDK.vbh_init(nodeId, clientcb);
vbhTransSDK.vbh_trans_sdk_init(nodeId, transcb);
