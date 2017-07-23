#pragma once
#include <stdint.h>

namespace hardware {

class LineSensor {
public:
	LineSensor(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3);

	int32_t getLinePosition() const;
	int32_t getLineWidth() const;
	void tick();
private:
	int16_t sensorPosition(uint8_t index) const;

	static const uint8_t MAX_SENSORS = 4;

	uint8_t m_sensorPins[MAX_SENSORS];
	uint16_t m_samples[MAX_SENSORS];
	uint16_t m_calibrateMin[MAX_SENSORS];
	uint16_t m_calibrateMax[MAX_SENSORS];
	uint8_t m_numSensors;
};

}