#include "utils/filter.h"
#include <Arduino.h>

const int16_t fixed_sin_table[256] = {
    0,3,6,9,13,16,19,22,25,28,31,34,38,41,44,47,
    50,53,56,59,62,65,68,71,74,77,80,83,86,89,92,95,
    98,101,104,107,109,112,115,118,121,123,126,129,132,134,137,140,
    142,145,147,150,152,155,157,160,162,165,167,170,172,174,177,179,
    181,183,185,188,190,192,194,196,198,200,202,204,206,207,209,211,
    213,215,216,218,220,221,223,224,226,227,229,230,231,233,234,235,
    237,238,239,240,241,242,243,244,245,246,247,248,248,249,250,250,
    251,252,252,253,253,254,254,254,255,255,255,256,256,256,256,256,
    256,256,256,256,256,256,255,255,255,254,254,254,253,253,252,252,
    251,250,250,249,248,248,247,246,245,244,243,242,241,240,239,238,
    237,235,234,233,231,230,229,227,226,224,223,221,220,218,216,215,
    213,211,209,207,206,204,202,200,198,196,194,192,190,188,185,183,
    181,179,177,174,172,170,167,165,162,160,157,155,152,150,147,145,
    142,140,137,134,132,129,126,123,121,118,115,112,109,107,104,101,
    98,95,92,89,86,83,80,77,74,71,68,65,62,59,56,53,
    50,47,44,41,38,34,31,28,25,22,19,16,13,9,6,3
};

int16_t fixed_sin(uint16_t angle) {
    uint8_t index = (angle >> 7) & 0xFF;
    int16_t value = fixed_sin_table[index];
    if (angle & 0x8000) value = -value;
    return value;
}

int16_t fixed_cos(uint16_t angle) {
    return fixed_sin(angle + 0x4000);
}

//https://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
uint32_t uint_sqrt(uint32_t n) {
    uint32_t op  = n;
    uint32_t res = 0;
    uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type

    // "one" starts at the highest power of four <= than the argument.
    while (one > op) {
        one >>= 2;
    }

    while (one != 0) {
        if (op >= res + one) {
            op = op - (res + one);
            res = res +  2 * one;
        }
        res >>= 1;
        one >>= 2;
    }

    /* Do arithmetic rounding to nearest integer */
    if (op > res) {
        res++;
    }
    
    return res;
}

/**
 * Samples an analog signal from a pin into a buffer, at 8 bits resolution.
 * in: analog pin number (0-7 on TINAH)
 * num_samples: number of samples to take
 * out: pointer to memory of at least num_samples bytes
 */
void sample_signal(uint8_t pin, size_t num_samples, uint8_t* out) {
    cli();
    for(size_t i=0; i < num_samples; i++) {
        out[i] = analogRead(pin) >> 2;
    }
    sei();
}

/**
 * Analyzes samples and returns the magnitude of a frequency component
 * detection_frequency: in Hz
 * sample_rate: in Hz
 * num_samples: number of samples to analyze.
 */
uint32_t detect_frequency(uint32_t detection_frequency, uint32_t sample_rate, size_t num_samples, uint8_t* samples) {
    uint16_t theta = 0;
    uint16_t d_theta = (detection_frequency << 16) / sample_rate;
    int32_t real = 0;
    int32_t cplx = 0;
    int32_t signal_sum = 0;
    for (uint16_t n = 0; n < num_samples; n++) {
        signal_sum += samples[n];
    }
    int16_t signal_offset = signal_sum / num_samples;
    for (uint16_t n = 0; n < num_samples; n++) {
        real += ((samples[n] - signal_offset) * fixed_cos(theta));
        cplx += ((samples[n] - signal_offset) * fixed_sin(theta));
        theta += d_theta;
    }
    real = real >> 8;
    cplx = cplx >> 8;
    return uint_sqrt((real * real) + (cplx * cplx)); 
}

