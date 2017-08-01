#include "utils/encoderinterrupts.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile int32_t encoderCounts[2];
static volatile uint8_t encoderPrevState[2];

#define ENCODER_INT 0

#define ENCODER_0_A_PIN 0
#define ENCODER_0_B_PIN 1
#define ENCODER_1_A_PIN 2
#define ENCODER_1_B_PIN 3

#define ENCODER_READ(pin) ((PIND >> pin) & 0b1)

#define ENCODER_0_READ() ((ENCODER_READ(ENCODER_0_B_PIN) << 1) | ENCODER_READ(ENCODER_0_A_PIN))
#define ENCODER_1_READ() ((ENCODER_READ(ENCODER_1_B_PIN) << 1) | ENCODER_READ(ENCODER_1_A_PIN))

int32_t getEncoderCount(uint8_t id) {
	int32_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		count = encoderCounts[id];
	}
	return count;
}

uint8_t getEncoderPin(uint8_t id, uint8_t pin) {
	return ENCODER_READ(id * 2 + pin);
}

void setEncoderCount(uint8_t id, int32_t value) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		encoderCounts[id] = value;
	}
}

void setInterruptMode(uint8_t pin, uint8_t rising) {
		if (rising) EICRA |= (1 << (pin * 2));
		else EICRA &= ~(1 << (pin * 2));
}

void toggleInterruptMode(uint8_t pin) {
		EICRA ^= (1 << (pin * 2));
}

void enableEncoderInterrupt(uint8_t pin) {
		EIMSK |= (1 << pin);
		EICRA |= (0b10 << (2 * pin));
		setInterruptMode(pin, !ENCODER_READ(pin));
}

void enableEncoderInterrupts() {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		//EIMSK |= (1 << 0);
		//EICRA |= (0b10 << (2 * 0));
		enableEncoderInterrupt(ENCODER_0_A_PIN);
		enableEncoderInterrupt(ENCODER_0_B_PIN);
		enableEncoderInterrupt(ENCODER_1_A_PIN);
		enableEncoderInterrupt(ENCODER_1_B_PIN);
	}
}

const int8_t table[16] = {
	0, 1, -1, 2,
	-1, 0, 2, 1,
	1, 2, 0, -1,
	2, -1, 1, 0
};

static inline void updateEncoders(void) {
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

/*ISR(INT0_vect) {
	updateEncoders();
}*/

ISR(INT0_vect) {
	toggleInterruptMode(0);
	updateEncoders();
}

ISR(INT1_vect) {
	toggleInterruptMode(1);
	updateEncoders();

}

ISR(INT2_vect) {
	toggleInterruptMode(2);
	updateEncoders();
}

ISR(INT3_vect) {
	toggleInterruptMode(3);
	updateEncoders();
}
