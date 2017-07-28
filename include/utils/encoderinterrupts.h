#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t getEncoderCount(uint8_t id);
uint8_t getEncoderState(uint8_t id, uint8_t pin);
void setEncoderCount(uint8_t id, int32_t value);
void enableEncoderInterrupts(void);

#ifdef __cplusplus
}
#endif