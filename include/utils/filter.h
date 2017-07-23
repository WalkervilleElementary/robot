#ifndef WALKERVILLE_ROBOT_UTILS_FILTER_H
#define WALKERVILLE_ROBOT_UTILS_FILTER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Samples an analog signal from a pin into a buffer, at 77 KHz and 8 bits resolution
 * in: analog pin number (0-7 on TINAH)
 * num_samples: number of samples to take
 * out: pointer to memory of at least num_samples bytes
 */
void sample_signal(uint8_t pin, size_t num_samples, uint8_t* out);

/**
 * Analyzes samples and returns the magnitude of a frequency component
 * detection_frequency: in Hz
 * sample_rate: in Hz
 * num_samples: number of samples to analyze.
 * samples: array of samples
 * return value is in arbitrary units, should be linear with amplitude
 */
uint32_t detect_frequency(uint8_t* samples, size_t num_samples, uint32_t detection_frequency, uint32_t sample_rate);

/**
 * Convenience function to detect a 10 kHz signal on an input pin
 */
uint32_t detect_10khz(uint8_t analog_pin);

/**
 * Convenience function to detect a 10 kHz signal on an input pin
 */
uint32_t detect_1khz(uint8_t analog_pin);

#ifdef __cplusplus
}
#endif

#endif // WALKERVILLE_ROBOT_UTILS_FILTER_H
