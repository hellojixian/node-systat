"use strict";

const os = require("os");
const smartctl = require("./smartctl");
const pciconf = require("./pciconf");
const ifconfig = require("./ifconfig");
const dmidecode = require("./dmidecode");
const systat;
if(process.hasOwnProperty('binding')){
    systat = process.binding('systat')
}else{
    systat = require('../build/Release/systat-'+os.type().toLowerCase()+'.node'); 
}
//exports all native methods
for(var i in systat)
{
    module.exports[i] = systat[i];    
}

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

module.exports.getDisksInfo = function(name)
{
    if(os.type() != "FreeBSD") return {};
    
    var disks = module.exports.getDisks();
    var result={};
    for (var i in disks){
        var disk = module.exports.getDiskInfo(disks[i]);
        result[disks[i]]={
            family: disk.model_family,
            model:  disk.device_model,            
            capacity: disk.user_capacity,
            firmware: disk.firmware_version,
            sn:     disk.serial_number
        };
    }
    return result;
}


module.exports.getNetworkInterfaceInfo = function(name)
{
    if(os.type() != "FreeBSD") return {};
    var info=pciconf.info(name);    
    var ifcfg=ifconfig.info(name);
    return {
        vendor: info.vendor,
        model:  info.device,
        mac:    ifcfg.mac
    };
}

module.exports.getNetworkInterfacesInfo = function()
{
    var ifs = Object.keys(module.exports.getNetworkInterfaceStatus());    
    var result = {};
    for(var i in ifs){    
        if(/^lo/.test(ifs[i])) continue;
        // if(/^bridge/.test(ifs[i])) continue;
        result[ifs[i]] = module.exports.getNetworkInterfaceInfo(ifs[i]);
    }
    return result;
}

module.exports.getCPUInfo = function()
{
    if(os.type() != "FreeBSD") return {};
    var type = 4; //4 means CPU 
    var info=dmidecode.info(type);
    
    return {
        id:         info.ID,
        family:     info.Family,        
        signature:  info.Signature,
        model:      info.Version,
        voltage:    info.Voltage,
        core:       info.CoreCount,
        speed:      info.CurrentSpeed,
        socket:     info.Upgrade
    };
}

module.exports.getMemoryInfo = function()
{
    if(os.type() != "FreeBSD") return {};    
    return dmidecode.memory();;
}


module.exports.getMainboardInfo = function()
{
    if(os.type() != "FreeBSD") return {};
    
    var baseboard = dmidecode.info(2); //baseboard
    var bios = dmidecode.info(0);
    return {
        bios: {
            vendor: bios.Vendor,
            version: bios.Version,
            romSize: bios.ROMSize,
            runtimeSize: bios.RuntimeSize,
            revision: bios.BIOSRevision
        },
        vendor: baseboard.Manufacturer,
        model: baseboard.ProductName,
    };
}