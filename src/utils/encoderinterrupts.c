#include "utils/encoderinterrupts.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile int32_t encoderCounts[2];
static volatile uint8_t encoderPrevState[2];

#define ENCODER_INT 0

#define ENCODER_0_READ() ((PIND >> 4) & 0b11)
#define ENCODER_1_READ() ((PIND >> 6) & 0b11)

int32_t getEncoderCount(uint8_t id) {
	int32_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = encoderCounts[id];
	}
	return count;
}

uint8_t getEncoderState(uint8_t id, uint8_t pin) {
  uint8_t state = id ? ENCODER_1_READ() : ENCODER_0_READ();
  return (state >> pin) & 0b01;
}

void setEncoderCount(uint8_t id, int32_t value) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		encoderCounts[id] = value;
	}
}

void enableEncoderInterrupts() {
	enableEncoderInterrupt(ENCODER_INT);
}

void enableEncoderInterrupt(uint8_t pin) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		EIMSK |= (1 << pin);
    EICRA |= (0b11 << (2 * pin)); 
	}
}

const int8_t table[16] = {
	0, 1, -1, 2,
	-1, 0, 2, 1,
	1, 2, 0, -1,
	2, -1, 1, 0
};

ISR(INT0_vect) {
  uint8_t input0 = ENCODER_0_READ();
  uint8_t input1 = ENCODER_1_READ();
  int8_t delta0 = table[input0 | (encoderPrevState[0] << 2)];
  int8_t delta1 = table[input1 | (encoderPrevState[1] << 2)];
  if (delta0 != 2) {
    encoderCounts[0] += delta0;
    encoderPrevState[0] = input0;
  }
  if (delta1 != 2) {
    encoderCounts[1] += delta1;
    encoderPrevState[1] = input1;
  }
  
}
