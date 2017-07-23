#include "utils/encoderinterrupts.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <Arduino.h>

static volatile int32_t encoderCounts[2];
static volatile uint32_t encoderDebounceFwd[2];
static volatile uint32_t encoderDebounceRev[2];

int32_t getEncoderCount(uint8_t id) {
	int32_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = encoderCounts[id];
	}
	return count;
}

void setEncoderCount(uint8_t id, int32_t value) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		encoderCounts[id] = value;
	}
}

void enableEncoderInterrupts() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
		EIMSK |= (1 << INT0) | (1 << INT1); 
	}
}

#define ENCODER_WAIT_TIME 3
#define ENCODER_0_DIR_READ() ((PIND >> 4) & 0x1)
#define ENCODER_1_DIR_READ() ((PIND >> 5) & 0x1)

ISR(INT0_vect) {
	unsigned long now = 0;//millis();
	if (ENCODER_0_DIR_READ()) {
		if (now - encoderDebounceFwd[0] >= ENCODER_WAIT_TIME) {
			encoderCounts[0]++;
			encoderDebounceFwd[0] = now;
		}
	}
	else {
		if (now - encoderDebounceRev[0] >= ENCODER_WAIT_TIME) {
			encoderCounts[0]--;
			encoderDebounceRev[0] = now;
		}
	}
}

ISR(INT1_vect) {
	unsigned long now = 0;//millis();
	if (ENCODER_1_DIR_READ()) {
		if (now - encoderDebounceFwd[1] >= ENCODER_WAIT_TIME) {
			encoderCounts[1]++;
			encoderDebounceFwd[1] = now;
		}
	}
	else {
		if (now - encoderDebounceRev[1] >= ENCODER_WAIT_TIME) {
			encoderCounts[1]--;
			encoderDebounceRev[1] = now;
		}
	}
}
