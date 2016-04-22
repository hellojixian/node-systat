#include <node.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
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
		
		//get arugments
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

	void getDiskIOStat(const FunctionCallbackInfo<Value>& args){		
		Isolate* isolate = args.GetIsolate();
				
		Local<Object> result = Object::New(isolate);
		
		//call system				
		std::vector<DiskIOStat> disks = System::getDiskIOStat();

		for (auto it=disks.begin(); it != disks.end(); ++it) {

		    Local<Object> disk = Object::New(isolate);

		    disk->Set(String::NewFromUtf8(isolate,"readOperations"),
		    			Number::New(isolate,it->readOperations));
		   	disk->Set(String::NewFromUtf8(isolate,"writeOperations"),
		    			Number::New(isolate,it->writeOperations));
			disk->Set(String::NewFromUtf8(isolate,"readBytes"),
		    			Number::New(isolate,it->readBytes));
		   	disk->Set(String::NewFromUtf8(isolate,"writeBytes"),
		    			Number::New(isolate,it->writeBytes));
		    disk->Set(String::NewFromUtf8(isolate,"readTime"),
		    			Number::New(isolate,it->readTime));
		    disk->Set(String::NewFromUtf8(isolate,"writeTime"),
		    			Number::New(isolate,it->writeTime));
		    disk->Set(String::NewFromUtf8(isolate,"busyTime"),
		    			Number::New(isolate,it->busyTime));

		    result->Set(String::NewFromUtf8(isolate,it->name),disk);	
		}
		args.GetReturnValue().Set(result);
	}

	void getDiskPartitions(const FunctionCallbackInfo<Value>& args){		
		Isolate* isolate = args.GetIsolate();						
		
		Local<Array> result = Array::New(isolate);
		//call system				
		std::vector<DiskPartitionInfo> partitions = System::getDiskPartitions();

		for (auto it=partitions.begin(); it != partitions.end(); ++it) {

		    Local<Object> partition = Object::New(isolate);

		    partition->Set(String::NewFromUtf8(isolate,"device"),
		    		  	   String::NewFromUtf8(isolate,it->device));
		   	partition->Set(String::NewFromUtf8(isolate,"mountPoint"),
		    		  	   String::NewFromUtf8(isolate,it->mountPoint));
			partition->Set(String::NewFromUtf8(isolate,"fileSystem"),
		    		  	   String::NewFromUtf8(isolate,it->fileSystem));
		   	
		    result->Set(std::distance(partitions.begin(),it),partition);	
		}
		args.GetReturnValue().Set(result);
	}

	void getDiskUsage(const FunctionCallbackInfo<Value>& args){		
		Isolate* isolate = args.GetIsolate();
				
		Local<Object> result = Object::New(isolate);
		
		//get arugments
		v8::String::Utf8Value mountPoint(args[0]->ToString());
		//call system				
		DiskUsageInfo disk = System::getDiskUsage(*mountPoint);

		Local<Object> space = Object::New(isolate);
		
	    space->Set(String::NewFromUtf8(isolate,"total"),
	    			Number::New(isolate,disk.space.total));

	    space->Set(String::NewFromUtf8(isolate,"used"),
	    			Number::New(isolate,disk.space.used));

	    space->Set(String::NewFromUtf8(isolate,"free"),
	    			Number::New(isolate,disk.space.free));

	    space->Set(String::NewFromUtf8(isolate,"percent"),
	    			Number::New(isolate,disk.space.percent));

		result->Set(String::NewFromUtf8(isolate,"space"),space);



		Local<Object> inode = Object::New(isolate);
		
	    inode->Set(String::NewFromUtf8(isolate,"total"),
	    			Number::New(isolate,disk.inode.total));

	    inode->Set(String::NewFromUtf8(isolate,"used"),
	    			Number::New(isolate,disk.inode.used));

	    inode->Set(String::NewFromUtf8(isolate,"free"),
	    			Number::New(isolate,disk.inode.free));

	    inode->Set(String::NewFromUtf8(isolate,"percent"),
	    			Number::New(isolate,disk.inode.percent));

		result->Set(String::NewFromUtf8(isolate,"inode"),inode);
		
		
		args.GetReturnValue().Set(result);
	}

	void getNetworkIOStat(const FunctionCallbackInfo<Value>& args){		
		Isolate* isolate = args.GetIsolate();
				
		Local<Object> result = Object::New(isolate);
		
		//call system				
		std::vector<NetworkIOStat> stats = System::getNetworkIOStat();
		for(auto it = stats.begin(); it!= stats.end(); ++it){
			Local<Object> interface = Object::New(isolate);

			interface->Set(String::NewFromUtf8(isolate,"bytesReceived"),
							Number::New(isolate, it->bytesReceived));

			interface->Set(String::NewFromUtf8(isolate,"bytesSent"),
							Number::New(isolate, it->bytesSent));

			interface->Set(String::NewFromUtf8(isolate,"packetsReceived"),
							Number::New(isolate, it->packetsReceived));

			interface->Set(String::NewFromUtf8(isolate,"packetsSent"),
							Number::New(isolate, it->packetsSent));

			interface->Set(String::NewFromUtf8(isolate,"multicastPacketsReceived"),
							Number::New(isolate, it->multicastPacketsReceived));

			interface->Set(String::NewFromUtf8(isolate,"multicastPacketsSent"),
							Number::New(isolate, it->multicastPacketsSent));

			interface->Set(String::NewFromUtf8(isolate,"errorIn"),
							Number::New(isolate, it->errorIn));
			
			interface->Set(String::NewFromUtf8(isolate,"errorOut"),
							Number::New(isolate, it->errorOut));
			
			interface->Set(String::NewFromUtf8(isolate,"dropIn"),
							Number::New(isolate, it->dropIn));
			
			interface->Set(String::NewFromUtf8(isolate,"dropOut"),
							Number::New(isolate, it->dropOut));
			
			interface->Set(String::NewFromUtf8(isolate,"collisions"),
							Number::New(isolate, it->collisions));

			result->Set(String::NewFromUtf8(isolate, it->name), interface);
		}
		args.GetReturnValue().Set(result);
	}


	void getNetworkInterfaceStatus(const FunctionCallbackInfo<Value>& args){		
		Isolate* isolate = args.GetIsolate();
				
		Local<Object> result = Object::New(isolate);
		
		//call system				
		std::vector<NetworkInterfaceStatus> stats = System::getNetworkInterfaceStatus();
		
		for(auto it = stats.begin(); it!= stats.end(); ++it){
			Local<Object> interface = Object::New(isolate);

			interface->Set(String::NewFromUtf8(isolate,"isUp"),
							Boolean::New(isolate, it->isUp));

			interface->Set(String::NewFromUtf8(isolate,"speed"),
							Number::New(isolate, it->speed));

			interface->Set(String::NewFromUtf8(isolate,"mtu"),
							Number::New(isolate, it->mtu));

			interface->Set(String::NewFromUtf8(isolate,"duplex"),
							Number::New(isolate, it->duplex));

			result->Set(String::NewFromUtf8(isolate, it->name), interface);
		}

		args.GetReturnValue().Set(result);
	}

	void init(Local<Object> exports) {
	  	NODE_SET_METHOD(exports, "checkChip", 			checkChip);
	  	NODE_SET_METHOD(exports, "getFanSpeeds", 		getFanSpeeds);
	  	NODE_SET_METHOD(exports, "getSystemTemperatures",getSystemTemperatures);
	  	NODE_SET_METHOD(exports, "getCPUTemperatures", 	getCPUTemperatures);
	  	NODE_SET_METHOD(exports, "getVoltages", 		getVoltages);
	  	NODE_SET_METHOD(exports, "getNICStat", 			getNICStat);
	  	NODE_SET_METHOD(exports, "getDiskIOStat", 		getDiskIOStat);	  	
	  	NODE_SET_METHOD(exports, "getDiskPartitions", 	getDiskPartitions);
	  	NODE_SET_METHOD(exports, "getDiskUsage", 		getDiskUsage);
	  	NODE_SET_METHOD(exports, "getNetworkIOStat", 	getNetworkIOStat);
	  	NODE_SET_METHOD(exports, "getNetworkInterfaceStatus", 	getNetworkInterfaceStatus);
	}

	NODE_MODULE(systat, init)
}