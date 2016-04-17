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

	void check_chip(const FunctionCallbackInfo<Value>& args){
	  	Isolate* isolate = args.GetIsolate();
	  	bool result = sensor::check_chip();		
	  	args.GetReturnValue().Set(Boolean::New(isolate,result));
	}

	void get_system_tempertures(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = sensor::get_system_tempertures();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void get_cpu_tempertures(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = bsd::get_cpu_tempertures();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}	

	void get_fan_speeds(const FunctionCallbackInfo<Value>& args){				
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = sensor::get_fan_speeds();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void get_voltages(const FunctionCallbackInfo<Value>& args){
		Isolate* isolate = args.GetIsolate();
		Local<Array> results = Array::New(isolate);
		
		// //get values
		std::vector<int> ret = sensor::get_voltages();
		// //assign value to v8 
		for (std::vector<int>::iterator it = ret.begin() ; it != ret.end(); ++it){
			results->Set(std::distance(ret.begin(),it), 	//index
						 v8::Number::New(isolate,*it));		//value
		}

		args.GetReturnValue().Set(results);
	}

	void Method(const FunctionCallbackInfo<Value>& args) {
	  	Isolate* isolate = args.GetIsolate();

		v8::String::Utf8Value strValue(args[0]->ToString());
		std::string _str = std::string(*strValue);
		_str = "[ "+_str+" ]";
	  	Local<String> str = v8::String::NewFromUtf8(isolate, _str.c_str());

	  	args.GetReturnValue().Set(str);
	}

	void init(Local<Object> exports) {
	  	NODE_SET_METHOD(exports, "checkChip", check_chip);
	  	NODE_SET_METHOD(exports, "getFanSpeeds", get_fan_speeds);
	  	NODE_SET_METHOD(exports, "getSystemTempertures", get_system_tempertures);
	  	NODE_SET_METHOD(exports, "getCPUTempertures", get_cpu_tempertures);
	  	NODE_SET_METHOD(exports, "getVoltages", get_voltages);
	}

	NODE_MODULE(systat, init)
}