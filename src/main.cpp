#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>

#include "utils/encoderinterrupts.h"

#include "hardware/dcmotor.h"
#include "hardware/switch.h"
#include "hardware/encoder.h"
#include "hardware/limitmechanism.h"
#include "hardware/frequencysensor.h"
#include "hardware/linesensor.h"
#include "hardware/drivetrain.h"
#include "hardware/servomotor.h"
#include "hardware/clawmechanism.h"

/*#include "hardware/driver.h"
#include "hardware/qrd.h"
#include "hardware/encoder.h"
#include "hardware/beacon.h"

#include "sequences/claw.h"
#include "sequences/tape.h"
#include "sequences/platform.h"
#include "sequences/maneuver.h"

#include "stages/gate.h"
#include "stages/pickup.h"*/

/*hardware::Driver driver;
hardware::Qrd qrd;
hardware::Encoder encoder;
hardware::Beacon beacon;

sequences::Claw claw;
sequences::Maneuver maneuver(driver, encoder);
sequences::Platform platform(driver);
sequences::Tape tape(qrd, beacon, driver);


stages::Gate gate(tape, beacon, encoder);
stages::Pickup pickup(qrd, claw, maneuver, tape);
stages::Zipline zipline(tape, platform, maneuver, beacon, driver, encoder);*/

using namespace hardware;

void setup(){
  portMode(0, INPUT);
  portMode(1, OUTPUT);
  
  LCD.begin(16,2) ;

  Serial.begin(9600);
  
  bool mirror = false;

  ServoMotor leftGrabServo(SERVO_0(), 10);
  ServoMotor leftRaiseServo(SERVO_1(), 10);
  ServoMotor rightGrabServo(SERVO_2(), 10);
  ServoMotor rightRaiseServo(SERVO_3(), 10);

  int16_t leftClawLiftPositions[5] = {0,0,0,0,0};
  int16_t leftClawGrabPositions[2] = {0,0};
  int16_t rightClawLiftPositions[5] = {0,0,0,0,0};
  int16_t rightClawGrabPositions[2] = {0,0};
  ClawMechanism leftClaw(leftRaiseServo, leftGrabServo, leftClawLiftPositions, leftClawGrabPositions);
  ClawMechanism rightClaw(rightRaiseServo, rightGrabServo, rightClawLiftPositions, rightClawGrabPositions);

  ClawMechanism& primaryClaw = mirror ? rightClaw : leftClaw;
  ClawMechanism& secondaryClaw = mirror ? leftClaw : rightClaw;

  DCMotor leftMotor(2);
  DCMotor rightMotor(3, true);
  Encoder leftEncoder(0);
  Encoder rightEncoder(1, true);

  DCMotor& insideMotor = mirror ? rightMotor : leftMotor;
  DCMotor& outsideMotor = mirror ? leftMotor : rightMotor;
  Encoder& insideEncoder = mirror ? rightEncoder : leftEncoder;
  Encoder& outsideEncoder = mirror ? leftEncoder : rightEncoder;

  Switch platformRaiseLimit(6, LOW);
  Switch platformLowerLimit(7, LOW);
  DCMotor platformMotor(0);
  LimitMechanism platform(platformMotor, platformRaiseLimit, platformLowerLimit);

  Switch startButton(50, LOW);
  Switch stopButton(49, LOW);
  
  FrequencySensor leftBeaconSensor(4);
  FrequencySensor rightBeaconSensor(5);

  LineSensor tapeSensor = mirror ? LineSensor(3,2,1,0) : LineSensor(0,1,2,3);
  
  Drivetrain drive(insideMotor, outsideMotor, insideEncoder, outsideEncoder, tapeSensor);


  enableEncoderInterrupts();
  
  while (true) {
    startButton.tick();
    stopButton.tick();
    leftEncoder.tick();
    rightEncoder.tick();
    LCD.setCursor(0,0);
    LCD.print(getEncoderCount(0));
    LCD.print("  ");
    LCD.setCursor(8,0);
    LCD.print(getEncoderCount(1));
    LCD.print("  ");
    LCD.setCursor(0,1);
    LCD.print(leftEncoder.getVelocity());
    LCD.print("  ");
    LCD.setCursor(8,1);
    LCD.print(rightEncoder.getVelocity());
    LCD.print("  ");
    /*Serial.print(getEncoderCount(0));
    Serial.print(",");
    Serial.print(getEncoderCount(1));*/
    /*Serial.print(getEncoderState(0,1));
    Serial.print(getEncoderState(0,0));
    Serial.print(getEncoderState(1,1));
    Serial.print(getEncoderState(1,0));*/
    //Serial.print("\n");
    delay(50);
  }
  
  /*int buttonPressCount = 0;
  while (true) {
    startButton.tick();
    if (startButton.released()) buttonPressCount++;
    LCD.setCursor(0,0);
    LCD.print(startButton.get());
    LCD.print(buttonPressCount);
    delay(50);
  }*/
  
  /*while (true) {

    leftEncoder.tick();
    rightEncoder.tick();
    drive.tick();
    startButton.tick();
    if (drive.readyForCommand() && startButton.released()) {
      drive.commandDriveStraight(255, 255);
    }
  }*/
}

void loop() {}
/*
uint32_t loop_delay;

void setup(){
  #include <phys253setup.txt>
  loop_delay = LOOP_DELAY();
}

// loop for testing zipline routine
int state = 0;
void loop() {
  if (stopbutton()) {
    zipline.update();
    platform.update();
    state = 0;  // restart routine after update
  }

  switch (state) {
    case 0:  // only testing dead reckoning right now
      zipline.set_state(3);
      state++;
    case 1:
      if (zipline.loop())
        state++;
      break;
  }

  delay(loop_delay);
}*/

// loop for testing platform
/*
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

  if (stopbutton()) {
    platform.update();
    state = 0;  // restart routine after updating
  }

  // run through raise/lower routine once
  switch (state) {
    case 0:  // raise
      platform.raise();
      delay(1000);  // pause for breath?
      state++;
    case 1:  // raising
      platform.lower();
      state++;
  }
  delay(loop_delay);
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
void loop(){
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  LCD.setCursor(0,1); LCD.print("Tape");
  tape.loop();

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
      distance = knob(6);
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0);
      if (mode == 0) LCD.print("distance");
      else LCD.print("degrees");
      LCD.setCursor(0,1); LCD.print(distance);
      delay(50);
    }

    if (mode == 0) maneuver.straight(distance);
    else maneuver.turn(distance);
  }
  delay(50);
}


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
