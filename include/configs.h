#ifndef WALKERVILLE_ROBOT_CONFIGS_H
#define WALKERVILLE_ROBOT_CONFIGS_H

// debug and generation use
#define USE_UPDATE() 1
#define DEBUG() 1

// PID initial values
#define GAIN_T() 1
#define GAIN_P() 8
#define GAIN_I() 0
#define GAIN_D() 5

// thresholds
#define TAPE_IR_STRENGTH_THRESHOLD() 100
#define GATE_IR_STRENGTH_THRESHOLD() 100
#define ENCODER_WAIT_TIME() 3

// velocity
#define VELOCITY() 90 // 80 to 100

// motor
#define R_MOTOR() 2
#define L_MOTOR() 3 // going backwards
#define ZIPLINE_MOTOR() 0

// qrd sensors
#define RR_QRD_SENSOR() 0
#define RC_QRD_SENSOR() 1
#define LC_QRD_SENSOR() 2
#define LL_QRD_SENSOR() 3

// encoder sensors
#define R_ENCODER() 3
#define L_ENCODER() 2

// maneuver
#define WHEEL_DIAMETER() 6.0 // cm
#define GEAR_RATIO() 2.0
#define AXLE_LENGTH() 21.59 // cm
#define MANEUVER_GAIN() 2
#endif  // WALKERVILLE_ROBOT_CONFIGS_H
