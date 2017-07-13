#ifndef WALKERVILLE_ROBOT_CONFIGS_H
#define WALKERVILLE_ROBOT_CONFIGS_H

// debug and generation use
#define USE_UPDATE() 1
#define DEBUG() 1
#define DEVICE() 1

// PID initial values
#define GAIN_T() -1
#define GAIN_P() 14
#define GAIN_I() 0
#define GAIN_D() 5

// thresholds
#define TAPE_IR_STRENGTH_THRESHOLD() 100
#define GATE_IR_STRENGTH_THRESHOLD() 100

// velocity
#define VELOCITY() 90 // 80 to 100

#define R_MOTOR() 0
#define L_MOTOR() 1
#define R_SENSOR() 0
#define L_SENSOR() 2

#endif  // WALKERVILL_ROBOT_CONFIGS_H
