#include "sequences/claw.h"

#include "phys253.h"
#if USE_UPDATE()
#include <LiquidCrystal.h>
#endif  // USE_UPDATE()

namespace sequences {

int8_t Claw::state_ = 0;
Claw::ClawSequence Claw::currentSequence_ = NONE;
uint8_t Claw::grabOffset_ = 0;
unsigned long Claw::delayUntil_ = 0;
bool Claw::left_ = false;

unsigned long Claw::fold_delay = 600;

int16_t Claw::left_claw_extended = L_C_EXTEND();
int16_t Claw::left_claw_folded = L_C_FOLD();
int16_t Claw::left_claw_drop = L_C_DROP();
int16_t Claw::left_claw_vertical = L_C_VERTICAL();
int16_t Claw::left_open = L_OPEN();
int16_t Claw::left_close = L_CLOSE();
int16_t Claw::left_offset[] = L_OFFSET();

int16_t Claw::open_offset = CLAW_OPEN_OFFSET();

int16_t Claw::right_claw_extended = R_C_EXTEND();
int16_t Claw::right_claw_folded = R_C_FOLD();
int16_t Claw::right_claw_drop = R_C_DROP();
int16_t Claw::right_claw_vertical = R_C_VERTICAL();
int16_t Claw::right_open = R_OPEN();
int16_t Claw::right_close = R_CLOSE();
int16_t Claw::right_offset[] = R_OFFSET();

void Claw::set_arm_position(int8_t side, int8_t position) {
  uint16_t val;
  switch (position) {
    case 0:  // extended
      val = (side == LEFT_CLAW) ? left_claw_extended : right_claw_extended;
      break;
    case 1:  // folded
      val = (side == LEFT_CLAW) ? left_claw_folded : right_claw_folded;
      break;
    case 2:  // vertical
      val = (side == LEFT_CLAW) ? left_claw_vertical : right_claw_vertical;
      break;
    case 3:  // mostly open
      val = (side == LEFT_CLAW) ? left_open + open_offset : right_open - open_offset;
      break;
    case 4:  // closed
      val = (side == LEFT_CLAW) ? left_close : right_close;
  }
  if (side == LEFT_CLAW) {
    if (position < 3) CLAW_L_RAISE_SERVO().write(val);
    else CLAW_L_GRAB_SERVO().write(val);
  }
  else {
    if (position < 3) CLAW_R_RAISE_SERVO().write(val);
    else CLAW_R_GRAB_SERVO().write(val);
  }
}

void Claw::fold(bool left_surface) {
  RCServo0.attach(SERVO_0());
  RCServo1.attach(SERVO_1());
  RCServo2.attach(SERVO_2());
  RCServo3.attach(SERVO_3());

  // open both claws simultaneously and hope they don't collide
  set_arm_position(LEFT_CLAW, EXTENDED);
  set_arm_position(RIGHT_CLAW, EXTENDED);
  delay(fold_delay);

  set_arm_position(left_surface ? RIGHT_CLAW : LEFT_CLAW, CLOSED);
  delay(fold_delay);
  set_arm_position(left_surface ? LEFT_CLAW : RIGHT_CLAW, OPEN);
  delay(fold_delay);
  set_arm_position(left_surface ? LEFT_CLAW : RIGHT_CLAW, FOLDED);
  delay(fold_delay);
  set_arm_position(left_surface ? RIGHT_CLAW : LEFT_CLAW, FOLDED);
  delay(fold_delay);
}

bool Claw::raise(int8_t side) {
  // if (currentSequence_ != NONE) return false;
  currentSequence_ = RAISE;
  state_ = 0;
  left_ = (side == LEFT_CLAW) ? true : false;
  return true;
}

bool Claw::grab(int8_t side, int8_t offset) {
  // if (currentSequence_ != NONE) return false;
  currentSequence_ = GRAB;
  state_ = 0;
  left_ = (side == LEFT_CLAW) ? true : false;
  grabOffset_ = offset;
  return true;
}

bool Claw::release(int8_t side) {
  // if (currentSequence_ != NONE) return false;
  currentSequence_ = RELEASE;
  state_ = 0;
  left_ = (side == LEFT_CLAW) ? true : false;
  return true;
}

bool Claw::loop() {
  // State description          notes
  // 0     doing nothing state
  // 1     release              wait for a period of time then activate claw
  // 2     grab                 same as release
  // 3     raise                wait for a period of time for arm then return complete
  // 4     claw_wat             wait for a period of time for claw. Uses same the same code as raise
  if (currentSequence_ == NONE) {
    return true;
  } else if (currentSequence_ == RAISE) {
    switch (state_) {
      case 0: 
        if (left_) {
          CLAW_L_RAISE_SERVO().write(left_claw_vertical);
          CLAW_L_GRAB_SERVO().write(left_open);
        }
        else {
          CLAW_R_RAISE_SERVO().write(right_claw_vertical);
          CLAW_L_GRAB_SERVO().write(left_open);
        }
        delayUntil_ = millis() + 600;
        state_ = 1;
      case 1:
        if (millis() >= delayUntil_) currentSequence_ = NONE;
        break;
    }
  } else if (currentSequence_ == GRAB) {
    switch (state_) {
      case 0: 
        if (left_) {
          CLAW_L_RAISE_SERVO().write(constrain(left_claw_extended + left_offset[grabOffset_], 0, 180));
          CLAW_L_GRAB_SERVO().write(left_open);
        } else {
          CLAW_R_RAISE_SERVO().write(constrain(right_claw_extended + right_offset[grabOffset_], 0, 180));
          CLAW_R_GRAB_SERVO().write(right_open);
        }
        delayUntil_ = millis() + 450;
        state_ += 1;
        //fall through
      case 1:
        if (millis() >= delayUntil_) {
          state_ += 1;
          //fall through
        } else {
          break;
        }
      case 2:
        if (left_) {
          CLAW_L_GRAB_SERVO().write(left_close);
        } else {
          CLAW_R_GRAB_SERVO().write(right_close);
        }
        delayUntil_ = millis() + 400;
        state_ += 1;
        //fall through
      case 3:
        if (millis() >= delayUntil_) currentSequence_ = NONE;
        break;
    }
  }
  else if (currentSequence_ == RELEASE) {
    switch (state_) {
      case 0: 
        if (left_) {
          CLAW_L_RAISE_SERVO().write(left_claw_drop );
        } else {
          CLAW_R_RAISE_SERVO().write(right_claw_drop);
        }
        delayUntil_ = millis() + 700;
        state_ += 1;
        //fall through
      case 1:
        if (millis() >= delayUntil_) {
          state_ += 1;
          //fall through
        } else {
          break;
        }
      case 2:
        if (left_) {
          CLAW_L_GRAB_SERVO().write(left_open);
        } else {
          CLAW_R_GRAB_SERVO().write(right_open);
        }
        delayUntil_ = millis() + 400;
        state_ += 1;
        //fall through
      case 3:
        if (millis() >= delayUntil_) {
          state_ += 1;
          //fall through
        } else {
          break;
        }
      case 4: 
        if (left_) {
          CLAW_L_RAISE_SERVO().write(left_claw_vertical);
        } else {
          CLAW_R_RAISE_SERVO().write(right_claw_vertical);
        }
        delayUntil_ = millis() + 600;
        state_ += 1;
        //fall through
      case 5:
        if (millis() >= delayUntil_) {
          state_ += 1;
          //fall through
        } else {
          break;
        }
      case 6:
        if (millis() >= delayUntil_) currentSequence_ = NONE;
        break;
    }
  }
  return false;
}

}  // namespace sequences
