#include <stdio.h>
#include <string>
#include "systat.h"

namespace shadowgrid {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;	

	void Method(const FunctionCallbackInfo<Value>& args) {
	  	Isolate* isolate = args.GetIsolate();

		v8::String::Utf8Value strValue(args[0]->ToString());
		std::string _str = std::string(*strValue);
		_str = "[ "+_str+" ]";
	  	Local<String> str = v8::String::NewFromUtf8(isolate, _str.c_str());

	  	// Local<String> str = String::Concat(String::Concat("[",args[0]->ToString()),"]");
	  	args.GetReturnValue().Set(str);
	}

	void init(Local<Object> exports) {
	  	NODE_SET_METHOD(exports, "test", Method);
	}

	NODE_MODULE(systat, init)

}