#include "hardware/linesensor.h"
#include <Arduino.h>

namespace hardware {

LineSensor::LineSensor(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3) {
	m_numSensors = 4;
	m_sensorPins[0] = pin0;
	m_sensorPins[1] = pin1;
	m_sensorPins[2] = pin2;
	m_sensorPins[3] = pin3;
}

int16_t LineSensor::sensorPosition(uint8_t index) const {
	int8_t fromLeft = index * 2;
	return 512 * (fromLeft - ((int8_t)m_numSensors - 1));
}

int32_t LineSensor::getLinePosition() const {
	int32_t totalWeight = 0;
	int32_t sum = 0;
	for (size_t i = 0; i < m_numSensors; i++) {
		totalWeight += m_samples[i];
		sum += sensorPosition(i) * m_samples[i];
	}
	return sum / totalWeight;
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

int32_t LineSensor::getLineWidth() const {
	int16_t mean = getLinePosition();
	int32_t totalWeight = 0;
	int32_t sum = 0;
	for (size_t i = 0; i < m_numSensors; i++) {
		totalWeight += m_samples[i];
		sum += m_samples[i] * (sensorPosition(i) - mean) * (sensorPosition(i) - mean);
	}
	return uint_sqrt(sum / totalWeight);
}

void LineSensor::tick() {
	for (size_t i = 0; i < m_numSensors; i++) {
		m_samples[i] = analogRead(m_sensorPins[i]);
	}
}

}