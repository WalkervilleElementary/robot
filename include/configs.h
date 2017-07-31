#ifndef WALKERVILLE_ROBOT_CONFIGS_H
#define WALKERVILLE_ROBOT_CONFIGS_H

// debug and generation use
#define USE_UPDATE() 1
#define DEBUG() 1
#define LOOP_DELAY() 50

// PID initial values
#define GAIN_T() 1
#define GAIN_P() 12
#define GAIN_I() 0
#define GAIN_D() 0

// claw
#define L_C_EXTEND() 23
#define L_C_REST() 180
#define L_C_VERTICAL() 105
#define L_C_TOP() 20
#define L_C_MIDDLE() 10
#define L_C_BOTTOM() 9
#define L_OPEN() 75
#define L_CLOSE() 165

#define R_C_EXTEND() 168
#define R_C_REST() 4
#define R_C_VERTICAL() 60
#define R_C_TOP() 168
#define R_C_MIDDLE() 172
#define R_C_BOTTOM() 180
#define R_OPEN() 161
#define R_CLOSE() 30

// motor
#define ACCEL_LIMIT() 50 // This is not the correct value
#define VELOCITY() 90 // 80 to 100
#define PLATFORM_RAISE_SPEED() -110
#define PLATFORM_LOWER_SPEED() 40
#define SLOW_FORWARD_SPEED() 50 // for e.g. dead reckoning
#define BACKUP_SPEED() -40  // for backing up after getting zipline

// thresholds
#define TAPE_QRD_STRENGTH_THRESHOLD() 100
#define GATE_IR_STRENGTH_THRESHOLD() 150
#define ENCODER_WAIT_TIME() 3
#define IR_ERROR_MARGIN() 8

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

// maneuver
#define WHEEL_DIAMETER() 6.0 // cm
#define WHEEL_CIRCUMFERENCE() (WHEEL_DIAMETER() * 3.14159265)
#define GEAR_RATIO() 2.0
#define TICKS_PER_ROTATION() 12.0
#define TICKS_PER_WHEEL_ROTATION() (TICKS_PER_ROTATION() * GEAR_RATIO())
#define AXLE_LENGTH() 21.59 // cm
#define AXLE_CIRCUMFERENCE() (AXLE_LENGTH() * 3.14159265)
#define CM(X) ((X / WHEEL_CIRCUMFERENCE() * TICKS_PER_WHEEL_ROTATION())
#define DEG(X) (CM(X / 360.0 * AXLE_CIRCUMFERENCE()))

#define MANEUVER_GAIN() 2

// gate
#define GATE_WAITING_DISTANCE() 120 // cm
#define CAUTIOUS_GATE_ROUTINE() 1 // only cross on signal rise

// zipline distances (cm)
#define ZIPLINE_TURN_DISTANCE() 75
#define MAX_ZIPLINE_DISTANCE() 50
#define BACKUP_DISTANCE() 25

#endif  // WALKERVILLE_ROBOT_CONFIGS_H
