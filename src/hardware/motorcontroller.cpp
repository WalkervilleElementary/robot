#include "hardware/motorcontroller.h"
 
#include <Arduino.h>
#include <phys253pins.h>

namespace hardware {

MotorController::MotorController(uint8_t directionPin, uint8_t enablePin, bool reversed) {
	 m_directionPin = directionPin;
	 m_enablePin = enablePin;
	 m_reversed = reversed;
}

 MotorController::MotorController(uint8_t output) {
	 m_directionPin = Motor_Direction[output];
	 m_enablePin = Motor_EnableDigital[output];
}
 
 void MotorController::set(int16_t power) {
	if (power > 255) {
		power = 255;	
	}
	if (power < -255) {
		power = -255;
	}
	if (m_reversed) power = -power;
	if (power >= 0) {
		digitalWrite(m_directionPin, HIGH);
		analogWrite(m_enablePin, abs(power));
	}
	else {
		digitalWrite(m_directionPin, LOW);
		analogWrite(m_enablePin, abs(power));
	}
}

void MotorController::stop() {
	set(0);
}

}