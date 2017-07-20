#include "sequences/platform.h"
#include <phys253.h>
#include <LiquidCrystal.h>

namespace sequences
{
Platform::Platform()
{
  state_ = 0;
  raise_speed_ = PLATFORM_RAISE_SPEED();
  lower_speed_ = PLATFORM_LOWER_SPEED();
  backup_speed_ = BACKUP_SPEED();
#if USE_UPDATE()
  update_state_ = 0;
#endif  // USE_UPDATE()
}

void Platform::setup(const hardware::Driver &driver)
{
  driver_ = driver;
}

bool Platform::loop()
{
  switch (state_)
  {
    case 0:
      break;
    case 1:  // raising platform
      if (digitalRead(PLATFORM_UPPER_SWITCH()))
        motor.speed(PLATFORM_MOTOR(), raise_speed_);
      else
        state_++;
      return false;
    case 2:  // lower just enough so switch is no longer active
      if (!digitalRead(PLATFORM_UPPER_SWITCH()))
      {
        motor.speed(PLATFORM_MOTOR(), lower_speed_);
        return false;
      }
      else
        break;
    case 3:  // slowly lower while backing up
      if (digitalRead(PLATFORM_LOWER_SWITCH()))
      {
        motor.speed(PLATFORM_MOTOR(), lower_speed_);
        driver_.sendWheelVelocities(backup_speed_, backup_speed_);
        return false;
      }
  }
  state_ = 0;
  stop();
  return true;
}

void Platform::stop() {
  state_ = 0;
  motor.stop(PLATFORM_MOTOR());
  driver_.stop();
}

void Platform::raise() {
  state_ = 1;
}

void Platform::lower() {
  state_ = 3;
}

#if USE_UPDATE()
void Platform::update()
{
  stop();
  delay(200);
  while (!startbutton())
  {
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 2) update_state_ = 0;
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);

    int change = (start_val - end_val)/4 ;
    LCD.clear();  LCD.home() ;

    switch (update_state_)
    {
    case 0:
        raise_speed_ += change;
        LCD.setCursor(0,0); LCD.print("raise speed");
        LCD.setCursor(0,1); LCD.print(raise_speed_);
        break;
    case 1:
        lower_speed_ += change;
        LCD.setCursor(0,0); LCD.print("lower speed");
        LCD.setCursor(0,1); LCD.print(lower_speed_ * 0.5);
        break;
    case 2:
        backup_speed_ += change;
        LCD.setCursor(0,0); LCD.print("backup speed");
        LCD.setCursor(0,1); LCD.print(backup_speed_);
        break;
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace sequences
