#include "configs/configs.h"
#include "hardware/driver.h"
#include "hardware/ir.h"
#include "sequences/tape.h"

#if DEVICE()
#include <phys253.h>          
#include <LiquidCrystal.h>     
#endif  // DEVICE()

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
  tape.loop();

#if USE_UPDATE()
#if DEVICE()
  if (stoptbutton())
  {
    tape.update();
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