#include "hardware/limitmotorcontroller.h"

namespace hardware {

LimitMotorController::LimitMotorController(MotorController& motor, const Switch& extendLimit, const Switch& retractLimit) :
	m_motor(motor), m_extendLimit(extendLimit), m_retractLimit(retractLimit)
{
	m_state = INACTIVE;
}

bool LimitMotorController::extend(int16_t power) {
	m_motorPower = power;
	m_state = EXTEND;
	return m_extendLimit.get();
}

bool LimitMotorController::retract(int16_t power) {
	m_motorPower = power;
	m_state = RETRACT;
	return m_retractLimit.get();
}

void LimitMotorController::stop() {
	m_motorPower = 0;
	m_motor.stop();
	m_state = INACTIVE;
}

void LimitMotorController::tick() {
	if (m_state == INACTIVE) {
		m_motor.stop();
	}
	else if (m_state == EXTEND) {
		if (m_extendLimit.get()) m_motor.stop();
		else m_motor.set(m_motorPower);
	}
	else if (m_state == RETRACT) {
		if (m_retractLimit.get()) m_motor.stop();
		else m_motor.set(-m_motorPower);
	}
}

}