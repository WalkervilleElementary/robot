#ifndef WALKERVILLE_ROBOT_CONFIGS_H
#define WALKERVILLE_ROBOT_CONFIGS_H

// debug and generation use
#define USE_UPDATE() 1
#define DEBUG() 1
#define DEVICE() 0

// PID initial values
#define GAIN_T() 7
#define GAIN_P() 8
#define GAIN_I() 0
#define GAIN_D() 8

// thresholds
#define TAPE_IR_STRENGTH_THRESHOLD() 100
#define GATE_IR_STRENGTH_THRESHOLD() 100

// velocity
#define VELOCITY() 400

#define R_MOTOR() 0
#define L_MOTOR() 1
#define L_SENSOR() 2
#define R_SENSOR() 0

#endif  // WALKERVILL_ROBOT_CONFIGS_H
