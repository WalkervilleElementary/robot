#include "configs.h"

#include <phys253.h>
#include "utils/filter.h"
#include "hardware/beacon.h"

const unsigned char PS_2 = (1 << ADPS0);
const unsigned char PS_4 = (1 << ADPS1);
const unsigned char PS_8 = (1 << ADPS1) | (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

namespace hardware
{
    
Beacon::Beacon()
{
}

uint32_t Beacon::leftIntensity() const {
  uint8_t samples[num_samples_];
  uint8_t old_ADCSRA = ADCSRA;
  ADCSRA &= ~PS_128;
  ADCSRA |= PS_16;
  sample_signal(L_BEACON_SENSOR_, num_samples_, samples);
  uint32_t left_intensity = detect_frequency(10000, sample_rate_, num_samples_, samples);
  ADCSRA = old_ADCSRA;
  return left_intensity;
}

uint32_t Beacon::rightIntensity() const {
  uint8_t samples[num_samples_];
  uint8_t old_ADCSRA = ADCSRA;
  ADCSRA &= ~PS_128;
  ADCSRA |= PS_16;
  sample_signal(R_BEACON_SENSOR_, num_samples_, samples);
  uint32_t right_intensity = detect_frequency(10000, sample_rate_, num_samples_, samples);
  ADCSRA = old_ADCSRA;
  return right_intensity;
}

#if USE_UPDATE()
void Beacon::update()
{  
  /*Serial.println(leftIntensity());
  LCD.setCursor(0,0);
  LCD.print(leftIntensity());
  LCD.print(' ');
  LCD.print(rightIntensity());
  LCD.print("   ");
  LCD.setCursor(0,1);
  LCD.print(analogRead(0));
  LCD.print("   ");*/
}
#endif  // USE_UPDATE()

}  // namespace hardware
