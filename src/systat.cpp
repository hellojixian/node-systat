#include <stdio.h>
#include <string>
#include "systat.h"
#include "it8728f/it8728f.h"

namespace shadowgrid {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;	

	void get_system_tempertures(const FunctionCallbackInfo<Value>& args){


	}

	void get_fan_speeds(const FunctionCallbackInfo<Value>& args){


	}

	void get_voltages(const FunctionCallbackInfo<Value>& args){


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
	  	NODE_SET_METHOD(exports, "test", Method);
	  	NODE_SET_METHOD(exports, "getFanSpeeds", get_fan_speeds);
	  	NODE_SET_METHOD(exports, "getSystemTempertures", get_system_tempertures);
	  	NODE_SET_METHOD(exports, "getVoltages", get_voltages);
	}

	NODE_MODULE(systat, init)
}