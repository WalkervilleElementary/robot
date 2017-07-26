#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>

#include "hardware/driver.h"
#include "hardware/qrd.h"
#include "hardware/encoder.h"
#include "hardware/beacon.h"

#include "sequences/claw.h"
#include "sequences/tape.h"
#include "sequences/platform.h"
#include "sequences/maneuver.h"

#include "stages/gate.h"
#include "stages/pickup.h"
#include "stages/zipline.h"

hardware::Driver driver;
hardware::Qrd qrd;
hardware::Encoder encoder;
hardware::Beacon beacon;

sequences::Claw claw;
sequences::Maneuver maneuver(driver, encoder);
sequences::Platform platform(driver);
sequences::Tape tape(qrd, beacon, driver);

stages::Gate gate(tape, beacon, encoder);
stages::Pickup pickup(qrd, claw, maneuver, tape);
stages::Zipline zipline(tape, platform, maneuver, beacon, driver, encoder);

bool left_surface;
unsigned long loop_delay = LOOP_DELAY();

void setup(){
  portMode(0, INPUT);
  portMode(1, OUTPUT);

  LCD.begin(16,2) ;

  Serial.begin(9600) ;

  RCServo0.attach(SERVO_0());
  RCServo1.attach(SERVO_1());
  RCServo2.attach(SERVO_2());
  RCServo3.attach(SERVO_3());

//claw.raise(sequences::RIGHT_CLAW);
//while(!claw.loop()) delay(100);
//claw.raise(sequences::LEFT_CLAW);
//while(!claw.loop()) delay(100);

  left_surface = true;  // TODO read the switch
  // left_surface = digitalRead(LEFT_RIGHT_SWITCH());
  // claw.fold(left_surface);
  // delay(1000);
}

// real loop
/*
uint8_t state = 0;
void loop() {
#if DEBUG()
  LCD.clear(); LCD.home();
  LCD.setCursor(0,0); LCD.print(state);
#endif  // DEBUG()

  switch (state) {
    case 0:  // going through gate
      if (gate.loop())
        state++;
      break;
    case 1:  // picking up agents
      if (pickup.loop())
        state++;
      break;
    case 2:  // finding zipline
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("zipline stage");
    //   if (zipline.loop())
    //     state++;
    //   break;
  }
}
*/

// loop for testing platform
/*
//claw.raise(sequences::RIGHT_CLAW);
//while(!claw.loop()) delay(100);
//claw.raise(sequences::LEFT_CLAW);
//while(!claw.loop()) delay(100);

int state = 0;
void loop()
{
#if DEBUG()
  LCD.clear(); LCD.home();
  LCD.setCursor(0,0);
  LCD.print(digitalRead(PLATFORM_UPPER_SWITCH()));
  LCD.print(digitalRead(PLATFORM_LOWER_SWITCH()));
  LCD.setCursor(0, 1); LCD.print(state);
#endif  // DEBUG()
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
  delay(LOOP_DELAY());
}
*/

// loop for testing gate routine
/*
int state = 0;
void loop(){
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()
#if USE_UPDATE()
  if (stopbutton()){
    gate.update();
    // qrd.update();
  }
#endif  // USE_UPDATE()

  // Tape follow loop
  switch (state){
    case 0:
      if (gate.loop())
        state++;
      else
        break;
    case 1:
      tape.loop();
  }
  delay(50);
}
*/


// loop for testing zipline routine
int state = 0;
void loop() {
  if (stopbutton()) {
    zipline.update();
    platform.update();
    state = 0;  // restart routine after update
  }

  switch (state) {
    case 0:
      zipline.set_state(1);
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
  }

  delay(loop_delay);
}


// loop for testing pickup
/*
void loop()
{
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  pickup.loop();

  if (stopbutton())
  {
    driver.stop();
    pickup.update();
    tape.update();
    qrd.update();
    gate.update();
    // qrd.update();
  }
  delay(50);
}
*/

// loop for testing tape follow
/*
//int velocity = 50;
void loop(){
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  LCD.setCursor(0,1); LCD.print("Tape");
  tape.loop();
  //tape.update_vel(++velocity);
  if (qrd.isIntersection()){
    tape.stop();
    LCD.setCursor(0,1); LCD.print("MANEUVER");
    delay(1000);
    maneuver.straight(6);
    while (!maneuver.loop()) delay(50);
  }

  if (stopbutton()){
    tape.update();
    qrd.update();
  }
  delay(100);
}
*/

//loop for testing following ir
/*
void loop(){
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  LCD.setCursor(0,1); LCD.print("Following IR");
  tape.followIr();
  tape.loop();

  if (stopbutton()){
    tape.update();
    qrd.update();
  }
  delay(50);
}
*/

// Testing motor speed
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
int distance = 0;
int mode = 0;

void loop(){
  LCD.clear(); LCD.home();
  LCD.setCursor(0,0); LCD.print(encoder.get(hardware::R_ENCODER_));
  LCD.setCursor(0,1); LCD.print(encoder.get(hardware::L_ENCODER_));

  maneuver.loop();

  if (stopbutton()){
    driver.stop();
    while (!startbutton()){
      if (stopbutton()) mode = (mode + 1) % 2;
      distance = knob(6) - 512;
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0);
      if (mode == 0){
        LCD.print("distance");
        LCD.setCursor(0,1); LCD.print(distance);
      }else{
        LCD.print("degrees");
        LCD.setCursor(0,1); LCD.print(distance);
      }
      delay(50);
    }

    if (mode == 0) maneuver.straight(distance);
    else maneuver.turn(distance);
  }
  delay(50);
}
*/

// loop for claw testing
/*
void loop()
{
  int state = 0;
  int left = 0;
  while (!startbutton())
  {
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
  if (state < 300)
  {
    LCD.setCursor(0,0); LCD.print("raise");
    if (left > 500) claw.raise(sequences::LEFT_CLAW);
    else claw.raise(sequences::RIGHT_CLAW);
  }
  else if (state < 600)
  {
    LCD.setCursor(0,0); LCD.print("grab");
    if (left > 500) claw.grab(sequences::LEFT_CLAW);
    else claw.grab(sequences::RIGHT_CLAW);
  }
  else
  {
    LCD.setCursor(0,0); LCD.print("release");
    if (left > 500) claw.release(sequences::LEFT_CLAW);
    else claw.release(sequences::RIGHT_CLAW);
  }
  while (!claw.loop());
  delay(500);
}
*/
