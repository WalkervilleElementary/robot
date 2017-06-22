#include "configs.h"


#if DEVICE()
#include "driver.h"
#include "ir.h"
#include "tape.h"

#include <phys253.h>
#include <LiquidCrystal.h>
#else  // end DEVICE(); start !DEVICE()
#include "hardware/driver.h"
#include "hardware/ir.h"
#include "sequences/tape.h"
#endif  // end !DEVICE()

hardware::Driver driver;
hardware::Ir ir;
sequences::Tape tape;

void setup()
{
#if DEVICE()
  #include <phys253setup.txt>
  Serial.begin(9600) ;
#endif  // DEVICE()
  tape.setup(ir, driver);
}

void loop()
{
#if DEBUG()
#if DEVICE()
  LCD.clear(); LCD.home();
#endif  // DEVICE()
#endif  // DEBUG()

  tape.loop();

#if USE_UPDATE()
#if DEVICE()
  if (stopbutton())
  {
    tape.update();
    ir.update();
  }
#endif  // DEVICE()
#endif  // USE_UPDATE()

#if DEVICE()
  delay(10);
#endif  // DEVICE()
}

#if !DEVICE()
int main(void)
{
  setup();
  while(true) loop();
}
#endif  // !DEVICE()
