
const getBytesLength = (param) => param.replace(/[^\x00-\xff]/g, 'xx').length;
const base64_encode = (param) => Buffer.from(param).toString('base64');
const base64_decode = (param) => Buffer.from(param, 'base64').toString('utf-8');

module.exports  = {
    getBytesLength,
    base64_encode,
    base64_decode
};