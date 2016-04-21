
"use strict";

var chai = require('chai');
var expect = chai.expect;
var systat = require('../');

describe('systat', function () {
    

    describe('Mainboard Features', function () {

        describe('getFanSpeeds()', function () {
            it('shoudl return the system fans speed(rpm) as array', function (done) {
                var result = systat.getFanSpeeds();            
                expect(result).to.be.an.array;
                done();
            });        
        });

        describe('getSystemTemperatures()', function () {
            it('shoudl return the system temperature(C) as array', function (done) {
                var result = systat.getSystemTemperatures();
                expect(result).to.be.an.array;        
                done();
            });        
        });

        describe('getCPUTTemperatures()', function () {
            it('shoudl return the CPU temperature(C) as array', function (done) {
                var result = systat.getCPUTemperatures();
                expect(result).to.be.an.array;            
                done();
            });        
        });

        describe('getVoltages()', function () {
            it('shoudl return the system voltages(mV) as array', function (done) {
                var result = systat.getVoltages();
                expect(result).to.be.an.array;
                done();
            });        
        });

    });


    describe('Disk Features', function () {        
    
        describe('getDisks()', function () {
            it('should return the disk name as array', function (done) {            
                var result = systat.getDisks();              
                expect(result).to.be.an.array;
                // console.log(result);
                done();
            });
        });    


        describe('getDiskPartitions()', function () {
            it('should return mounted disk partition info as array', function (done) {
                var result = systat.getDiskPartitions();
                expect(result).to.be.an.array;
                // console.log(result);
                done();
            });
        });

        describe('getDiskUsage(root)', function () {
            it('should return the partition usage info as object', function (done) {
                var result = systat.getDiskUsage("/");
                expect(result).to.be.an.object;
                // console.log(result);
                done();
            });

            it('should return object with all zero filled if pass a non exists path', function (done) {
               var result = systat.getDiskUsage("nonexist0");
                expect(result).to.be.an.object;
                expect(result.total).to.equal(0);
                expect(result.used).to.equal(0);
                expect(result.free).to.equal(0);
                done(); 
            });

            it('should return object with all zero filled if pass a bad path', function (done) {
               var result = systat.getDiskUsage(";ls -l;");
                expect(result).to.be.an.object;
                expect(result.total).to.equal(0);
                expect(result.used).to.equal(0);
                expect(result.free).to.equal(0);
                done(); 
            });            
        });


        describe('getDiskIOStat()', function () {
            it('shoudl return the disk I/O statistics info as object', function (done) {
                var result = systat.getDiskIOStat();              
                expect(result).to.be.an.object;
                // console.log(result);
                done();
            });      
        });    

        describe('getDiskTemperatures(device)', function () {
            it('should return the disk temperature(c) as int', function (done) {            
                var result = systat.getDiskTemperature(systat.getDisks()[0]);              
                expect(result).to.be.an.int;
                // console.log(result);
                done();
            });

            it('should return -1 object if give a non exist device name', function (done) {
               var result = systat.getDiskTemperature("nonexist0");
                expect(result).to.be.an.int;
                expect(result).to.equal(-1);
                // console.log(result);
                done(); 
            });

            it('should return -1 object if device name is in bad format', function (done) {
               var result = systat.getDiskTemperature(";ls -l;");
                expect(result).to.be.an.int;
                expect(result).to.equal(-1);       
                done(); 
            });
        }); 


        describe('getDiskHealthTestResult(device)', function () {
            it('should return PASSED if the disk is in OK status', function (done) {            
                var result = systat.getDiskHealthTestResult(systat.getDisks()[0]);              
                expect(result).to.be.an.string;
                expect(result).to.equal('PASSED');
                // console.log(result);
                done();
            });

            it('should return empty object if give a non exist device name', function (done) {
               var result = systat.getDiskHealthTestResult("nonexist0");
                expect(result).to.be.an.string;
                expect(result).to.be.empty;
                // console.log(result);
                done(); 
            });

            it('should return empty object if device name is in bad format', function (done) {
               var result = systat.getDiskHealthTestResult(";ls -l;");
                expect(result).to.be.an.string;
                expect(result).to.be.empty;                
                done(); 
            });

        });

        describe('getDiskInfo(device)', function () {
            it('should return disk SMART info as object', function (done) {            
                var result = systat.getDiskInfo(systat.getDisks()[0]);              
                expect(result).to.be.an.object;            
                // console.log(result);
                done();
            });

            it('should return empty object if give a non exist device name', function (done) {
               var result = systat.getDiskInfo("nonexists0");
                expect(result).to.be.an.object;
                expect(result).to.be.empty;
                // console.log(result);
                done(); 
            });

            it('should return empty object if device name is in bad format', function (done) {
               var result = systat.getDiskInfo(";ls -l;");
                expect(result).to.be.an.object;
                expect(result).to.be.empty;                
                done(); 
            });
        });    
    });

    describe('Network Features', function () {
        
        describe('getNetworkIOStat()', function () {
            it('it should return network interfaces I/O statistics info as object', function (done) {
                var result = systat.getNetworkIOStat();              
                expect(result).to.be.an.object;    
                done();
            });
        });

        describe('getNetworkInterfaceStatus()', function () {
            it('should return network interface physical status as object', function (done) {
                var result = systat.getNetworkInterfaceStatus();              
                expect(result).to.be.an.object;    
                done();
            });
        });
    });

    describe('Misc Features', function () {
        
        describe('getNICStat(nic)', function () {
            it('should return the mac stats info as object', function (done) {
                var result = systat.getNICStat("em0");              
                expect(result).to.be.an.object;
                // console.log(result);
                done();
            });      

            it('should return stats info with all zero filled if give a non exists nic', function (done) {
                var result = systat.getNICStat("non-exists");              
                expect(result).to.be.an.object;
                expect(result.packetsSent).to.equal(0);
                expect(result.packetsReceived).to.equal(0);                
                done();
            });    
        });
    });
});