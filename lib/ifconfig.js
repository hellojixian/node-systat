"use strict";

var child_process = require('child_process');
var cmd = "/sbin/ifconfig";  

module.exports.info = function(name)
{        
    var stdout = child_process.execFileSync(cmd, [name]);
    var lines = stdout.toString().split('\n').slice(2,-1);
    var result = {}; var values;
    for(var i in lines){
        values = lines[i].match(/\s+ether\s(.*)/i);            
        if(values!=null)
        {
            result.mac = values[1];
            break;
        }     
    }
    return result; 
};