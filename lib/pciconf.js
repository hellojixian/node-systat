"use strict";

var child_process = require('child_process');
var cmd = "/usr/sbin/pciconf"; 

var validateArgs = function(device){
  return  /^[a-z]{2,}[0-9]{1,2}$/.test(device);
}

module.exports.info = function(name)
{
    if(!validateArgs(name)) return {};
    var result = {};

    try{
        var stdout = child_process.execFileSync(cmd, ["-l","-v",name],{stdio:[0,'pipe','pipe']});
        var lines = stdout.toString().split('\n').slice(1,-1);
         var values;
        for (var i in lines){
            values = lines[i].match(/(\S+)\s+=\s+\'?([^']+)\'?/i);                
            result[values[1]] = values[2];
        }
    }
    catch(e)
    {
        result['vendor'] = "ShadowGrid Inc";
        result['device'] = "System Pesudo Device";
    }
    return result;  
};