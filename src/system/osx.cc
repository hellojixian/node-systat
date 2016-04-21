#include "system.h"

namespace shadowgrid {

    std::vector<int> System::getCPUTemperatures()
    {
        std::vector<int> result;

        return result;
    };

    NICStatInfo System::getNICStat(const char *name)
    {
        struct NICStatInfo info;
        bzero(&info, sizeof(NICStatInfo));

        return info;
    };


    std::vector<DiskIOStat> System::getDiskIOStat()
    {
        std::vector<DiskIOStat> list;

        return list;
    };

    std::vector<DiskPartitionInfo> System::getDiskPartitions(){
        std::vector<DiskPartitionInfo> list;
        
        return list;
    };

    DiskUsageInfo System::getDiskUsage(const char *name){
        DiskUsageInfo result;
        bzero(&result, sizeof(DiskUsageInfo));
        
        return result;
    };

    std::vector<NetworkIOStat> System::getNetworkIOStat(){
        std::vector<NetworkIOStat> result;
        
        return result;
    };   

    std::vector<NetworkInterfaceStatus> System::getNetworkInterfaceStatus(){
        std::vector<NetworkInterfaceStatus> result;

        return result;
    };

} // shadowgrid