#include "hardware/beaconwatcher.h"
#include "utils/filter.h"
#include <Arduino.h>

namespace hardware {

BeaconWatcher::BeaconWatcher(uint8_t leftInput, uint8_t rightInput) :
  UNKNOWN_THRESHOLD(100), GATE_TIME(3500)
{
  m_leftInput = leftInput;
  m_rightInput = rightInput;
  m_leftSide = true;
  m_openedTime = 0;
  m_left10 = 0;
  m_left1 = 0;
  m_right10 = 0;
  m_right1 = 0;
}

BeaconWatcher::BeaconSignal BeaconWatcher::threshold(int32_t intensity1, int32_t intensity10) {
  if  (intensity10 > intensity1 && intensity10 >= UNKNOWN_THRESHOLD) {
    return SIGNAL_10K;
  }
  else if (intensity1 > intensity10 && intensity1 >= UNKNOWN_THRESHOLD) {
    return SIGNAL_1K;
  }
  else {
    return SIGNAL_UNKNOWN;
  }
}

bool BeaconWatcher::gateOpened() {
  return m_debouncedSignal == SIGNAL_1K;
}

unsigned long BeaconWatcher::gateOpenedTime() {
  return m_openedTime;
}

bool BeaconWatcher::safeToProceed() {
  return millis() - m_openedTime < GATE_TIME;
}

void BeaconWatcher::tick() {
  unsigned long startTime = micros();
  uint8_t samples[154];
  int32_t intensity10, intensity1;

  if (m_leftSide) {
    sample_signal(m_leftInput, 154, samples);
    m_left10 = detect_frequency(samples, 77, 10000, 77000);
    m_left1 = detect_frequency(samples, 154, 1000, 77000);
    m_leftSide = false;
  }
  else {
    sample_signal(m_rightInput, 154, samples);
    m_right10 = detect_frequency(samples, 77, 10000, 77000);
    m_right1 = detect_frequency(samples, 154, 1000, 77000);
    m_leftSide = true;
  }

  BeaconSignal result = threshold(m_left1 + m_right1, m_left10 + m_right10);
  if (result != m_currentSignal) {
    m_debounceCounter = 10;
  }
  else if (m_debounceCounter > 0) {
    m_debounceCounter--;
  }
  m_currentSignal = result;
  if (m_debounceCounter == 0) {
    if (m_currentSignal == SIGNAL_1K && m_debouncedSignal == SIGNAL_10K) {
      m_openedTime = millis();
    }
    m_debouncedSignal = m_currentSignal;
  }
#if DEBUG()
  Serial.print(micros() - startTime);
  Serial.print(',');
  Serial.print(millis() - m_openedTime) ;
  Serial.print(',');
  Serial.print(m_currentSignal);
  Serial.print(',');
  Serial.print('\t');
  Serial.print(m_leftSignal);
  Serial.print(',');
  Serial.print('\t');
  Serial.print(m_rightSignal);
  Serial.println();
#endif  // DEBUG()
}

}