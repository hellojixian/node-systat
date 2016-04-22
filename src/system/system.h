#ifndef BSD_H
#define BSD_H

#include <stdio.h>
#include <vector>


namespace shadowgrid 
{

	struct FrameStatInfo {
		unsigned long frames64;
		unsigned long frames65_127;
		unsigned long frames128_255;
		unsigned long frames256_511;
		unsigned long frames512_1023;
		unsigned long frames1024_1522;
	};


	struct NICStatInfo {
		unsigned long bytesReceived;
		unsigned long bytesSent;
		unsigned long packetsReceived;
		unsigned long packetsSent;
		unsigned long framesReceived;
		unsigned long framesSent;				
		unsigned long boardcastPacketsReceived;
		unsigned long boardcastPacketsSent;
		unsigned long multicastPacketsReceived;
		unsigned long multicastPacketsSent;
		struct FrameStatInfo framesReceivedDetail;
		struct FrameStatInfo framesSentDetail;
	};

	struct DiskIOStat {
		char name[128];
		unsigned long readOperations;
		unsigned long writeOperations;
		unsigned long readBytes;
		unsigned long writeBytes;
		long long readTime;
		long long writeTime;
		long long busyTime;
	};

	struct DiskPartitionInfo {
		char device[128];
		char mountPoint[128];
		char fileSystem[128];
	};

	struct UsageInfo {
		unsigned long total;
		unsigned long used;
		unsigned long free;
		unsigned int percent;
	};

	struct DiskUsageInfo {
		struct UsageInfo space;
		struct UsageInfo inode;
	};

	struct NetworkIOStat {
		char name[32];
		unsigned long bytesReceived;
		unsigned long bytesSent;
		unsigned long packetsReceived;
		unsigned long packetsSent;
		unsigned long multicastPacketsReceived;
		unsigned long multicastPacketsSent;
		unsigned long errorIn;
		unsigned long errorOut;
		unsigned long dropIn;
		unsigned long dropOut;
		unsigned long collisions;
	};

	struct NetworkInterfaceStatus{
		char name[32];
		bool isUp;
		unsigned int speed;
		unsigned int duplex;
		unsigned int mtu;
	};

	class System
	{
	public:
		static std::vector<int> getCPUTemperatures();		
		static NICStatInfo getNICStat(const char *name);
		
		static std::vector<DiskIOStat> getDiskIOStat();
		static std::vector<DiskPartitionInfo> getDiskPartitions();
		static DiskUsageInfo getDiskUsage(const char *name);

		static std::vector<NetworkIOStat> getNetworkIOStat();	
		static std::vector<NetworkInterfaceStatus> getNetworkInterfaceStatus();
	private:
		static int _getNicSpeed(int ifm_active);
	};
}

#endif //BSD_H