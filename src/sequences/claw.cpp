#include "sequences/claw.h"

#include "phys253.h"
#if USE_UPDATE()
#include <LiquidCrystal.h>
#endif  // USE_UPDATE()

namespace sequences{

int8_t Claw::state_ = 0;
uint16_t Claw::degree_ = 0;
bool Claw::left_ = false;

unsigned long Claw::delay_ = 0;
unsigned long Claw::raise_pause = 1000; //ms
unsigned long Claw::grab_pause = 1000;
unsigned long Claw::retrieve_pause = 1000;

uint16_t Claw::left_claw_extended = L_C_EXTEND();
uint16_t Claw::left_claw_rest = L_C_REST();
uint16_t Claw::left_claw_vertical = L_C_VERTICAL();
uint16_t Claw::left_open = L_OPEN();
uint16_t Claw::left_close = L_CLOSE();
int16_t Claw::left_offset[] = {10, 0, -10};
// Right values have not been tested
uint16_t Claw::right_claw_extended = R_C_EXTEND();
uint16_t Claw::right_claw_rest = R_C_REST();
uint16_t Claw::right_claw_vertical = R_C_VERTICAL();
uint16_t Claw::right_open = R_OPEN();
uint16_t Claw::right_close = R_CLOSE();
int16_t Claw::right_offset[] = {10, 0 , -10};

bool Claw::raise(int8_t side){
  if (state_ != 0) return false;
  if (side == LEFT_CLAW)  RCServo0.write(left_claw_vertical);
  else RCServo1.write(right_claw_vertical);
  state_ = 3;
  delay_ = millis() + raise_pause;
  return true;
}

bool Claw::grab(int8_t side, int8_t offset){
  if (state_ != 0) return false;
  state_ = 2;
  if (side == LEFT_CLAW){
    RCServo0.write(left_claw_extended + left_offset[offset]);
    degree_ = left_close;
    left_ = true;
  }else{
    RCServo1.write(right_claw_extended + right_offset[offset]);
    degree_ = right_close;
    left_ = false;
  }
  delay_ = millis() + grab_pause;
  return true;
}

bool Claw::release(int8_t side){
  if (state_ != 0) return false;
  state_ = 1;
  if (side == LEFT_CLAW){
    RCServo0.write(left_claw_rest);
    degree_ = left_open;
    left_ = true;
  }else{
    RCServo1.write(right_claw_rest);
    degree_ = right_open;
    left_ = false;
  }
  delay_ = millis() + retrieve_pause;
  return true;
}

bool Claw::loop(){
  // State description          notes
  // 0     doing nothing state
  // 1     release              wait for a period of time then activate claw
  // 2     grab                 same as release
  // 3     raise                wait for a period of time for arm then return complete
  // 4     claw_wat             wait for a period of time for claw. Uses same the same code as raise

  switch(state_){
    case 1:
    case 2:
      if (millis() > delay_){
        if (left_) RCServo2.write(degree_);
        else RCServo3.write(degree_);
        state_ = 4;
        delay_ = millis() + 1000;
      }else{
        break;
      }
    case 3:
    case 4:
      if (millis() < delay_) break;
      state_ = 0;
    case 0:
      return true;
    default:
      // TODO throw error
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("CLAW DOING SOMETHING WEIRD");
      delay(1000);
  }
  return false;
}

#if USE_UPDATE()
bool Claw::update(){
  int update_state_ = 0;
  while (!startbutton()){
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 9) update_state_ = 0;
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);

    int change = (start_val - end_val)/50;
    LCD.clear();  LCD.home() ;

    switch (state_){
      case 0:
        left_claw_extended += change;
        LCD.setCursor(0,0); LCD.print("left_claw_extended");
        LCD.setCursor(0,1); LCD.print(left_claw_extended);
        break;
      case 1:
        left_claw_rest += change;
        LCD.setCursor(0,0); LCD.print("left_claw_rest");
        LCD.setCursor(0,1); LCD.print(left_claw_rest);
        break;
      case 2:
        left_claw_vertical += change;
        LCD.setCursor(0,0); LCD.print("left_claw_vertical");
        LCD.setCursor(0,1); LCD.print(left_claw_vertical);
        break;
      case 3:
        left_open += change;
        LCD.setCursor(0,0); LCD.print("left_open");
        LCD.setCursor(0,1); LCD.print(left_open);
        break;
      case 4:
        left_close += change;
        LCD.setCursor(0,0); LCD.print("left_close");
        LCD.setCursor(0,1); LCD.print(left_close);
        break;
      case 5:
        right_claw_extended += change;
        LCD.setCursor(0,0); LCD.print("right_claw_extended");
        LCD.setCursor(0,1); LCD.print(right_claw_extended);
        break;
      case 6:
        right_claw_rest += change;
        LCD.setCursor(0,0); LCD.print("right_claw_rest");
        LCD.setCursor(0,1); LCD.print(right_claw_rest);
        break;
      case 7:
        right_claw_vertical += change;
        LCD.setCursor(0,0); LCD.print("right_claw_vertical");
        LCD.setCursor(0,1); LCD.print(right_claw_vertical);
        break;
      case 8:
        right_open += change;
        LCD.setCursor(0,0); LCD.print("right_open");
        LCD.setCursor(0,1); LCD.print(right_open);
        break;
      case 9:
        left_close += change;
        LCD.setCursor(0,0); LCD.print("left_close");
        LCD.setCursor(0,1); LCD.print(left_close);
        break;
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace sequences
