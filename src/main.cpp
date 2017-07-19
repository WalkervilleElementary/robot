#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>

#include "hardware/driver.h"
#include "hardware/ir.h"
#include "hardware/encoder.h"
#include "hardware/beacon.h"

#include "sequences/claw.h"
#include "sequences/tape.h"
#include "sequences/zipline.h"
#include "sequences/maneuver.h"

#include "stages/gate.h"
#include "stages/pickup.h"

hardware::Driver driver;
hardware::Ir ir;
hardware::Encoder encoder;
hardware::Beacon beacon;

sequences::Claw claw;
sequences::Tape tape;
sequences::Zipline zipline;
sequences::Maneuver maneuver;

stages::Gate gate;
stages::Pickup pickup;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  tape.setup(ir, driver);
  maneuver.setup(driver, encoder);
  gate.setup(tape, beacon, encoder);
  pickup.setup(ir, claw, maneuver, tape);
}

int state = 0;
void loop()
{
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  if (stopbutton())
  {
    gate.update();
    // ir.update();
  }

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

/*
// loop for testing pickup

void loop()
{
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  pickup.loop();

  if (stopbutton())
  {
    pickup.update();
    tape.update();
    ir.update();
    gate.update();
    // ir.update();
  }
  delay(50);
}
*/

/*
// loop for testing maneuver

int distance = 0;

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
      distance = knob(6);
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print(distance);
      delay(50);
    }
    encoder.stop(hardware::R_ENCODER_);
    encoder.stop(hardware::L_ENCODER_);
    encoder.start(hardware::R_ENCODER_);
    encoder.start(hardware::L_ENCODER_);
    maneuver.straight(distance);
  }
  delay(50);
}
*/