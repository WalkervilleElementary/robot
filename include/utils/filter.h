#ifndef WALKERVILLE_ROBOT_UTILS_FILTER_H
#define WALKERVILLE_ROBOT_UTILS_FILTER_H

#include <stdint.h>
#include <stddef.h>

void sample_signal(uint8_t pin, size_t num_samples, uint8_t* out);
uint32_t detect_frequency(uint32_t detection_frequency, uint32_t sample_rate, size_t num_samples, uint8_t* samples); 

#endif // WALKERVILLE_ROBOT_UTILS_FILTER_H
