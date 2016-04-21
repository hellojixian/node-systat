#include "system.h"
#include <iostream>



#ifdef __FreeBSD__
#include <regex>
#include <net/if.h>       // net io counters
#include <net/if_dl.h>
#include <net/route.h>
#include <net/if_media.h>
#include <net/if_var.h>
#include <net/if_types.h>
#include <devstat.h>  // get io counters
#include <libutil.h>  // process open files, shared libs (kinfo_getvmmap)
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <strings.h>

#include <netdb.h>
#include <netinet/in.h>

     
#if __FreeBSD_version < 900000
    #include <utmp.h>  // system users
#else
    #include <utmpx.h>
#endif

// convert a bintime struct to milliseconds
#define BT2MSEC(bt) (bt.sec * 1000 + (((uint64_t) 1000000000 * \
                       (uint32_t) (bt.frac >> 32) ) >> 32 ) / 1000000)


namespace shadowgrid {

    std::vector<int> System::getCPUTemperatures()
    {
        std::vector<int> result;
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
        return result;
    };

    NICStatInfo System::getNICStat(const char *name)
    {
        struct NICStatInfo info;


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

        return info;
    };


    std::vector<DiskIOStat> System::getDiskIOStat()
    {
        int ret=0,i=0;
        std::vector<DiskIOStat> list;
        struct statinfo stats;
        
        if (devstat_checkversion(NULL) < 0) return list;
        stats.dinfo = (struct devinfo *)malloc(sizeof(struct devinfo));
        bzero(stats.dinfo, sizeof(struct devinfo));
        ret = devstat_getdevs(NULL, &stats);

        for (i = 0; i < stats.dinfo->numdevs; i++) {
            struct DiskIOStat disk;            
            struct devstat current;

            char disk_name[128];
            current = stats.dinfo->devices[i];
            snprintf(disk_name, sizeof(disk_name), "%s%d",
                     current.device_name,
                     current.unit_number);
            
            if(!strcmp(current.device_name,"pass")) continue;

            strcpy(disk.name, disk_name);            
            disk.readOperations    =  current.operations[DEVSTAT_READ];
            disk.writeOperations   =  current.operations[DEVSTAT_WRITE];
            disk.readBytes         =  current.bytes[DEVSTAT_READ];
            disk.writeBytes        =  current.bytes[DEVSTAT_WRITE];
            disk.readTime          =  BT2MSEC(current.duration[DEVSTAT_READ]);
            disk.writeTime         =  BT2MSEC(current.duration[DEVSTAT_WRITE]);
            disk.busyTime          =  BT2MSEC(current.busy_time);

            list.push_back(disk);
        }

        if (stats.dinfo->mem_ptr) free(stats.dinfo->mem_ptr);
        free(stats.dinfo);    
        return list;
    };

    std::vector<DiskPartitionInfo> System::getDiskPartitions(){
        std::vector<DiskPartitionInfo> list;
        int num; long len;   
        struct statfs *fs = NULL;
        num = getfsstat(NULL, 0, MNT_NOWAIT);
        len = sizeof(*fs) * num;
        fs = (struct statfs *)malloc(len);
        num = getfsstat(fs, len, MNT_NOWAIT);
        for(int i=0; i<num; i++){
            if(strncmp(fs[i].f_mntfromname,"/dev/",5)!=0) continue;
            DiskPartitionInfo info;
            bzero(&info, sizeof(DiskPartitionInfo));
            strcpy(info.device,     fs[i].f_mntfromname);
            strcpy(info.mountPoint, fs[i].f_mntonname);
            strcpy(info.fileSystem, fs[i].f_fstypename);            
            list.push_back(info);            
        }
        return list;
    };

    DiskUsageInfo System::getDiskUsage(const char *name){
        DiskUsageInfo result;
        bzero(&result, sizeof(DiskUsageInfo));
        int ret;
        struct statvfs stat;
        ret = statvfs(name,&stat);
        if(ret!=0) return result;

        result.free  = stat.f_bavail * stat.f_frsize;
        result.total = stat.f_blocks * stat.f_frsize;
        result.used  = (stat.f_blocks - stat.f_bfree) * stat.f_frsize;
        result.percent = (((double)result.used/(double)result.total)*10000)/100;

        return result;
    };


