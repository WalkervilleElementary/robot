#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>

#include "utils/encoderinterrupts.h"

#include "hardware/qrd.h"
#include "hardware/encoder.h"
#include "hardware/beacon.h"

#include "sequences/claw.h"
#include "sequences/drivetrain.h"
#include "sequences/platform.h"

#include "stages/gate.h"
#include "stages/pickup.h"
#include "stages/zipline.h"

bool left_surface;
unsigned long loop_delay = LOOP_DELAY();

void setup(){
  portMode(0, INPUT);
  portMode(1, OUTPUT);

#if DEBUG()
  LCD.begin(16,2) ;
  Serial.begin(115200) ;
#endif  // DEBUG()

  RCServo0.attach(SERVO_0());
  RCServo1.attach(SERVO_1());
  RCServo2.attach(SERVO_2());
  RCServo3.attach(SERVO_3());
  
  left_surface = true;  // TODO read the switch
  // left_surface = digitalRead(LEFT_RIGHT_SWITCH());

  hardware::Qrd qrd;
  hardware::Beacon beacon;
  hardware::Encoder l_encoder(0);
  hardware::Encoder r_encoder(1, true);

  hardware::EncoderMotor l_motor(L_MOTOR(), l_encoder, true);
  hardware::EncoderMotor r_motor(R_MOTOR(), r_encoder);

  sequences::Claw claw;
  sequences::Drivetrain driver(l_motor, r_motor, qrd);
  sequences::Platform platform(driver);

  stages::Gate gate(driver, beacon, r_encoder); // TODO change encoder dynamically
  stages::Pickup pickup(qrd, r_encoder, claw, driver);
  stages::Zipline zipline(driver, platform, beacon, r_encoder, qrd);

  claw.fold(left_surface);
  delay(1000);

  enableEncoderInterrupts();

  ////////////////////////////////
  // Insert extra variable here //
  ////////////////////////////////
  uint8_t state = 0;
  unsigned long delayTime;
  ////////////////////////////////
  unsigned long waitUntil = millis() + loop_delay;
  for (;;) {//////////////////////
    ////////////////////////////////
    //// Insert code here //////////
    ////////////////////////////////
    switch (state) {
      case 0:  // wait for start button
        if (!startbutton()) break;
        else state++;
      case 1:  // going through gate
        if (gate.loop())
          state++;
        break;
      case 2:  // picking up agents
        if (pickup.loop())
          state++;
        break;
      case 3:  // finding zipline
         if (zipline.loop())
           state++;
         break;
    }
    ///////////////////////////////
    //// Do not touch /////////////
    ///////////////////////////////
    while (millis() < waitUntil) continue;
    waitUntil = millis() + loop_delay;

    driver.tick();
    l_encoder.tick();
    r_encoder.tick();
    l_motor.tick();
    r_motor.tick();
  }
}


void loop() {}

// real loop
/*
// EXTRA VARIABLES
uint8_t state = 0;

// CODE
    switch (state) {
      case 0:  // wait for start button
        if (!startbutton()) break;
        else state++;
      case 1:  // going through gate
        if (gate.loop())
          state++;
        break;
      case 2:  // picking up agents
        if (pickup.loop())
          state++;
        break;
      case 3:  // finding zipline
         if (zipline.loop())
           state++;
         break;
    }
*/

// loop for testing platform
/*
// EXTRA VARIABLES
  claw.raise(sequences::RIGHT_CLAW);
  while(!claw.loop()) delay(100);
  claw.raise(sequences::LEFT_CLAW);
  while(!claw.loop()) delay(100);

  int state = 0;

// CODE
    if (stopbutton())
    {
      platform.update();
      state = 0;  // restart routine after updating
    }

    // run through raise/lower routine once
    switch (state)
    {
      case 0:  // starting state
        platform.raise();
        state++;
      case 1:  // raising
        if (platform.loop())
          state++;
        break;
      case 2:
        if (!digitalRead(PLATFORM_UPPER_SWITCH())){
          platform.lower();
          state++;
        }
        break;
      case 3:  // lowering
        if (platform.loop())
          state++;
        break;
    }
*/

// loop for testing gate routine
/*
// EXTRA VARIABLES
  int state = 0;
  unsigned int gate_time = 0;

// CODE
    if (stopbutton()){
      gate.update();
    }

    // Tape follow loop
    switch (state) {
      case 0:
        if (gate.loop()) state++;
        break;
      case 1:
        driver.commandLineFollow(1);
        break;
      default:
        driver.stop();
        break;
    }
*/

// loop for testing zipline routine
/*
// EXTRA VARIABLE
  int state = 0;
// CODE
    if (stopbutton()) {
      zipline.update();
      platform.update();
      state = 0;  // restart routine after update
    }

    switch (state) {
      case 0:
        zipline.set_state(0);
        if (claw.raise(sequences::LEFT_CLAW)) state++;
        break;
      case 1:  // raise left claw
        if (claw.loop()) {
          if (claw.raise(sequences::RIGHT_CLAW)) state++;
        }
        break;
      case 2:  // raise right claw
        if (claw.loop()) state++;
        break;
      case 3:  // zipline
        if (zipline.loop()) state++;
        break;
      case 4:
        driver.stop();
    }
*/

