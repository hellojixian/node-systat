#ifndef BSD_H
#define BSD_H

#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/sysctl.h>


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


	class System
	{
	public:
		static std::vector<int> getCPUTemperatures();
		static NICStatInfo getNICStat(const char *name);
	};
}

#endif //BSD_H