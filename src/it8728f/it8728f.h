#ifndef IT8728F_H
#define IT8728F_H

#ifdef __cplusplus
extern "C"{ 
#endif

extern static std::vector<int> get_system_temp();
extern static std::vector<int> get_fan_speed();
extern static std::vector<int> get_voltages();

#ifdef __cplusplus
}
#endif

#endif //IT8728F_H