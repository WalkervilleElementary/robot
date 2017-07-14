#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>
#include "hardware/driver.h"
#include "hardware/ir.h"
#include "sequences/tape.h"

hardware::Driver driver;
hardware::Ir ir;
sequences::Tape tape;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  tape.setup(ir, driver);
}

void loop()
{
#if DEBUG()
  LCD.clear(); LCD.home();
#endif  // DEBUG()

  tape.loop();

  if (stopbutton())
  {
    tape.update();
    ir.update();
  }

  delay(50);
}

