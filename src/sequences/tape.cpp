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
  if (error_source_ == 0){
    error_ = qrd_.getTapeError();
  }
   if (error_source_ == 1){
    error_ = beacon_.getTapeError();
    // TODO: decide what should happen if error is -4
    if (error_ == -4) error_ = 0;
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
  if (error_source_ == 0) {
    i_error = 0;
    kp_ = gain_p_ * error;
    ki_ = gain_i_ * i_error_;
    kd_ = gain_d_ * (error - prev_error_)*1000/dt;
  } else {
    kp_ = beacon_gain_p * error;
    ki_ = beacon_gain_i * i_error_;
    kd_ = beacon_gain_d * (error - prev_error_)*1000/dt;
  }
  prev_error_ = error;
  return gain_t_*(kp_ + ki_ - kd_);
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
  int change = 0;
  stop();
  while (!startbutton()){
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 7) update_state_ = 0;
    int tune_val = knob(7);
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    if (tune_val >  TUNE_THRESHOLD()){
      change = (start_val - end_val)/4 ;
    }
    LCD.clear();  LCD.home() ;

    switch (update_state_){
    SWITCH_CASES(0,gain_t_)
    SWITCH_CASES(1,gain_p_)
    SWITCH_CASES(2,gain_i_)
    SWITCH_CASES(3,gain_d_)
    SWITCH_CASES(4,velocity_)
    SWITCH_CASES(5,beacon_gain_p)
    SWITCH_CASES(6,beacon_gain_i)
    SWITCH_CASES(7,beacon_gain_d)
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace tape
