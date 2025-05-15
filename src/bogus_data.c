#include "bogus_data.h"

/**
 * Helper function to interpolate health parameters based on cycle count
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @param table The lookup table to interpolate from
 * @param is_uint16 Set to 1 if the table contains uint16_t values, 0 for float
 * @return Interpolated value at the given cycle count
 */
float interpolate_battery_parameter(uint16_t cycle_count, const void* table, uint8_t is_uint16) {
    // Handle out-of-bounds cycle count
    if (cycle_count > BATTERY_MAX_CYCLES) {
        cycle_count = BATTERY_MAX_CYCLES;
    }
    
    // Find the position in the reference table
    uint8_t index = 0;
    while (index < BATTERY_HEALTH_TABLE_SIZE - 1 && 
           battery_cycle_reference[index + 1] < cycle_count) {
        index++;
    }
    
    // If exact match found or at the last entry
    if (battery_cycle_reference[index] == cycle_count || 
        index == BATTERY_HEALTH_TABLE_SIZE - 1) {
        if (is_uint16) {
            return (float)((const uint16_t*)table)[index];
        } else {
            return ((const float*)table)[index];
        }
    }
    
    // Linear interpolation between points
    float x0 = (float)battery_cycle_reference[index];
    float x1 = (float)battery_cycle_reference[index + 1];
    float y0, y1;
    
    if (is_uint16) {
        y0 = (float)((const uint16_t*)table)[index];
        y1 = (float)((const uint16_t*)table)[index + 1];
    } else {
        y0 = ((const float*)table)[index];
        y1 = ((const float*)table)[index + 1];
    }
    
    // Interpolation formula: y = y0 + (y1 - y0) * (x - x0) / (x1 - x0)
    return y0 + (y1 - y0) * ((float)cycle_count - x0) / (x1 - x0);
}

/**
 * Get overall battery health percentage
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Health percentage (0-100)
 */
float get_battery_health(uint16_t cycle_count) {
    return interpolate_battery_parameter(cycle_count, battery_health_table, 0);
}

/**
 * Get capacity retention percentage
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Capacity retention percentage (0-100)
 */
float get_capacity_retention(uint16_t cycle_count) {
    return interpolate_battery_parameter(cycle_count, battery_capacity_retention_table, 0);
}

/**
 * Get power capability percentage
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Power capability percentage (0-100)
 */
float get_power_capability(uint16_t cycle_count) {
    return interpolate_battery_parameter(cycle_count, battery_power_capability_table, 0);
}

/**
 * Get estimated actual capacity in mAh
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Estimated capacity in mAh
 */
uint16_t get_estimated_capacity(uint16_t cycle_count) {
    return (uint16_t)interpolate_battery_parameter(cycle_count, battery_estimated_capacity_table, 1);
}

/**
 * Get self-discharge rate in percent per month
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Self-discharge rate (%/month)
 */
float get_self_discharge_rate(uint16_t cycle_count) {
    return interpolate_battery_parameter(cycle_count, battery_self_discharge_table, 0);
}

/**
 * Alternative calculation method for health parameters
 * These can be used instead of lookup tables for reduced memory usage
 * or for fine-grained cycle count resolution
 */

/**
 * Calculate overall battery health directly
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Health percentage (0-100)
 */
float calculate_health(uint16_t cycle_count) {
    float health;
    
    if (cycle_count <= BATTERY_RATED_CYCLES) {
        // Linear degradation until rated cycles
        health = 100.0f - (cycle_count / (float)BATTERY_RATED_CYCLES) * 30.0f;
    } else {
        // Accelerated degradation beyond rated cycles
        float excess_cycles = cycle_count - BATTERY_RATED_CYCLES;
        float remaining_health = 70.0f; // Health at rated cycle count
        health = remaining_health - (excess_cycles / (BATTERY_MAX_CYCLES - BATTERY_RATED_CYCLES)) * 60.0f;
        // Ensure we don't go below 10%
        if (health < 10.0f) health = 10.0f;
    }
    
    return health;
}

/**
 * Calculate capacity retention directly
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Capacity retention percentage (0-100)
 */
float calculate_capacity_retention(uint16_t cycle_count) {
    float retention;
    
    if (cycle_count <= BATTERY_RATED_CYCLES) {
        // Standard capacity fade: ~20% loss over rated life
        retention = 100.0f - (cycle_count / (float)BATTERY_RATED_CYCLES) * 20.0f;
    } else {
        // Accelerated capacity fade beyond rated cycles
        float excess_cycles = cycle_count - BATTERY_RATED_CYCLES;
        float remaining_capacity = 80.0f; // Capacity at rated cycle count
        retention = remaining_capacity - (excess_cycles / (BATTERY_MAX_CYCLES - BATTERY_RATED_CYCLES)) * 50.0f;
        // Ensure we don't go below 30%
        if (retention < 30.0f) retention = 30.0f;
    }
    
    return retention;
}

/**
 * Calculate power capability directly
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Power capability percentage (0-100)
 */
float calculate_power_capability(uint16_t cycle_count) {
    float cycle_factor;
    float capacity_retention = calculate_capacity_retention(cycle_count);
    
    if (cycle_count <= BATTERY_RATED_CYCLES) {
        cycle_factor = 100.0f - (cycle_count / (float)BATTERY_RATED_CYCLES) * 15.0f;
    } else {
        float excess_cycles = cycle_count - BATTERY_RATED_CYCLES;
        float remaining_power = 85.0f; // Power at rated cycle count
        cycle_factor = remaining_power - (excess_cycles / (BATTERY_MAX_CYCLES - BATTERY_RATED_CYCLES)) * 45.0f;
        if (cycle_factor < 40.0f) cycle_factor = 40.0f;
    }
    
    // Capacity effect on power
    float capacity_factor = 50.0f + capacity_retention * 0.5f;
    
    // Weighted combination
    return 0.7f * cycle_factor + 0.3f * capacity_factor;
}

/**
 * Calculate estimated capacity directly
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Estimated capacity in mAh
 */
uint16_t calculate_estimated_capacity(uint16_t cycle_count) {
    float capacity_retention = calculate_capacity_retention(cycle_count);
    return (uint16_t)((capacity_retention / 100.0f) * BATTERY_NOMINAL_CAPACITY);
}

/**
 * Calculate self-discharge rate directly
 * 
 * @param cycle_count Current number of charge/discharge cycles
 * @return Self-discharge rate (%/month)
 */
float calculate_self_discharge_rate(uint16_t cycle_count) {
    float overall_health = calculate_health(cycle_count);
    
    // Base self-discharge rate for a new battery (% per month)
    const float base_self_discharge = 2.0f;
    
    // Calculate increase based on cycle count and health
    float health_factor = (100.0f - overall_health) / 100.0f;
    float cycle_factor = cycle_count / (float)BATTERY_RATED_CYCLES;
    if (cycle_factor > 1.5f) cycle_factor = 1.5f;
    
    // Combined effect - can increase up to 5x baseline
    return base_self_discharge * (1.0f + 2.0f * health_factor * cycle_factor);
}