#include "hardware/frequencysensor.h"
#include "utils/filter.h"

namespace hardware {
FrequencySensor::FrequencySensor(uint8_t analogPin) {
  m_analogPin = analogPin;
}

uint32_t FrequencySensor::intensity10khz() const {
  return detect_10khz(m_analogPin);
}

uint32_t FrequencySensor::intensity1khz() const {
  return detect_1khz(m_analogPin);
}

}
