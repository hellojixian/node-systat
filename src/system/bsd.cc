#include "system.h"

#ifdef __FreeBSD__
#include <machine/cpufunc.h>
#include <sys/stat.h>
#include <iostream>


#endif

namespace shadowgrid {

	std::vector<int> bsd::get_cpu_tempertures()
	{
		std::vector<int> result;
		// printf("i m here in ct\n");		
		return result;
	};

} // shadowgrid