    std::vector<NetworkIOStat> System::getNetworkIOStat(){
        std::vector<NetworkIOStat> result;
        char *buf = NULL, *lim, *next;
        struct if_msghdr *ifm;
        int mib[6];
        size_t len;
        
        mib[0] = CTL_NET;          // networking subsystem
        mib[1] = PF_ROUTE;         // type of information
        mib[2] = 0;                // protocol (IPPROTO_xxx)
        mib[3] = 0;                // address family
        mib[4] = NET_RT_IFLIST;    // operation
        mib[5] = 0;

        sysctl(mib, 6, NULL, &len, NULL, 0);
        buf = (char *)malloc(len);
        sysctl(mib, 6, buf, &len, NULL, 0);
        lim = buf + len;
        for (next = buf; next < lim; ) {
            
            ifm = (struct if_msghdr *)next;
            next += ifm->ifm_msglen;

            if (ifm->ifm_type == RTM_IFINFO) {        
                struct if_msghdr *if2m = (struct if_msghdr *)ifm;
                struct sockaddr_dl *sdl = (struct sockaddr_dl *)(if2m + 1);
                struct NetworkIOStat stat;
                bzero(&stat, sizeof(struct NetworkIOStat));

                strncpy(stat.name, sdl->sdl_data, sdl->sdl_nlen);
                stat.name[sdl->sdl_nlen] = 0;
                
                if (strncmp(stat.name, "usbus", 5) == 0)
                    continue;

                stat.bytesReceived      = if2m->ifm_data.ifi_ibytes;
                stat.bytesSent          = if2m->ifm_data.ifi_obytes;
                stat.packetsReceived    = if2m->ifm_data.ifi_ipackets;
                stat.packetsSent        = if2m->ifm_data.ifi_opackets;
                stat.multicastPacketsReceived  = if2m->ifm_data.ifi_imcasts;
                stat.multicastPacketsSent      = if2m->ifm_data.ifi_omcasts;
                stat.errorIn            = if2m->ifm_data.ifi_ierrors;
                stat.errorOut           = if2m->ifm_data.ifi_oerrors;
                stat.collisions         = if2m->ifm_data.ifi_collisions;
                stat.dropIn             = if2m->ifm_data.ifi_iqdrops;
#ifdef _IFI_OQDROPS                
                stat.dropOut            = if2m->ifm_data.ifi_oqdrops;
#else
                stat.dropOut            = 0;
#endif
                result.push_back(stat);
            }else {
                continue;
            }
        }
        free(buf);
        return result;
    };   
    
    std::vector<NetworkInterfaceStatus> System::getNetworkInterfaceStatus(){
        std::vector<NetworkInterfaceStatus> result;

        std::vector<NetworkIOStat> ifaces = getNetworkIOStat();
        for(auto it = ifaces.begin(); it!=ifaces.end(); ++it){            

            int sock = 0;
            int ret;
                 
            
            struct ifreq ifr;
            struct ifmediareq ifmed;
            struct NetworkInterfaceStatus status;        

            bzero(&status,sizeof(struct NetworkInterfaceStatus));
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            strncpy(ifr.ifr_name, it->name, sizeof(ifr.ifr_name));


            strcpy(status.name,it->name);

            //get cable status
            ret = ioctl(sock, SIOCGIFFLAGS, &ifr);        
            if ((ifr.ifr_flags & IFF_UP) != 0)
                status.isUp = true;
            else
                status.isUp = false;

            //get mtu 
            ret = ioctl(sock, SIOCGIFMTU, &ifr);    
            status.mtu = ifr.ifr_mtu;

            //get duplex & speed
            memset(&ifmed, 0, sizeof(struct ifmediareq));
            strlcpy(ifmed.ifm_name, it->name, sizeof(ifmed.ifm_name));
            ret = ioctl(sock, SIOCGIFMEDIA, (caddr_t)&ifmed);
            if (ret == -1) {
                status.speed = 0;
                status.duplex = 0;
            }
            else {
                status.speed = _getNicSpeed(ifmed.ifm_active);
                if ((ifmed.ifm_active | IFM_FDX) == ifmed.ifm_active)
                    status.duplex = 2;
                else if ((ifmed.ifm_active | IFM_HDX) == ifmed.ifm_active)
                    status.duplex = 1;
                else
                    status.duplex = 0;
            } 

            result.push_back(status);
        }
        
        return result;
    };


