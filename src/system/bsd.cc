#include "system.h"

#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>


#ifdef __FreeBSD__
#include <regex>

#endif

namespace shadowgrid {

	std::vector<int> System::getCPUTemperatures()
	{
		std::vector<int> result;
#ifdef __FreeBSD__
		int mib[2], ret, ncpu,temperature;
		size_t len =4;				
				
		//get cpu number				
		ret = sysctlbyname("hw.ncpu", &ncpu, &len, NULL, 0);
		//get each cpu temperature		
		for(int i=0; i<ncpu; i++){
			
			std::string path = "dev.cpu."+std::to_string(i)+".temperature";
			len = 4;
			ret = sysctlbyname(path.c_str(), &temperature, &len, NULL, 0);
			temperature = (temperature - 2732) / 10;			
			result.push_back(temperature);
		}
#endif
		return result;
	};

	NICStatInfo System::getNICStat(const char *name)
	{
		NICStatInfo info;
		FrameStatInfo framesSentDetail;
		FrameStatInfo framesReceivedDetail;

		info.framesReceivedDetail = framesReceivedDetail;
		info.framesSentDetail = framesSentDetail;

#ifdef __FreeBSD__	
		//parse parameters
		std::string nicName = static_cast<std::string>(name);
		std::regex regexp ("([a-z].*)([0-9].*)");
		std::smatch sm;
		std::regex_match(nicName,sm,regexp);		
		std::string nic = static_cast<std::string>(sm[1]).c_str();
		std::string num = static_cast<std::string>(sm[2]).c_str();		

		// fetch values		
		std::string path;
		int value, ret;
		size_t len =4;
		
		//bytes
		path = "dev."+nic+"."+num+".mac_stats.good_octets_txd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.bytesSent = value;

		path = "dev."+nic+"."+num+".mac_stats.good_octets_recvd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.bytesReceived = value;

		//packets
		path = "dev."+nic+"."+num+".mac_stats.total_pkts_txd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.packetsSent = value;

		path = "dev."+nic+"."+num+".mac_stats.total_pkts_recvd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.packetsReceived = value;

		//boardcast packets
		path = "dev."+nic+"."+num+".mac_stats.bcast_pkts_txd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.boardcastPacketsReceived = value;

		path = "dev."+nic+"."+num+".mac_stats.bcast_pkts_recvd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.boardcastPacketsSent = value;

		//multicast packets
		path = "dev."+nic+"."+num+".mac_stats.mcast_pkts_txd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.multicastPacketsReceived = value;

		path = "dev."+nic+"."+num+".mac_stats.mcast_pkts_recvd";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.multicastPacketsSent = value;

		//prepare frames info for sent
		path = "dev."+nic+"."+num+".mac_stats.tx_frames_64";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesSentDetail.frames64 = value;
		info.framesSent = value;

		path = "dev."+nic+"."+num+".mac_stats.tx_frames_65_127";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesSentDetail.frames65_127 = value;
		info.framesSent += value;

		path = "dev."+nic+"."+num+".mac_stats.tx_frames_128_255";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesSentDetail.frames128_255 = value;
		info.framesSent += value;

		path = "dev."+nic+"."+num+".mac_stats.tx_frames_256_511";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesSentDetail.frames256_511 = value;
		info.framesSent += value;

		path = "dev."+nic+"."+num+".mac_stats.tx_frames_512_1023";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesSentDetail.frames512_1023 = value;
		info.framesSent += value;

		path = "dev."+nic+"."+num+".mac_stats.tx_frames_1024_1522";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesSentDetail.frames1024_1522 = value;
		info.framesSent += value;

		//prepare frames info for received
		path = "dev."+nic+"."+num+".mac_stats.rx_frames_64";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesReceivedDetail.frames64 = value;
		info.framesReceived = value;

		path = "dev."+nic+"."+num+".mac_stats.rx_frames_65_127";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesReceivedDetail.frames65_127 = value;
		info.framesReceived += value;

		path = "dev."+nic+"."+num+".mac_stats.rx_frames_128_255";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesReceivedDetail.frames128_255 = value;
		info.framesReceived += value;

		path = "dev."+nic+"."+num+".mac_stats.rx_frames_256_511";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesReceivedDetail.frames256_511 = value;
		info.framesReceived += value;

		path = "dev."+nic+"."+num+".mac_stats.rx_frames_512_1023";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesReceivedDetail.frames512_1023 = value;
		info.framesReceived += value;

		path = "dev."+nic+"."+num+".mac_stats.rx_frames_1024_1522";
		ret = sysctlbyname(path.c_str(), &value, &len, NULL, 0);
		info.framesReceivedDetail.frames1024_1522 = value;		
		info.framesReceived += value;
#endif		


		return info;
	};

} // shadowgrid


