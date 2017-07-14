#include "configs.h"

#include <phys253.h>
#include <LiquidCrystal.h>
#include "sequences/zipline.h"
#include <phys253pins.h>

const int LIMIT_SWITCH_RAISE = 7;
const int LIMIT_SWITCH_LOWER = 6;
const int MOTOR = ZIPLINE_MOTOR();
const int MOTOR_RAISE_SPEED = -176;
const int MOTOR_LOWER_SPEED = 72;

namespace sequences
{
Zipline::Zipline()
{

}

void Zipline::setup()
{
    
}

bool Zipline::loop()
{

}

void Zipline::stop()
{
    state = STOP;
}

bool Zipline::raise() {
    state = RAISE;
    if (digitalRead(LIMIT_SWITCH_RAISE)) {
        return false;
    }
    else return true;
}

bool Zipline::lower() {
    state = LOWER;
    if (digitalRead(LIMIT_SWITCH_LOWER)) {
        return false;
    }
    else return true;
}

#if USE_UPDATE()
void Zipline::update()
{
#if DEBUG()
    LCD.setCursor(0,0);
    LCD.print(digitalRead(LIMIT_SWITCH_RAISE));
    LCD.print(digitalRead(LIMIT_SWITCH_LOWER));
    LCD.print(state);
#endif  // DEBUG()
    
    if (!digitalRead(stopInput)) stop();
    if (!digitalRead(startInput)) {
        state = (state == RAISE) ? LOWER : RAISE;
    }
    if (state == STOP) {
        motor.stop(MOTOR);
    }
    else if (state == RAISE) {
        if (digitalRead(LIMIT_SWITCH_RAISE)) {
            motor.speed(MOTOR, MOTOR_RAISE_SPEED);
        }
        else {
            motor.stop(MOTOR);
        }
    }
    else if (state == LOWER) {
        if (digitalRead(LIMIT_SWITCH_LOWER)) {
            motor.speed(MOTOR, MOTOR_LOWER_SPEED);
        }
        else {
            motor.stop(MOTOR);
        }
    }
}
#endif  // USE_UPDATE()

}  // namespace sequences