    int System::_getNicSpeed(int ifm_active) {
        // Determine NIC speed. Taken from:
        // http://www.i-scream.org/libstatgrab/
        // Assuming only ETHER devices
        switch(IFM_TYPE(ifm_active)) {
            case IFM_ETHER:
                switch(IFM_SUBTYPE(ifm_active)) {
    #if defined(IFM_HPNA_1) && ((!defined(IFM_10G_LR)) \
        || (IFM_10G_LR != IFM_HPNA_1))
                    // HomePNA 1.0 (1Mb/s)
                    case(IFM_HPNA_1):
                        return 1;
    #endif
                    // 10 Mbit
                    case(IFM_10_T):  // 10BaseT - RJ45
                    case(IFM_10_2):  // 10Base2 - Thinnet
                    case(IFM_10_5):  // 10Base5 - AUI
                    case(IFM_10_STP):  // 10BaseT over shielded TP
                    case(IFM_10_FL):  // 10baseFL - Fiber
                        return 10;
                    // 100 Mbit
                    case(IFM_100_TX):  // 100BaseTX - RJ45
                    case(IFM_100_FX):  // 100BaseFX - Fiber
                    case(IFM_100_T4):  // 100BaseT4 - 4 pair cat 3
                    case(IFM_100_VG):  // 100VG-AnyLAN
                    case(IFM_100_T2):  // 100BaseT2
                        return 100;
                    // 1000 Mbit
                    case(IFM_1000_SX):  // 1000BaseSX - multi-mode fiber
                    case(IFM_1000_LX):  // 1000baseLX - single-mode fiber
                    case(IFM_1000_CX):  // 1000baseCX - 150ohm STP
    #if defined(IFM_1000_TX) && !defined(__OpenBSD__)
                    // FreeBSD 4 and others (but NOT OpenBSD) -> #define IFM_1000_T in net/if_media.h
                    case(IFM_1000_TX):
    #endif
    #ifdef IFM_1000_FX
                    case(IFM_1000_FX):
    #endif
    #ifdef IFM_1000_T
                    case(IFM_1000_T):
    #endif
                        return 1000;
    #if defined(IFM_10G_SR) || defined(IFM_10G_LR) || defined(IFM_10G_CX4) \
             || defined(IFM_10G_T)
    #ifdef IFM_10G_SR
                    case(IFM_10G_SR):
    #endif
    #ifdef IFM_10G_LR
                    case(IFM_10G_LR):
    #endif
    #ifdef IFM_10G_CX4
                    case(IFM_10G_CX4):
    #endif
    #ifdef IFM_10G_TWINAX
                    case(IFM_10G_TWINAX):
    #endif
    #ifdef IFM_10G_TWINAX_LONG
                    case(IFM_10G_TWINAX_LONG):
    #endif
    #ifdef IFM_10G_T
                    case(IFM_10G_T):
    #endif
                        return 10000;
    #endif
    #if defined(IFM_2500_SX)
    #ifdef IFM_2500_SX
                    case(IFM_2500_SX):
    #endif
                        return 2500;
    #endif // any 2.5GBit stuff...
                    // We don't know what it is
                    default:
                        return 0;
                }
                break;

    #ifdef IFM_TOKEN
            case IFM_TOKEN:
                switch(IFM_SUBTYPE(ifm_active)) {
                    case IFM_TOK_STP4:  // Shielded twisted pair 4m - DB9
                    case IFM_TOK_UTP4:  // Unshielded twisted pair 4m - RJ45
                        return 4;
                    case IFM_TOK_STP16:  // Shielded twisted pair 16m - DB9
                    case IFM_TOK_UTP16:  // Unshielded twisted pair 16m - RJ45
                        return 16;
    #if defined(IFM_TOK_STP100) || defined(IFM_TOK_UTP100)
    #ifdef IFM_TOK_STP100
                    case IFM_TOK_STP100:  // Shielded twisted pair 100m - DB9
    #endif
    #ifdef IFM_TOK_UTP100
                    case IFM_TOK_UTP100:  // Unshielded twisted pair 100m - RJ45
    #endif
                        return 100;
    #endif
                    // We don't know what it is
                    default:
                        return 0;
                }
                break;
    #endif

    #ifdef IFM_FDDI
            case IFM_FDDI:
                switch(IFM_SUBTYPE(ifm_active)) {
                    // We don't know what it is
                    default:
                        return 0;
                }
                break;
    #endif
            case IFM_IEEE80211:
                switch(IFM_SUBTYPE(ifm_active)) {
                    case IFM_IEEE80211_FH1:  // Frequency Hopping 1Mbps
                    case IFM_IEEE80211_DS1:  // Direct Sequence 1Mbps
                        return 1;
                    case IFM_IEEE80211_FH2:  // Frequency Hopping 2Mbps
                    case IFM_IEEE80211_DS2:  // Direct Sequence 2Mbps
                        return 2;
                    case IFM_IEEE80211_DS5:  // Direct Sequence 5Mbps
                        return 5;
                    case IFM_IEEE80211_DS11:  // Direct Sequence 11Mbps
                        return 11;
                    case IFM_IEEE80211_DS22:  // Direct Sequence 22Mbps
                        return 22;
                    // We don't know what it is
                    default:
                        return 0;
                }
                break;

            default:
                return 0;
        }
    }

} // shadowgrid
#endif

