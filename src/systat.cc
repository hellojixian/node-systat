#include <node.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "sensor/sensor.h"
#include "system/system.h"

namespace shadowgrid {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;		
	using v8::Array;
	using v8::Boolean;
	using v8::Number;

	void checkChip(const FunctionCallbackInfo<Value>& args){
	  	Isolate* isolate = args.GetIsolate();
	  	bool result = Sensor::checkChip();		
	  	args.GetReturnValue().Set(Boolean::New(isolate,result));
	}

	void getSystemTemperatures(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		//get values
		std::vector<int> ret = Sensor::getSystemTemperatures();
		//assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void getCPUTemperatures(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		//get values
		std::vector<int> ret = System::getCPUTemperatures();
		//assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}	

	void getFanSpeeds(const FunctionCallbackInfo<Value>& args){				
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		//get values
		std::vector<int> ret = Sensor::getFanSpeeds();
		//assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void getVoltages(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		//get values
		std::vector<int> ret = Sensor::getVoltages();
		//assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void getNICStat(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Object> results = Object::New(isolate);
		
		Local<Object> framesSentDetail = Object::New(isolate);
		Local<Object> framesReceivedDetail = Object::New(isolate);
		
		//get values
		v8::String::Utf8Value nic(args[0]->ToString());
		//call system
		NICStatInfo info = System::getNICStat(*nic);
		
		results->Set(String::NewFromUtf8(isolate,"packetsSent"), 
					 Number::New(isolate,info.packetsSent));				

		results->Set(String::NewFromUtf8(isolate,"packetsReceived"), 
					 Number::New(isolate,info.packetsReceived));
		
		results->Set(String::NewFromUtf8(isolate,"bytesSent"), 
					 Number::New(isolate,info.bytesSent));	
		
		results->Set(String::NewFromUtf8(isolate,"bytesReceived"), 
					 Number::New(isolate,info.bytesReceived));

		results->Set(String::NewFromUtf8(isolate,"framesSent"), 
					 Number::New(isolate,info.framesSent));		
		
		results->Set(String::NewFromUtf8(isolate,"framesReceived"), 
					 Number::New(isolate,info.framesReceived));

		results->Set(String::NewFromUtf8(isolate,"framesSentDetail"), 
					 framesSentDetail);

		framesSentDetail->Set(String::NewFromUtf8(isolate,"frames1024_1522"), 
					 		  Number::New(isolate,info.framesSentDetail.frames1024_1522));

		framesSentDetail->Set(String::NewFromUtf8(isolate,"frames512_1023"), 
					 		  Number::New(isolate,info.framesSentDetail.frames512_1023));

		framesSentDetail->Set(String::NewFromUtf8(isolate,"frames256_511"), 
					 		  Number::New(isolate,info.framesSentDetail.frames256_511));

		framesSentDetail->Set(String::NewFromUtf8(isolate,"frames128_255"), 
					 		  Number::New(isolate,info.framesSentDetail.frames256_511));

		framesSentDetail->Set(String::NewFromUtf8(isolate,"frames65_127"), 
					 		  Number::New(isolate,info.framesSentDetail.frames65_127));

		framesSentDetail->Set(String::NewFromUtf8(isolate,"frames64"), 
					 		  Number::New(isolate,info.framesSentDetail.frames64));

		results->Set(String::NewFromUtf8(isolate,"framesReceviedDetail"), 
					 framesReceivedDetail);

		framesReceivedDetail->Set(String::NewFromUtf8(isolate,"frames1024_1522"), 
					 		  Number::New(isolate,info.framesReceivedDetail.frames1024_1522));

		framesReceivedDetail->Set(String::NewFromUtf8(isolate,"frames512_1023"), 
					 		  Number::New(isolate,info.framesReceivedDetail.frames512_1023));

		framesReceivedDetail->Set(String::NewFromUtf8(isolate,"frames256_511"), 
					 		  Number::New(isolate,info.framesReceivedDetail.frames256_511));

		framesReceivedDetail->Set(String::NewFromUtf8(isolate,"frames128_255"), 
					 		  Number::New(isolate,info.framesReceivedDetail.frames256_511));

		framesReceivedDetail->Set(String::NewFromUtf8(isolate,"frames65_127"), 
					 		  Number::New(isolate,info.framesReceivedDetail.frames65_127));

		framesReceivedDetail->Set(String::NewFromUtf8(isolate,"frames64"), 
					 		  Number::New(isolate,info.framesReceivedDetail.frames64));

		results->Set(String::NewFromUtf8(isolate,"framesSentDetail"), 
					 framesSentDetail);

		results->Set(String::NewFromUtf8(isolate,"boardcastPacketsSent"), 
					 Number::New(isolate,info.boardcastPacketsSent));				

		results->Set(String::NewFromUtf8(isolate,"boardcastPacketsReceived"), 
					 Number::New(isolate,info.boardcastPacketsReceived));

		results->Set(String::NewFromUtf8(isolate,"multicastPacketsSent"), 
					 Number::New(isolate,info.multicastPacketsSent));		
		
		results->Set(String::NewFromUtf8(isolate,"multicastPacketsReceived"), 
					 Number::New(isolate,info.multicastPacketsReceived));

		

		//prepare output
		args.GetReturnValue().Set(results);
	}

	void init(Local<Object> exports) {
	  	NODE_SET_METHOD(exports, "checkChip", 			checkChip);
	  	NODE_SET_METHOD(exports, "getFanSpeeds", 		getFanSpeeds);
	  	NODE_SET_METHOD(exports, "getSystemTemperatures",getSystemTemperatures);
	  	NODE_SET_METHOD(exports, "getCPUTemperatures", 	getCPUTemperatures);
	  	NODE_SET_METHOD(exports, "getVoltages", 		getVoltages);
	  	NODE_SET_METHOD(exports, "getNICStat", 			getNICStat);
	}

	NODE_MODULE(systat, init)
}