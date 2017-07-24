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

namespace hardware{

int8_t Beacon::beacon_error_ = 0;
int8_t Beacon::uncertainty = BEACON_UNCERTAINTY();

uint32_t Beacon::leftIntensity()  {
  return detect_10khz(L_BEACON_SENSOR_);
}

uint32_t Beacon::rightIntensity()  {
  return detect_10khz(R_BEACON_SENSOR_);
}

int8_t Beacon::getTapeError() {
  // TODO implement this
  LCD.clear(); LCD.home();
  int left_beacon_val = leftIntensity();
  LCD.setCursor(0,0); LCD.print(left_beacon_val);
  int right_beacon_val = rightIntensity();
  LCD.setCursor(0,1); LCD.print(right_beacon_val);
  delay(50);
  if (abs(left_beacon_val - right_beacon_val) < uncertainty){
    beacon_error_ = 0;
  }
  else if (left_beacon_val > right_beacon_val){
    beacon_error_ = 2;
  }
  else{
    beacon_error_ = -2;
  }
   return beacon_error_;
}
#if USE_UPDATE()
void Beacon::update(){
  delay(1000);
  while(startbutton());
  while(!startbutton()){
    int tune_val = knob(7);
    if (tune_val < TUNE_THRESHOLD()){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Tuning Off");
      LCD.setCursor(0,1); LCD.print(tune_val);
      delay(100);
    }
    else{
      int start_val = knob(6);
      delay(100);
      int end_val = knob(6);
      uncertainty += (end_val - start_val)/15;
      LCD.clear();  LCD.home() ;
      LCD.setCursor(0,0); LCD.print("uncertainty");
      LCD.setCursor(0,1); LCD.print(uncertainty);
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace hardware
