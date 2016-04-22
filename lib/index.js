"use strict";

const os = require("os");
const smartctl = require("./smartctl");
const systat = require('../build/Release/systat-'+os.type().toLowerCase()+'.node'); 

module.exports = systat;

module.exports.getDisks = function()
{
    if(os.type() != "FreeBSD") return [];
    var disks = smartctl.scan();
    var result = [];
    for(var i in disks){
        var disk = disks[i].substr("/dev/".length); 
        if(disk.substr(0,3)!='ses'){
            result.push(disk);
        }
    }
    return result;    
}


module.exports.getDiskTemperature = function(name){    
    if(os.type() != "FreeBSD") return -1;
    var attrs = smartctl.smartAttrs(name);
    for(var i in attrs){        
        if(parseInt(attrs[i].id)==194) return attrs[i].raw;        
    }
    return -1;
}


module.exports.getDiskHealthTestResult = function(name)
{
    if(os.type() != "FreeBSD") return "NOT SUPPORTED";
    return smartctl.health(name);    
}


module.exports.getDiskInfo = function(name)
{
    if(os.type() != "FreeBSD") return {};
    return smartctl.info(name);
}