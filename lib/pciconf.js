"use strict";

var child_process = require('child_process');
var cmd = "/usr/sbin/pciconf"; 

var validateArgs = function(device){
  return  /^[a-z]{2,}[0-9]{1,2}$/.test(device);
}

module.exports.info = function(name)
{
    if(!validateArgs(name)) return {};

    var stdout = child_process.execFileSync(cmd, ["-l","-v",name]);
    var lines = stdout.toString().split('\n').slice(1,-1);
    var result = {}; var values;
    for (var i in lines){
        values = lines[i].match(/(\S+)\s+=\s+\'?([^']+)\'?/i);                
        result[values[1]] = values[2];
    }
    return result;  
};