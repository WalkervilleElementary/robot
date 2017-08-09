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

namespace hardware {

uint8_t Beacon::uncertainty_ = BEACON_UNCERTAINTY();
uint32_t Beacon::threshold_ = BEACON_THRESHOLD();

unsigned long Beacon::left_update_;
unsigned long Beacon::right_update_;

uint32_t Beacon::left_intensity_;
uint32_t Beacon::right_intensity_;

uint32_t Beacon::leftIntensity() {
  unsigned long current_time = millis();
  if (current_time - left_update_ > update_interval_) {
    left_intensity_ = detect_10khz(L_BEACON_SENSOR_);
    left_update_ = millis();
  }
  return left_intensity_;
}

uint32_t Beacon::rightIntensity() {
  unsigned long current_time = millis();
  if (current_time - right_update_ > update_interval_) {
    right_intensity_ = detect_10khz(R_BEACON_SENSOR_);
    right_update_ = millis();
  }
  return right_intensity_;
}

int8_t Beacon::getTapeError() {
  int left = leftIntensity();
  int right = rightIntensity();
#if DEBUG()
  LCD.setCursor(0,0); LCD.print(left);
  LCD.setCursor(0,1); LCD.print(right);
#endif  // DEBUG()
  if (left + right < threshold_) return -4;
  else if (abs(left - right) < uncertainty_) return 0;
  else if (left > right) return 2;
  else return -2;
}

#if USE_UPDATE()
void Beacon::update() {
  delay(1000);
  while(startbutton());
  while(!startbutton()) {
    int tune_val = knob(7);
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    if (tune_val > TUNE_THRESHOLD()) {
      uncertainty_ += (end_val - start_val)/15;
    }
    LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("uncertainty");
    LCD.setCursor(0,1); LCD.print(uncertainty_);
  }
}
#endif  // USE_UPDATE()

}  // namespace hardware
