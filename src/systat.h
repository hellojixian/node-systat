#ifndef SYSTAT_H
#define SYSTAT_H

#include <node.h>

namespace shadowgrid {
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;

	void cpu_count_phys_sync(const FunctionCallbackInfo<Value>& args);
	void init(Local<Object> exports);
}

#endif //SYSTAT_H