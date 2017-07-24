#include "configs.h"

#include <phys253.h>
#if DEBUG() || USE_UPDATE()
#include <LiquidCrystal.h>
#endif
#include "sequences/tape.h"

namespace sequences{

int Tape::gain_t_ = GAIN_T();
int Tape::gain_p_ = GAIN_P();
int Tape::gain_i_ = GAIN_I();
int Tape::gain_d_ = GAIN_D();
int Tape::velocity_ =  VELOCITY();
int Tape::beacon_gain_p = BEACON_GAIN_P();
int Tape::beacon_gain_i = BEACON_GAIN_I();
int Tape::beacon_gain_d = BEACON_GAIN_D();

uint8_t Tape::error_source_ = 0;
int8_t Tape::i_error_ = 0;
int8_t Tape::prev_error_ = 0;

int Tape::kp_;
int Tape::ki_;
int Tape::kd_;

int Tape::error_;
int Tape::command_;

bool Tape::loop(){
  // TODO modify this to use ir error if error_source_ is 1
  if (error_source_ == 0){
    error_ = qrd_.getTapeError();
  }
   if (error_source_ == 1){
    error_ = beacon.getTapeError();
  }
  command_ = computeCommand(error_, 100);
  motor_.sendMotorCommand(velocity_, command_);
}

void Tape::stop(){
  motor_.stop();
}
// TODO fix implicit casting
int Tape::computeCommand(int8_t error, unsigned long dt){
  i_error_ += error*1000/dt;
  i_error_ = constrain(i_error_, -5, 5);
  if (error_source_ == 0){
    kp_ = gain_p_ * error;
    ki_ = gain_i_ * i_error_;
    kd_ = gain_d_ * (error - prev_error_)*1000/dt;
  }
  else{
    kp_ = beacon_gain_p_ * error;
    ki_ = beacon_gain_i_ * i_error_;
    kd_ = beacon_gain_d_ * (error - prev_error_)*1000/dt;
  }
  prev_error_ = error;
  return gain_t_*(kp_ + ki_ +  kd_);
}

void Tape::followIr() {
  error_source_ = 1;
}

void Tape::followTape() {
  error_source_ = 0;
}

#if USE_UPDATE()
void Tape::update(){
  int8_t update_state_ = 0;
  stop();
  while (!startbutton()){
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 7) update_state_ = 0;
    int tune_val = knob(7);
    if (tune_val < TUNE_THRESHOLD()){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Tuning off");
      LCD.setCursor(0,1); LCD.print(tune_val);
      delay(100);
    }
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);

    int change = (start_val - end_val)/4 ;
    LCD.clear();  LCD.home() ;

    switch (update_state_){
    case 0:
        gain_t_ += change;
        LCD.setCursor(0,0); LCD.print("gain_tot");
        LCD.setCursor(0,1); LCD.print(gain_t_);
        break;
    case 1:
        gain_p_ += change;
        LCD.setCursor(0,0); LCD.print("gain_p");
        LCD.setCursor(0,1); LCD.print(gain_p_);
        break;
    case 2:
        gain_i_ += change;
        LCD.setCursor(0,0); LCD.print("gain_i");
        LCD.setCursor(0,1); LCD.print(gain_i_);
        break;
    case 3:
        gain_d_ += end_val - start_val;
        LCD.setCursor(0,0); LCD.print("gain_d");
        LCD.setCursor(0,1); LCD.print(gain_d_);
        break;
    case 4:
        velocity_ += change;
        LCD.setCursor(0,0); LCD.print("velocity");
        LCD.setCursor(0,1); LCD.print(velocity_);
        break;
    case 5:
        beacon_gain_p_ += change;
        LCD.setCursor(0,0); LCD.print("beacon_gain_p");
        LCD.setCursor(0,1); LCD.print(beacon_gain_p_);
        break;
    case 6:
        beacon_gain_i_ += change;
        LCD.setCursor(0,0); LCD.print("beacon_gain_i");
        LCD.setCursor(0,1); LCD.print(beacon_gain_i_);
        break;
    case 7:
        beacon_gain_d_ += change;
        LCD.setCursor(0,0); LCD.print("beacon_gain_d");
        LCD.setCursor(0,1); LCD.print(beacon_gain_d_);
        break;
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace tape
