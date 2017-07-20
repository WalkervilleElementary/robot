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
  return detect_10khz(L_BEACON_SENSOR_);
}

uint32_t Beacon::rightIntensity() const {
  return detect_10khz(R_BEACON_SENSOR_);
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
