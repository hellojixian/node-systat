"use strict";

var child_process = require('child_process');
var cmd = "/usr/local/sbin/dmidecode";  

module.exports.info = function(type)
{    
    var stdout = child_process.execFileSync(cmd, ["-t",parseInt(type)]);
    var lines = stdout.toString().split('\n').slice(6,-2);    
    var result = {}; var values;
    for (var i in lines){
        values = lines[i].match(/\s+([^:]+):\s+([^']+)/i);                        
        if(values!=null){
            if(values[2]=='Fill By OEM') continue;
            if(values[2]=='Not Provided') continue;
            if(values[2]=='To be filled by O.E.M.') continue;
            if(values[2]=='Unknown') continue;            
            result[values[1].replace(/\s/g,"")] = values[2];
        }
    }    
    return result;  
};

module.exports.memory = function()
{
    var stdout = child_process.execFileSync(cmd, ["-t",17]);
    var devices = stdout.toString().split('\n\n').slice(1,-1);    
    var result = []; var values; 

    for(var i in devices){
        var lines = devices[i].split('\n');
        var device = {};
        for (var l in lines){
            values = lines[l].match(/\s+([^:]+):\s+([^']+)/i);
            if(values!=null){
                if(values[2]=='Not Provided') continue;                
                device[values[1].replace(/\s/g,"")] = values[2];
            }
        }
        if(device.Size!="No Module Installed"){
            result.push({
                size: device.Size,
                speed: device.speed,
                vendor: device.Manufacturer,
                sn: device.SerialNumber,
                type: device.Type,
                width: device.TotalWidth,
                locator: device.Locator
            });        
        }else{
            result.push({
                size: device.Size
            });
        }
    }
    return result;
}
