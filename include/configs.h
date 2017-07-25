#ifndef WALKERVILLE_ROBOT_CONFIGS_H
#define WALKERVILLE_ROBOT_CONFIGS_H

// debug and generation use
#define USE_UPDATE() 1
#define DEBUG() 1
#define LOOP_DELAY() 50
#define TUNE_THRESHOLD() 500 //for determining if spinning knob 6 should do anything

// PID initial values
#define GAIN_T() 1
#define GAIN_P() 12
#define GAIN_I() 0
#define GAIN_D() 0

// beacon following initial values
#define BEACON_GAIN_P() 12
#define BEACON_GAIN_I() 0
#define BEACON_GAIN_D() 0
#define BEACON_UNCERTAINTY() 50 //Have no idea what this should be, needs testing

// claw
#define L_C_EXTEND() 5  // 5
#define L_C_REST() 160  // 170
#define L_C_VERTICAL() 100
#define L_OPEN() 20  //20
#define L_CLOSE() 85  // 90
// THe right claw needs to be tested
#define R_C_EXTEND() 177
#define R_C_REST() 10
#define R_C_VERTICAL() 60
#define R_OPEN() 102
#define R_CLOSE() 34

// motor
#define ACCEL_LIMIT() 50 // This is not the correct value
#define VELOCITY() 90 // 80 to 100
#define PLATFORM_RAISE_SPEED() -255
#define PLATFORM_LOWER_SPEED() 30
#define PLATFORM_MAINTAIN_SPEED() -10
#define BACKUP_SPEED() -40  // for backing up after getting zipline
#define BACKUP_TIME() 3000
#define DEAD_RECKONING_SPEED() 50 // slow forward

// thresholds
#define TAPE_QRD_STRENGTH_THRESHOLD() 100
#define GATE_IR_STRENGTH_THRESHOLD() 150
#define ENCODER_WAIT_TIME() 3

// motor
#define R_MOTOR() 2
#define L_MOTOR() 3 // going backwards
#define PLATFORM_MOTOR() 0
#define SERVO_0() 12 //no longer using TINAH servo connectors
#define SERVO_1() 13
#define SERVO_2() 14
#define SERVO_3() 15

// qrd sensors
#define RR_QRD_SENSOR() 0
#define RC_QRD_SENSOR() 1
#define LC_QRD_SENSOR() 2
#define LL_QRD_SENSOR() 3

// encoder sensors
#define R_ENCODER() 3
#define L_ENCODER() 2

// 10 KHz IR sensors
#define L_BEACON_SENSOR() 4
#define R_BEACON_SENSOR() 5

// switches
#define PLATFORM_UPPER_SWITCH() 7
#define PLATFORM_LOWER_SWITCH() 6
#define LEFT_RIGHT_SWITCH() 5

// maneuver
#define WHEEL_DIAMETER() 6.0 // cm
#define GEAR_RATIO() 2.0
#define AXLE_LENGTH() 21.59 // cm
#define MANEUVER_GAIN() 2

// gate
#define GATE_WAITING_DISTANCE() 120 // cm
#define CAUTIOUS_GATE_ROUTINE() 1 // only cross on signal rise

// zipline
#define ZIPLINE_TURN_DISTANCE() 75 // cm
#define MAX_ZIPLINE_DISTANCE() 50 // cm -- don't go further than this!

//macro to make switch code more concise
#define MACRO_VARIABLE_TO_STRING(Variable) (void(Variable),#Variable) //supposed to convert variable name to a string
#define SWITCH_CASES(case_state, variable)\
  case case_state:\
      variable += change;\
      LCD.setCursor(0,0); LCD.print(MACRO_VARIABLE_TO_STRING(variable));\
      LCD.setCursor(0,1); LCD.print(variable);\
      break;

#endif  // WALKERVILLE_ROBOT_CONFIGS_H
