#ifndef IT8728F_H
#define IT8728F_H

#include <node.h>


namespace shadowgrid { 

	class sensor{
	public:
		static int check_chip();
		static std::vector<int> get_system_tempertures();
		static std::vector<int> get_fan_speeds();
		static std::vector<int> get_voltages();
	};

}


#endif //IT8728F_H