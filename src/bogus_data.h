#ifndef BOGUS_DATA_H
#define BOGUS_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
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
uint16_t get_estimated_capacity(uint16_t cycle_count);
float get_self_discharge_rate(uint16_t cycle_count);
float calculate_health(uint16_t cycle_count);
float calculate_capacity_retention(uint16_t cycle_count);
float calculate_power_capability(uint16_t cycle_count);
uint16_t calculate_estimated_capacity(uint16_t cycle_count);
float calculate_self_discharge_rate(uint16_t cycle_count);


// Voltage profile (volts)
const float battery_discharge_voltage[PROFILE_SIZE] = {
    4.200, 4.159, 4.118, 4.078, 4.037, 3.999, 3.984, 3.969, 3.955, 3.940,
    3.926, 3.911, 3.897, 3.882, 3.867, 3.853, 3.838, 3.824, 3.809, 3.794,
    3.780, 3.765, 3.751, 3.736, 3.722, 3.707, 3.692, 3.678, 3.663, 3.649,
    3.634, 3.620, 3.605, 3.590, 3.576, 3.561, 3.547, 3.532, 3.517, 3.503,
    3.459, 3.408, 3.357, 3.306, 3.255, 3.204, 3.153, 3.102, 3.051, 3.000
};

const float battery_discharge_resistance[PROFILE_SIZE] = {
    0.112837, 0.111122, 0.112412, 0.107232, 0.107879, 0.081417, 0.080817, 0.080592, 0.081016, 0.082678,
    0.082797, 0.081995, 0.080354, 0.079368, 0.080095, 0.080654, 0.081905, 0.080130, 0.079215, 0.078712,
    0.079582, 0.081612, 0.080953, 0.079036, 0.078944, 0.078962, 0.081209, 0.080207, 0.080817, 0.080483,
    0.081146, 0.079311, 0.081759, 0.080766, 0.079584, 0.081444, 0.082251, 0.081286, 0.080724, 0.080746,
    0.081864, 0.081966, 0.163747, 0.163930, 0.167276, 0.175105, 0.176446, 0.185830, 0.187018, 0.191340
};

// Current profile (amperes)
const float battery_discharge_current[PROFILE_SIZE] = {
    0.980, 0.981, 0.980, 1.007, 1.003, 1.008, 0.992, 1.025, 0.998, 0.998,
    1.015, 1.012, 1.008, 1.016, 0.998, 1.023, 0.984, 1.023, 1.024, 0.995,
    0.981, 0.985, 0.983, 0.977, 0.998, 0.988, 0.999, 1.022, 1.020, 0.986,
    1.011, 1.010, 1.018, 1.017, 1.007, 0.984, 0.995, 0.988, 1.000, 0.977,
    0.983, 1.011, 0.933, 0.751, 0.583, 0.431, 0.298, 0.185, 0.098, 0.050
};

// Temperature profile (degrees Celsius)
const float battery_discharge_temperature[PROFILE_SIZE] = {
    26.4, 27.2, 27.6, 28.3, 28.4, 28.7, 28.5, 28.8, 29.0, 28.9,
    29.0, 28.9, 28.8, 29.0, 28.8, 29.1, 28.9, 28.8, 29.1, 29.1,
    28.7, 29.1, 28.8, 28.7, 28.8, 28.9, 28.7, 28.8, 28.7, 29.0,
    28.8, 28.9, 29.0, 28.8, 29.0, 28.8, 29.0, 28.9, 28.9, 28.9,
    28.7, 28.9, 28.6, 28.7, 28.0, 27.5, 27.0, 26.5, 26.2, 25.9
};

// Voltage profile (volts)
const float battery_charge_voltage[PROFILE_SIZE] = {
    3.000, 3.021, 3.041, 3.062, 3.084, 3.105, 3.127, 3.149, 3.172, 3.194,
    3.217, 3.240, 3.264, 3.287, 3.311, 3.335, 3.360, 3.385, 3.410, 3.435,
    3.460, 3.486, 3.512, 3.538, 3.565, 3.592, 3.619, 3.646, 3.673, 3.701,
    3.729, 3.758, 3.786, 3.815, 3.844, 3.874, 3.903, 3.933, 3.963, 3.994,
    4.200, 4.200, 4.200, 4.200, 4.200, 4.200, 4.200, 4.200, 4.200, 4.200
};

