#ifndef BSD_H
#define BSD_H

#include <stdio.h>
#include <vector>

namespace shadowgrid 
{
	class System
	{
	public:
		static std::vector<int> getCPUTempertures();
	};
}

#endif //BSD_H