// loop for testing pickup
/*
// EXTRA VARIABLE

// CODE
    pickup.loop();

    if (stopbutton())
    {
      driver.stop();
      pickup.update();
      qrd.update();
    }
*/

// loop for testing tape follow
/*
// EXTRA VARIABLE
  int state = 0;
  unsigned long delayTime;
  
// CODE
    if (state == 0) {
      driver.commandLineFollow(0);
      if (qrd.isIntersection()) {
        driver.stop();
        delayTime = millis() + 1000;
        state++;
      }
    } else if (state == 1) {
      if (millis() >= delayTime) {
        driver.commandDriveStraight(6);
        state++;
      }
    } else if (state == 2) {
      if (driver.readyForCommand()) state = 0;
    }

    

    if (stopbutton()){
      qrd.update();
    }
*/

//loop for testing following ir
/*
// EXTRA VARIABLES

// CODE
    driver.commandBeaconFollow();

    if (stopbutton()){
      qrd.update();
    }
*/

// Testing motor speed (deprecated)
/*
void loop(){
  int motor_speed = 90;

  if (stopbutton()){
    while (!startbutton()){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print(knob(6)/4);
      motor_speed = knob(6)/4;
      delay(50);
    }
  }

  unsigned long start = millis();
  while (millis() < start + 1000){
    driver.sendMotorCommand(motor_speed,0);
    delay(100);
  }
  delay(5000);
}
*/

// loop for testing maneuver
/*
// EXTRA VARIABLES
  int distance = 0;
  int mode = 0;

// CODE
    if (stopbutton() && driver.readyForCommand()){
      driver.stop();
      while (!startbutton()){
        if (stopbutton()) mode = (mode + 1) % 3;
        distance = knob(6) - 512;
        LCD.clear(); LCD.home();
        LCD.setCursor(0,0);
        
        if (mode == 0) LCD.print("distance");
        else if (mode == 1) LCD.print("right");
        else if (mode == 2) LCD.print("left");
        
        LCD.setCursor(0,1); LCD.print(distance);
        delay(50);
      }
 
      if (mode == 0) driver.commandDriveStraight(distance);
      else if (mode == 1) driver.commandTurnRight(distance);
      else driver.commandTurnLeft(distance);
    }
   
*/

// loop for testing Drivetrain.setPower and motor ramping
/*
// EXTRA VARIABLES
  int state = 0;
  unsigned long delayTime;

// CODE
    const int16_t power = 255;
    if (stopbutton()) {
      driver.stop();
      state = 0;
    }
    if (state == 0) {
      if (startbutton()) {
        state++;
      }
    }
    if (state == 1) {
      if (millis() > delayTime) {
        driver.setPower(power, power);
        delayTime = millis() + 2000;
        state = 2;
      }
    }
    if (state == 2) {
      if (millis() > delayTime) {
        driver.setPower(-power, -power);
        delayTime = millis() + 2000;
        state = 1;
      }
    }
*/

// loop for claw testing
/*
// EXTRA VARIABLES
  int state = 0;
  int left = 0;
// CODE
    while (!startbutton()) {
      if (stopbutton())
        claw.update();
      state = knob(6);
      left = knob(7);
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("state");
      LCD.setCursor(7,0); LCD.print(state);
      LCD.setCursor(0,1); LCD.print("left");
      LCD.setCursor(7,1); LCD.print(left);
      delay(100);
    }
    LCD.clear(); LCD.home();
    if (state < 300) {
      LCD.setCursor(0,0); LCD.print("raise");
      if (left > 500) claw.raise(sequences::LEFT_CLAW);
      else claw.raise(sequences::RIGHT_CLAW);
    } else if (state < 600) {
      if (state < 400) {
        LCD.setCursor(0,0); LCD.print("grab");
        if (left > 500) claw.grab(sequences::LEFT_CLAW, 0);
        else claw.grab(sequences::RIGHT_CLAW, 0);
      } else if (state < 500) {
        LCD.setCursor(0,0); LCD.print("grab");
        if (left > 500) claw.grab(sequences::LEFT_CLAW, 1);
        else claw.grab(sequences::RIGHT_CLAW, 1);
      } else {
        LCD.setCursor(0,0); LCD.print("grab");
        if (left > 500) claw.grab(sequences::LEFT_CLAW, 2);
        else claw.grab(sequences::RIGHT_CLAW, 2);
      }
    } else {
      LCD.setCursor(0,0); LCD.print("release");
      if (left > 500) claw.release(sequences::LEFT_CLAW);
      else claw.release(sequences::RIGHT_CLAW);
    }
    while (!claw.loop());
    waitTime = millis() + loop_delay;
*/

// loop for finding right servo positions
/*
void loop() {
  int height = knob(6) / 4;
  int open = knob(7) / 4;
  LCD.clear(); LCD.home(); LCD.setCursor(0,0);
  LCD.print("height "); LCD.print(height);
  LCD.setCursor(0,1); LCD.print("open "); LCD.print(open);
  RCServo1.write(height);
  RCServo3.write(open);
  delay(50);
}
*/

