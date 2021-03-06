"use strict";
var exec, execSmart;

var child_process = require('child_process');
var cmd = "/usr/local/sbin/smartctl";

var validateArgs = function(device){  
  return  /^[a-z]{2,}[0-9]{1,2}$/.test(device);
}

/*
    R A W  C M D
 */

module.exports.raw = execSmart = function(args) {  
  try{
    var stdout = child_process.execFileSync(cmd , args);
    var lines = stdout.toString().split('\n').slice(0, -1);    
    return lines.slice(4);
  }catch(e){    
    return [];
  }
};


/*
    D E V I C E  I N F O S
 */

module.exports.info = function(device) {
  if(!validateArgs(device)) return {}; //check input parameter

  var lines = execSmart(["-i" ,"/dev/" + device]);
  var deviceInfos, i, len, line, ref;
 
  deviceInfos = {};
  ref = lines.slice(0, -1);
  for (i = 0, len = ref.length; i < len; i++) {
    line = ref[i];
    deviceInfos[line.substring(0, line.search(': ')).trim().replace(/\ +/g, '_').toLowerCase()] = line.substring(1 + line.search(': ')).trim();
  }
  return deviceInfos;  
};


/*
    S M A R T  A T T R S
 */

module.exports.smartAttrs = function(device) {
  if(!validateArgs(device)) return []; //check input parameter

  var lines = execSmart(["-A", "/dev/" + device]);    
  var attr, head, i, infos, len, line;
  
  lines = lines.slice(2, -1);
  head = lines.shift();  
  infos = [];
  for (i = 0, len = lines.length; i < len; i++) {
    line = lines[i];
    attr = line.substring(head.indexOf('ATTRIBUTE_NAME'), head.indexOf('FLAGS')).trim().toLowerCase();
    if (attr === '') {
      continue;
    }
    infos.push({
      attr: attr,
      id: Number(line.substring(0, head.indexOf('ATTRIBUTE_NAME')).trim()),
      flags: line.substring(head.indexOf('FLAGS'), head.indexOf('VALUE')).trim(),
      value: line.substring(head.indexOf('VALUE'), head.indexOf('WORST')).trim(),
      worst: line.substring(head.indexOf('WORST'), head.indexOf('THRESH')).trim(),
      thresh: line.substring(head.indexOf('THRESH'), head.indexOf('FAIL')).trim(),
      fail: line.substring(head.indexOf('FAIL'), head.indexOf('RAW_VALUE')).trim(),
      raw: Number(line.substring(head.indexOf('RAW_VALUE')).trim().split(' ')[0])
    });
  }
  return infos;
};


/*
    S M A R T  H E A L T H
 */

module.exports.health = function(device) {
  if(!validateArgs(device)) return ""; //check input parameter

  var lines = execSmart(["-H", "/dev/" + device]);  
  lines = lines.slice(0, -1);
  if(lines.length==0) return "";
  if (0 === lines[0].search('SMART overall-health self-assessment test result: ')) {
     var status = lines[0].split(' ').pop().toUpperCase();
     return status;
  }
  return "";
};

module.exports.scan = function() {
  var stdout = child_process.execSync(cmd +' --scan-open');
  var devices, i, len, n, ref;
  devices = [];
  ref = stdout.toString().split('\n').slice(0, -1);
  for (i = 0, len = ref.length; i < len; i++) {
    n = ref[i];
    devices.push(n.split(' ')[0]);
  }
  return devices;  
};

