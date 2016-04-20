#ifndef BSD_H
#define BSD_H

#include <stdio.h>
#include <vector>


namespace shadowgrid 
{

	struct FrameStatInfo {
		unsigned int frames64;
		unsigned int frames65_127;
		unsigned int frames128_255;
		unsigned int frames256_511;
		unsigned int frames512_1023;
		unsigned int frames1024_1522;
	};

	struct NICErrorStatInfo{
		unsigned int crcErrors;
		unsigned int receviedErrors;
		unsigned int receviedJabber;
	};

	struct NICStatInfo {
		unsigned int bytesReceived;
		unsigned int bytesSent;
		unsigned int packetsReceived;
		unsigned int packetsSent;
		unsigned int framesReceived;
		unsigned int framesSent;		
		shadowgrid::FrameStatInfo framesReceivedDetail;
		shadowgrid::FrameStatInfo framesSentDetail;
		unsigned int boardcastPacketsReceived;
		unsigned int boardcastPacketsSent;
		unsigned int multicastPacketsReceived;
		unsigned int multicastPacketsSent;
	};

	struct NetworkIOStat {

	};

	struct DiskIOStat {
		char name[128];
		unsigned int readOperations;
		unsigned int writeOperations;
		unsigned int readBytes;
		unsigned int writeBytes;
		long long readTime;
		long long writeTime;
		long long busyTime;
	};

	struct DiskPartitionInfo {
		char device[128];
		char mountPoint[128];
		char fileSystem[128];
	};

	class System
	{
	public:
		static std::vector<int> getCPUTemperatures();		
		static int getDiskTemperature(const char *name);		
		static NICStatInfo getNICStat(const char *name);
		
		static std::vector<DiskIOStat> getDiskIOStat();
		static std::vector<DiskPartitionInfo> getDiskPartitions();

		static std::vector<NetworkIOStat> getNetworkIOStat();	

	};
}

#endif //BSD_H