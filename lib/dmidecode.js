"use strict";

var child_process = require('child_process');
var cmd = require('path').dirname(__dirname)+"/bin/dmidecode_freebsd";  

module.exports.info = function(type)
{    
    var stdout = child_process.execFileSync(cmd, ["-t",parseInt(type)]);
    var lines = stdout.toString().split('\n').slice(6,-2);    
    var result = {}; var values;
    for (var i in lines){
        values = lines[i].match(/\s+([^:]+):\s+([^']+)/i);                        
        if(values!=null){
            if(values[2]=='Fill By OEM') continue;
            result[values[1].replace(/\s/g,"")] = values[2];
        }
    }    
    return result;  
};