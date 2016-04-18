#include "system.h"

#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <iostream>


#endif

namespace shadowgrid {

	std::vector<int> System::getCPUTempertures()
	{
		std::vector<int> result;
		// printf("i m here in ct\n");		
		int ret;
		int mib[5];		
		size_t len, len2;
		len = 5;

		// char *p;
		unsigned long value;
		const char *path = "dev.em.0.mac_stats.good_octets_recvd";

		ret = sysctlnametomib(path, mib, &len);
		printf("ret: %d",ret);
		printf("%s: %d %d %d %d len: %zu\n", path, mib[0], mib[1],mib[2],mib[3], len);
		
		ret = sysctl(mib, len, &value, &len2, NULL, 0);
		printf("ret: %d",ret);
		// p = static_cast<char *>(malloc(len2));
		// sysctl(mib, len, p, &len2, NULL, 0);
		printf("%s: %lu len: %zu\n", path, value, len2);

		// for (i = 0; i < 10; i++) {
		//    mib[2] = i;
		//    sysctl(mib, 4, NULL, &len, NULL, 0);
		//    p = static_cast<char *>(malloc(len));
		//    sysctl(mib, 4, p, &len, NULL, 0);

		//    if (sysctl(mib, 4, &p, &len, NULL, 0) == -1)
		// 		perror("sysctl");
		//    else	if (len	> 0){
		// 	   printf("max proc: %s len: %zu\n", p, len);
		//    }
	 //    }
	    
		return result;
	};

} // shadowgrid


