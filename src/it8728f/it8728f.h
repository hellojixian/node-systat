#ifndef IT8728F_H
#define IT8728F_H

#include <iostream>
#include <vector>

#ifdef __cplusplus
extern "C"{ 
#endif

static std::vector<int> get_system_temp();
static std::vector<int> get_fan_speed();
static std::vector<int> get_voltages();

#ifdef __cplusplus
}
#endif

#endif //IT8728F_H