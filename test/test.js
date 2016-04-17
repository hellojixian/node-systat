"use strict";

var chai = require('chai');
var expect = chai.expect;
var systat = require('../lib/systat');

describe('systat', function () {
    describe('version()', function () {
        it('should return the version number', function (done) {
            console.log(systat.test("hello sss"));            
            done();
        });        
    });
});