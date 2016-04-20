"use strict";
var exec, execSmart;

var execSync = require('child_process').execSync;
var path = require('path');

/*
    R A W  C M D
 */

module.exports.raw = execSmart = function(args) {
  var cmd = path.dirname(__dirname)+"/bin/smartctl_freebsd";  
  var stdout = execSync(cmd+" "+args);  
  var lines = stdout.toString().split('\n').slice(0, -1);    
  return lines.slice(4);
};


/*
    D E V I C E  I N F O S
 */

module.exports.info = function(devicePath, cb) {
  return execSmart("-i " + devicePath, function(e, lines) {
    var deviceInfos, i, len, line, ref;
    if (e != null) {
      return cb(e, lines);
    }
    deviceInfos = {};
    ref = lines.slice(0, -1);
    for (i = 0, len = ref.length; i < len; i++) {
      line = ref[i];
      deviceInfos[line.substring(0, line.search(': ')).trim().replace(/\ +/g, '_').toLowerCase()] = line.substring(1 + line.search(': ')).trim();
    }
    return cb(null, deviceInfos);
  });
};


/*
    S M A R T  A T T R S
 */

module.exports.smartAttrs = function(device, cb) {
  var lines = execSmart("-A -f brief /dev/" + device);  
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

module.exports.health = function(devicePath, cb) {
  return execSmart("-H " + devicePath, function(e, lines) {
    var status;
    if (e != null) {
      return cb(e, lines);
    }
    lines = lines.slice(0, -1);
    if (0 === lines[0].search('SMART overall-health self-assessment test result: ')) {
      status = lines[0].split(' ').pop().toLowerCase();
      return cb(null, status);
    } else {
      return cb(null, lines);
    }
  });
};

module.exports.scan = function(cb) {
  var stdout = execSync('smartctl --scan-open', {maxBuffer: 1024 * 1024 * 24});
  var devices, i, len, n, ref;
  devices = [];
  ref = stdout.toString().split('\n').slice(0, -1);
  for (i = 0, len = ref.length; i < len; i++) {
    n = ref[i];
    devices.push(n.split(' ')[0]);
  }
  return devices;  
};

