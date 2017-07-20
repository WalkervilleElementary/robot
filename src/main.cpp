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

hardware::Driver driver;
hardware::Qrd qrd;
hardware::Encoder encoder;
hardware::Beacon beacon;

sequences::Claw claw;
sequences::Tape tape;
sequences::Platform platform;
sequences::Maneuver maneuver;

stages::Gate gate;
stages::Pickup pickup;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  tape.setup(qrd, driver);
  platform.setup(driver);
  maneuver.setup(driver, encoder);
  gate.setup(tape, beacon, encoder);
  pickup.setup(qrd, claw, maneuver, tape);
}

// loop for testing platform
<<<<<<< HEAD
=======
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
      delay(1000);  // pause for breath?
      platform.lower();
      state++;
    case 3:  // lowering
      if (platform.loop())
        state++;
      break;
  }

  delay(LOOP_DELAY());
}

// loop for testing gate routine
/*
>>>>>>> 3a60eb9... Platform logic completed and tested
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
<<<<<<< 51cfb3f2d5a969225682be39c51bdac9211338ac
  {
    platform.update();
    state = 0;  // restart routine after updating
  }
<<<<<<< HEAD

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
      delay(1000);  // pause for breath?
      platform.lower();
      state++;
    case 3:  // lowering
      if (platform.loop())
        state++;
      break;
  }
=======
    {
      gate.update();
      // qrd.update();
    }
>>>>>>> Platform raising and lowering

  delay(LOOP_DELAY());
}

// loop for testing gate routine
/*
int state = 0;
void loop()
{
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  if (stopbutton())
    {
      gate.update();
      // qrd.update();
    }

=======

>>>>>>> 51cfb3f... Implement claw
  // Tape follow loop
  switch (state)
  {
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

// loop for testing maneuver
/*
int distance = 0;
int mode = 0;

void loop()
{
  LCD.clear(); LCD.home();
  LCD.setCursor(0,0); LCD.print(encoder.get(hardware::R_ENCODER_));
  LCD.setCursor(0,1); LCD.print(encoder.get(hardware::L_ENCODER_));
  gate.loop();
  if (stopbutton())
  {
    driver.stop();
    while (!startbutton())
    {
      if (stopbutton()) mode = (mode + 1) % 2;
      distance = knob(6);
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0);
      if (mode == 0) LCD.print("distance");
      else LCD.print("degrees");
      LCD.setCursor(0,1); LCD.print(distance);
      delay(50);
    }
    encoder.stop(hardware::R_ENCODER_);
    encoder.stop(hardware::L_ENCODER_);
    encoder.start(hardware::R_ENCODER_);
    encoder.start(hardware::L_ENCODER_);

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
  while (!startbutton())
  {
    if (stopbutton())
      claw.update();
    state = knob(6);
    LCD.clear(); LCD.home();
    LCD.setCursor(0,0); LCD.print(state);
    delay(100);
  }
  LCD.clear(); LCD.home();
  LCD.setCursor(0,1); LCD.print(state);
  if (state < 300)
  {
    LCD.setCursor(0,0); LCD.print("raise");
    claw.raise(sequences::LEFT_CLAW);
  }
  else if (state < 600)
  {
    LCD.setCursor(0,0); LCD.print("grab");
    claw.grab(sequences::LEFT_CLAW);
  }
  else
  {
    LCD.setCursor(0,0); LCD.print("release");
    claw.release(sequences::LEFT_CLAW);
  }
  while (!claw.loop());
}
*/
