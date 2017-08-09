#ifndef WALKERVILLE_ROBOT_CONFIGS_H
#define WALKERVILLE_ROBOT_CONFIGS_H

// debug and generation use
#define USE_UPDATE() 0
#define DEBUG() 0
#define LOOP_DELAY() 10
#define TUNE_THRESHOLD() 500 //for determining if spinning knob 6 should do anything
#define CAUTIOUS_GATE_ROUTINE() 1 // only cross on signal rise

// claw servo positions
#define L_C_EXTEND() 23
#define L_C_FOLD() 180  // folded position
#define L_C_DROP() 174  // position for dropping agent
#define L_C_VERTICAL() 75
#define L_OPEN() 55
#define L_CLOSE() 180
#define L_OFFSET() {-6, -15, -20}

#define R_C_EXTEND() 160
#define R_C_FOLD() 2
#define R_C_DROP() 8
#define R_C_VERTICAL() 90
#define R_OPEN() 105
#define R_CLOSE() 8
#define R_OFFSET() {-8, 2, 4}

// claw open position is too wide when folded, reduce by this amount to avoid collisions
#define CLAW_OPEN_OFFSET() 28

// distances (cm)
#define GATE_WAITING_DISTANCE() 100
#define ZIPLINE_CHECKPOINT_DISTANCE() 50.0
#define ZIPLINE_BACKUP_DISTANCE() 40.0

// Pickup values
#define PICKUP_TO_RAMP() 280
#define PICKUP_TO_INTERSECTION() 300
#define PICKUP_TURN_DEGREE() 30
#define PICKUP_TURN_FORWARD_DISTANCE() 5
#define PICKUP_DISTANCE_FIRST_AGENT() 20
#define PICKUP_HEIGHT() {2,1,0,2,1,0}  // {1,2,0,1,2,0}
#define PICKUP_BACKWARD_DISTANCE() -4
#define PICKUP_FORWARD_DISTANCE() 3

// PID initial values
#define GAIN_P() 12.0
#define GAIN_I() 0.0
#define GAIN_D() 0.0

// beacon following initial values
#define BEACON_GAIN_P() 6  // 12
#define BEACON_GAIN_I() 0
#define BEACON_GAIN_D() 0
#define BEACON_UNCERTAINTY() 70 // 50
#define BEACON_THRESHOLD() 70  // Beacon is undetected below this value

// motor
#define ACCEL_LIMIT() 50 // This is not the correct value
#define VELOCITY() 90 // 80 to 100

#define PLATFORM_RAISE_SPEED() -210
#define PLATFORM_LOWER_SPEED_TOP() 29  // 40
#define PLATFORM_LOWER_SPEED_BOTTOM() 60

#define ZIPLINE_FORWARD_POWER() 80
#define ZIPLINE_BACKUP_POWER() -40
#define PLATFORM_MAINTAIN_POWER() -50

// thresholds
#define TAPE_QRD_STRENGTH_THRESHOLD() 150
#define GATE_IR_STRENGTH_THRESHOLD() 200
#define ENCODER_WAIT_TIME() 3

// motor
#define R_MOTOR() 2
#define L_MOTOR() 3 // going backwards
#define PLATFORM_MOTOR() 0
#define SERVO_0() 12 //no longer using TINAH servo connectors
#define SERVO_1() 13
#define SERVO_2() 14
#define SERVO_3() 15

#define CLAW_L_RAISE_SERVO() RCServo0
#define CLAW_L_GRAB_SERVO() RCServo2
#define CLAW_R_RAISE_SERVO() RCServo1
#define CLAW_R_GRAB_SERVO() RCServo3

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
#define LEFT_RIGHT_SWITCH() 4

// maneuver
#define WHEEL_DIAMETER() 6.75 // cm
#define GEAR_RATIO() 2.0
#define ENCODER_TICKS_PER_REVOLUTION() 96.0
#define AXLE_LENGTH() 21.59 // cm
#define MANEUVER_GAIN() 2

#define LINE_FOLLOW_0_SPEED() 180
#define LINE_FOLLOW_0_GAIN() 45
#define LINE_FOLLOW_1_SPEED() 255
#define LINE_FOLLOW_1_GAIN() 50
#define LINE_FOLLOW_2_SPEED() 255
#define LINE_FOLLOW_2_GAIN() 50

// Previous values
/*
#define LINE_FOLLOW_0_SPEED() 80
#define LINE_FOLLOW_0_GAIN() 22
#define LINE_FOLLOW_1_SPEED() 120
#define LINE_FOLLOW_1_GAIN() 24
#define LINE_FOLLOW_2_SPEED() 255
#define LINE_FOLLOW_2_GAIN() 50
*/


//macro to make switch code more concise
#define MACRO_VARIABLE_TO_STRING(Variable) (void(Variable),#Variable) //supposed to convert variable name to a string
#define SWITCH_CASES(case_state, variable)\
  case case_state:\
      variable += change;\
      LCD.setCursor(0,0); LCD.print(MACRO_VARIABLE_TO_STRING(variable));\
      LCD.setCursor(0,1); LCD.print(variable);\
      break;

#endif  // WALKERVILLE_ROBOT_CONFIGS_H