const float battery_charge_resistance[PROFILE_SIZE] = {
    0.217846, 0.213855, 0.205922, 0.201804, 0.197922, 0.192292, 0.187141, 0.183688, 0.092681, 0.092988,
    0.092210, 0.089467, 0.092177, 0.091119, 0.089691, 0.091575, 0.090718, 0.090978, 0.091143, 0.089049,
    0.091385, 0.090281, 0.088909, 0.088444, 0.089780, 0.090324, 0.091312, 0.090590, 0.090130, 0.091370,
    0.088365, 0.091661, 0.091530, 0.090307, 0.090761, 0.090206, 0.090932, 0.091787, 0.091278, 0.092426,
    0.092203, 0.092213, 0.093840, 0.098941, 0.101859, 0.134234, 0.140832, 0.147433, 0.156679, 0.167193
};

// Current profile (amperes)
const float battery_charge_current[PROFILE_SIZE] = {
    0.990, 1.001, 1.005, 1.016, 0.989, 1.010, 0.979, 1.010, 1.023, 0.995,
    0.987, 0.999, 1.015, 0.989, 0.984, 0.993, 1.008, 0.984, 1.012, 1.017,
    0.982, 1.014, 1.009, 0.998, 1.003, 0.982, 0.979, 1.012, 1.020, 0.997,
    1.006, 1.008, 0.979, 0.983, 0.980, 0.997, 0.989, 0.985, 1.000, 1.004,
    0.783, 0.576, 0.424, 0.312, 0.230, 0.169, 0.125, 0.092, 0.068, 0.050
};

// Temperature profile (degrees Celsius)
const float battery_charge_temperature[PROFILE_SIZE] = {
    25.0, 25.4, 25.6, 26.2, 26.4, 27.0, 26.9, 27.5, 27.8, 28.0,
    28.1, 28.2, 28.7, 28.8, 28.7, 28.7, 28.9, 29.1, 28.9, 28.8,
    28.8, 28.7, 28.6, 28.6, 28.2, 28.1, 28.0, 27.6, 27.2, 27.0,
    26.6, 26.4, 25.7, 25.4, 25.1, 28.9, 28.6, 28.2, 28.0, 27.7,
    27.3, 27.2, 26.8, 26.6, 26.5, 25.9, 25.7, 25.5, 25.3, 24.9
};

// Cycle count reference points (0 to 600 cycles in steps of 30)
const uint16_t battery_cycle_reference[BATTERY_HEALTH_TABLE_SIZE] = {
    0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 
    330, 360, 390, 420, 450, 480, 510, 540, 570, 600
};

// Overall battery health (%)
// This is a weighted indicator of overall battery condition
const float battery_health_table[BATTERY_HEALTH_TABLE_SIZE] = {
    100.6, 97.7, 96.4, 95.9, 93.2, 89.3, 89.7, 86.4, 87.0, 82.7, 82.9,
    80.6, 79.1, 77.1, 73.5, 74.3, 71.4, 63.3, 46.9, 27.7, 10.0
};

// Capacity retention (%)
// How much of the original capacity remains
const float battery_capacity_retention_table[BATTERY_HEALTH_TABLE_SIZE] = {
    100.4, 99.8, 96.2, 96.1, 94.9, 92.1, 92.4, 91.5, 89.4, 88.2, 88.2,
    87.6, 86.4, 83.3, 82.0, 80.6, 79.9, 75.3, 59.2, 44.9, 30.1
};

// Power capability (%)
// The battery's ability to deliver peak power - decreases with aging
const float battery_power_capability_table[BATTERY_HEALTH_TABLE_SIZE] = {
    100.5, 97.7, 96.5, 97.7, 95.2, 94.3, 94.9, 96.1, 94.1, 92.8, 90.3,
    91.9, 90.6, 90.3, 89.5, 88.9, 87.2, 81.6, 70.9, 59.5, 46.7
};

// Estimated capacity (mAh)
// Actual capacity based on nominal capacity and retention factor
const uint16_t battery_estimated_capacity_table[BATTERY_HEALTH_TABLE_SIZE] = {
    3012, 2994, 2886, 2883, 2847, 2763, 2772, 2745, 2682, 2646, 2646,
    2628, 2592, 2499, 2460, 2418, 2397, 2259, 1776, 1347, 903
};

// Self-discharge rate (%/month)
// Rate at which battery loses charge when not in use - increases with age
const float battery_self_discharge_table[BATTERY_HEALTH_TABLE_SIZE] = {
    2.01, 2.04, 2.00, 2.10, 2.16, 2.13, 2.21, 2.31, 2.31, 2.28, 2.37,
    2.59, 2.52, 2.68, 2.92, 2.89, 3.12, 3.42, 4.38, 5.42, 6.43
};

#ifdef __cplusplus
}
#endif

#endif // 