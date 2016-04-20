
"use strict";

var chai = require('chai');
var expect = chai.expect;
var systat = require('../');

describe('systat', function () {
    
    describe('checkChip()', function () {
        it('should return true if the chip is supported', function (done) {
            var result = systat.checkChip();
            expect(result).to.be.a.boolean;
            done(); 
        });
    });

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

    describe('getNICStat(nic)', function () {
        it('shoudl return the mac stats info as object', function (done) {
            var result = systat.getNICStat("em0");              
            expect(result).to.be.an.object;
            // console.log(result);
            done();
        });        
    });

    describe('getDiskIOStat(device)', function () {
        it('shoudl return the disk I/O statistics info as object', function (done) {
            var result = systat.getDiskIOStat();              
            expect(result).to.be.an.object;
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
    });

    describe('getDiskTemperatures(device)', function () {
        it('should return the disk temperature(c) as int', function (done) {
            var disks = systat.getDiskIOStat();
            var result = systat.getDiskTemperature(Object.keys(disks)[0]);              
            expect(result).to.be.an.int;
            // console.log(result);
            done();
        });
    }); 

    describe('getDiskTemperatures(device)', function () {
        it('should return the disk temperature(c) as int', function (done) {
            var disks = systat.getDiskIOStat();
            var result = systat.getDiskTemperature(Object.keys(disks)[0]);              
            expect(result).to.be.an.int;
            // console.log(result);
            done();
        });
    }); 

    describe('getDisks()', function () {
        it('should return the disk name as array', function (done) {            
            var result = systat.getDisks();              
            expect(result).to.be.an.array;
            // console.log(result);
            done();
        });
    });     

});