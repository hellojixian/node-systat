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

	void checkChip(const FunctionCallbackInfo<Value>& args){
	  	Isolate* isolate = args.GetIsolate();
	  	bool result = Sensor::checkChip();		
	  	args.GetReturnValue().Set(Boolean::New(isolate,result));
	}

	void getSystemTempertures(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = Sensor::getSystemTempertures();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void getCPUTempertures(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = System::getCPUTempertures();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}	

	void getFanSpeeds(const FunctionCallbackInfo<Value>& args){				
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = Sensor::getFanSpeeds();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void getVoltages(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = Sensor::getVoltages();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void init(Local<Object> exports) {
	  	NODE_SET_METHOD(exports, "checkChip", 			checkChip);
	  	NODE_SET_METHOD(exports, "getFanSpeeds", 		getFanSpeeds);
	  	NODE_SET_METHOD(exports, "getSystemTempertures",getSystemTempertures);
	  	NODE_SET_METHOD(exports, "getCPUTempertures", 	getCPUTempertures);
	  	NODE_SET_METHOD(exports, "getVoltages", 		getVoltages);
	}

	NODE_MODULE(systat, init)
}