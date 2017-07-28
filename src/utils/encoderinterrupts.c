#include "utils/encoderinterrupts.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile int32_t encoderCounts[2];
static volatile uint32_t encoderDebounceFwd[2];
static volatile uint32_t encoderDebounceRev[2];

//Only use pins on Port D!
#define ENCODER_0_INT 0
#define ENCODER_0_DIR 2
#define ENCODER_1_INT 1
#define ENCODER_1_DIR 3

#define ENCODER_0_DIR_READ() ((PIND >> ENCODER_0_DIR) & 0x1)
#define ENCODER_1_DIR_READ() ((PIND >> ENCODER_1_DIR) & 0x1)

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
	enableEncoderInterrupt(ENCODER_0_INT);
  enableEncoderInterrupt(ENCODER_1_INT);
}

void enableEncoderInterrupt(uint8_t encoderId) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		EIMSK |= (1 << encoderId);
    EICRA |= (0b11 << (2 * encoderId)); 
	}
}

ISR(_VECTOR(ENCODER_0_INT)) {
	unsigned long now = millis();
	if (ENCODER_0_DIR_READ()) {
		//if (now - encoderDebounceFwd[0] >= ENCODER_WAIT_TIME) {
			encoderCounts[0]++;
			encoderDebounceFwd[0] = now;
		//}
	}
	else {
		//if (now - encoderDebounceRev[0] >= ENCODER_WAIT_TIME) {
			encoderCounts[0]--;
			encoderDebounceRev[0] = now;
		//}
	}
}

ISR(_VECTOR(ENCODER_1_INT)) {
	unsigned long now = millis();
	if (ENCODER_1_DIR_READ()) {
		//if (now - encoderDebounceFwd[1] >= ENCODER_WAIT_TIME) {
			encoderCounts[1]++;
			encoderDebounceFwd[1] = now;
		//}
	}
	else {
		//if (now - encoderDebounceRev[1] >= ENCODER_WAIT_TIME) {
			encoderCounts[1]--;
			encoderDebounceRev[1] = now;
		//}
	}
}
