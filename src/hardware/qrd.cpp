#include "configs.h"

#include "hardware/qrd.h"

#if DEBUG()
#include <LiquidCrystal.h>
#endif  // DEBUG()
#include <phys253.h>

namespace hardware{

int Qrd::tape_threshold_ = TAPE_QRD_STRENGTH_THRESHOLD();
int8_t Qrd::error_ = 0;

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

  // ll lc rc rr val
  // x  o  o  o   3
  // x  x  o  o   2
  // o  x  o  o   1
  // o  x  x  o   0
  // o  o  x  o  -1
  // o  o  x  x  -2
  // o  o  o  x  -3
  // o  o  o  o  +/-5

  if (lc_on){
    if (rc_on) error_ = 0;
    else if (ll_on) error_ = 2;
    else error_ = 1;
  }
  else if (rc_on){
    if (rr_on) error_ = -2;
    else error_ = -1;
  }
  else if (ll_on){
    error_ = 3;
  }
  else if (rr_on){
    error_ = -3;
  }
  else{
    error_ = (error_ > 0)? 5 : -5;
  }

  return error_;
}

bool Qrd::isIntersection(){
  const int ll_val = analogRead(LL_QRD_SENSOR_);
  const int lc_val = analogRead(LC_QRD_SENSOR_);
  const int rc_val = analogRead(RC_QRD_SENSOR_);
  const int rr_val = analogRead(RR_QRD_SENSOR_);

  return ll_val > tape_threshold_ && lc_val > tape_threshold_ &&
         rc_val > tape_threshold_ && rr_val > tape_threshold_;
}

#if USE_UPDATE()
void Qrd::update(){
  delay(1000);
  while(startbutton());
  while(!startbutton()){
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    tape_threshold_ += (end_val - start_val)/10;
    LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("threshold");
    LCD.setCursor(0,1); LCD.print(tape_threshold_);
  }
}
#endif  // USE_UPDATE()

}  // namespace hardware
