#ifndef BSD_H
#define BSD_H

#include <stdio.h>
#include <vector>

namespace shadowgrid 
{
	class bsd
	{
	public:
		static std::vector<int> get_cpu_tempertures();
	};
}

#endif //BSD_H