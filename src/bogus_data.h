#ifndef BOGUS_DATA_H
#define BOGUS_DATA_H

#include <stdint.h>
// Battery specifications
#define BATTERY_NOMINAL_CAPACITY 3000  // mAh
#define BATTERY_RATED_CYCLES     500   // cycles
#define BATTERY_MAX_CYCLES       600   // cycles for end-of-life

// Number of data points in the lookup tables
#define BATTERY_HEALTH_TABLE_SIZE 21
#define PROFILE_SIZE (50)

float interpolate_battery_parameter(uint16_t cycle_count, const void* table, uint8_t is_uint16);
float get_battery_health(uint16_t cycle_count);
float get_capacity_retention(uint16_t cycle_count);
float get_power_capability(uint16_t cycle_count);
float get_self_discharge_rate(uint16_t cycle_count);
uint16_t get_estimated_capacity(uint16_t cycle_count);
float get_self_discharge_rate(uint16_t cycle_count);



#endif // 