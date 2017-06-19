#include "configs/configs.h"

#if DEVICE()
#include <phys253.h>          
#include <LiquidCrystal.h>     
#endif  // DEVICE()

void setup()
{
#if DEVICE()  
  #include <phys253setup.txt>
  Serial.begin(9600) ;
#endif  // DEVICE()
}

void loop()
{

}

#if !DEVICE()
int main(void)
{
  setup();
  while(true) loop();
}
#endif  // !DEVICE()