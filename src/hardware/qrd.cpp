#include "configs.h"

#include "hardware/qrd.h"

#if DEBUG()
#include <LiquidCrystal.h>
#endif  // DEBUG()
#include <phys253.h>

namespace hardware{

int Qrd::tape_threshold_ = TAPE_QRD_STRENGTH_THRESHOLD();
int8_t Qrd::error_ = 0;
bool Qrd::invalid_ = false;

const int Qrd::error_array_[] = {-4, -3, -1, -2, 1, -6, 0, -6, 3, -5, -5, -5, 2, -5, -6, -7};

int8_t Qrd::getTapeError(){
  const int ll_val = analogRead(LL_QRD_SENSOR_);
  const int lc_val = analogRead(LC_QRD_SENSOR_);
  const int rc_val = analogRead(RC_QRD_SENSOR_);
  const int rr_val = analogRead(RR_QRD_SENSOR_);

#if DEBUG()
  LCD.home();
  LCD.setCursor(0,0);  LCD.print(ll_val);
  LCD.setCursor(4,0);  LCD.print(lc_val);
  LCD.setCursor(8,0); LCD.print(rc_val);
  LCD.setCursor(12,0); LCD.print(rr_val);
#endif  // DEBUG()

  const bool ll_on = ll_val > tape_threshold_;
  const bool lc_on = lc_val > tape_threshold_;
  const bool rc_on = rc_val > tape_threshold_;
  const bool rr_on = rr_val > tape_threshold_;

  // ll lc rc rr index value
  // 1  1  1  1    15     -7
  // 1  1  1  0    14     -6
  // 1  1  0  1    13     -5
  // 1  1  0  0    12     2
  // 1  0  1  1    11     -5
  // 1  0  1  0    10     -5
  // 1  0  0  1    9      -5
  // 1  0  0  0    8      3
  // 0  1  1  1    7      -6
  // 0  1  1  0    6      0
  // 0  1  0  1    5      -6
  // 0  1  0  0    4      1
  // 0  0  1  1    3      -2
  // 0  0  1  0    2      -1
  // 0  0  0  1    1      -3
  // 0  0  0  0    0      -4

  int val = 0;
  if (ll_on) val |= 8;
  if (lc_on) val |= 4;
  if (rc_on) val |= 2;
  if (rr_on) val |= 1;

  switch (error_array_[val])
  {
    case -3:  // tracking tape
    case -2:
    case -1:
    case 0:
    case 1:
    case 2:
    case 3:
      error_ = error_array_[val];
      break;
    case -4:  // lost tape
      error_ = (error_ > 0) ? 5 : -5;
      break;
    case -5:  // invalid states
      return 0;
    case -6:
    case -7:
      invalid_ = true;
      return 0;
  }

  return error_;
}

bool Qrd::isIntersection(){
  if (invalid_)
  {
    invalid_ = false;
    return true;
  }
  return false;
}

#if USE_UPDATE()
void Qrd::update(){
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
      tape_threshold_ += (end_val - start_val)/10;
      LCD.clear();  LCD.home() ;
      LCD.setCursor(0,0); LCD.print("threshold");
      LCD.setCursor(0,1); LCD.print(tape_threshold_);
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace hardware
