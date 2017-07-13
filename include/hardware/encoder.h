#ifndef WALKERVILLE_ROBOT_HARDWARE_ENCODER_H
#define WALKERVILLE_ROBOT_HARDWARE_ENCODER_H

#include "configs.h"

#include <phys253.h>
#include <avr/interrupt.h>
#include <LiquidCrystal.h>

namespace hardware
{
  extern volatile unsigned int counts[2];
  extern volatile unsigned long prevTime[2];

class Encoder
{
private:
  void enableExternalInterrupt(unsigned int INTX, unsigned int mode);
  void disableExternalInterrupt(unsigned int INTX);

public:
  Encoder();
  inline ~Encoder(){};

  void start(unsigned int INTX);
  unsigned int stop(unsigned int INTX);
  unsigned int get(unsigned int INTX);

  void loop();
};  // class Encoder
}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_ENCODER_H
