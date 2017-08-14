#pragma once

#include "configs.h"
#include "stdint.h"

namespace hardware {

class BeaconWatcher {
public:
  BeaconWatcher(uint8_t leftInput, uint8_t rightInput);
  unsigned long gateOpenedTime();
  bool gateOpened();
  bool safeToProceed();
  void tick();
private:
  enum BeaconSignal {SIGNAL_UNKNOWN, SIGNAL_1K, SIGNAL_10K};
  BeaconSignal threshold(int32_t intensity1, int32_t intensity10);
  //BeaconSignal m_leftSignal;
  //BeaconSignal m_rightSignal;
  bool m_leftSide;
  uint8_t m_leftInput;
  uint8_t m_rightInput;
  uint8_t m_debounceCounter;
  int32_t m_left10;
  int32_t m_left1;
  int32_t m_right10;
  int32_t m_right1;
  BeaconSignal m_currentSignal;
  BeaconSignal m_debouncedSignal;
  unsigned long m_openedTime;

  const int UNKNOWN_THRESHOLD;
  const int GATE_TIME;
};

}