#pragma once
#include <stdint.h>

namespace hardware {

class FrequencySensor {
public:
	FrequencySensor(uint8_t analogPin);

	uint32_t intensity10khz() const;
	uint32_t intensity1khz() const;

private:
	uint8_t m_analogPin;
};

}
