#ifndef IT8728F_H
#define IT8728F_H

#include <stdio.h>
#include <vector>

namespace shadowgrid { 

	class Sensor{
	public:
		static int checkChip();
		static std::vector<int> getSystemTempertures();
		static std::vector<int> getFanSpeeds();
		static std::vector<int> getVoltages();
	};

}


#endif //IT8728F_H