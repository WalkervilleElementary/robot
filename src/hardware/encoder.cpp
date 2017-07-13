#include "hardware/encoder.h"
#include "avr/interrupt.h"

namespace hardware
{

  volatile unsigned int counts[2];
  volatile unsigned long prevTime[2];
  const unsigned long WAITTIME = ENCODER_WAIT_TIME();

Encoder::Encoder()
{
}

void Encoder::enableExternalInterrupt(unsigned int INTX, unsigned int mode)
{
  if (INTX > 3 || mode > 3 || mode == 1) return;
  cli();
  /* Allow pin to trigger interrupts        */
  EIMSK |= (1 << INTX);
  /* Clear the interrupt configuration bits */
  EICRA &= ~(1 << (INTX*2+0));
  EICRA &= ~(1 << (INTX*2+1));
  /* Set new interrupt configuration bits   */
  EICRA |= mode << (INTX*2);
  sei();
}

void Encoder::disableExternalInterrupt(unsigned int INTX)
{
  if (INTX > 3) return;
  EIMSK &= ~(1 << INTX);
}

void Encoder::start(unsigned int INTX)
{
  counts[INTX] = 0;
  enableExternalInterrupt(INTX, RISING);
}

unsigned int Encoder::stop(unsigned int INTX)
{
  disableExternalInterrupt(INTX);
  return counts[INTX];
}

unsigned int Encoder::get(unsigned int INTX)
{
  return counts[INTX];
}

}  // namespace hardware

ISR(INT0_vect)
{
  if (millis() > hardware::prevTime[0] + hardware::WAITTIME)
  {
    hardware::counts[0]++;
    hardware::prevTime[0] = millis();
  }
};

ISR(INT1_vect)
{
  if (millis() > hardware::prevTime[1] + hardware::WAITTIME)
  {
    hardware::counts[1]++;
    hardware::prevTime[1] = millis();
  }
};
