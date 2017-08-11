#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>

#include "utils/filter.h"



void setup(){
  portMode(0, INPUT);
  portMode(1, INPUT);

  LCD.begin(16,2) ;
  Serial.begin(115200) ;

}


void loop() {
  const size_t num_samples = 420;
  uint8_t samples[num_samples];
  sample_signal(0, num_samples, 1, samples);
  for (size_t i = 0; i < num_samples; i++) {
    Serial.println(samples[i]);
  }
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print(detect_frequency, num_samples, 100000, 615385);
  delay(1000);
}
