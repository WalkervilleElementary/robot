#include "hardware/drivetrain.h"

namespace hardware {

Drivetrain::Drivetrain(MotorController& leftMotor, MotorController& rightMotor, const Encoder& leftEncoder, const Encoder& rightEncoder, const LineSensor& lineSensor) :
	m_leftMotor(leftMotor), m_rightMotor(rightMotor), m_leftEncoder(leftEncoder), m_rightEncoder(rightEncoder), m_lineSensor(lineSensor)
{
	m_command = IDLE;
}

void Drivetrain::commandDriveStraight(int32_t distance, int16_t speed) {
	m_leftEncoderSetpoint = m_leftEncoder.getDisplacement() + distance;
	m_rightEncoderSetpoint = m_rightEncoder.getDisplacement() + distance;
	m_command == DRIVE_ENCODER;
}

bool Drivetrain::readyForCommand() {
	return m_command == IDLE;
}

void Drivetrain::tick() {
	if (m_command == IDLE) {
		m_leftMotor.stop();
		m_rightMotor.stop();
	}
	else if (m_command == DRIVE_ENCODER) {

	}
}

int32_t Drivetrain::leftEncoderError() {
	return m_leftEncoder.getDisplacement() - m_leftEncoderSetpoint;
}

int32_t Drivetrain::rightEncoderError() {
	return m_leftEncoder.getDisplacement() - m_leftEncoderSetpoint;
}

Drivetrain::PIDController::PIDController() {
}

void Drivetrain::PIDController::setParameters(float P, float I, float D) {
	kP = P;
	kI = I;
	kD = D;
}

void Drivetrain::PIDController::init(float error) {
	integral = 0;
	prevError = error;
}

float Drivetrain::PIDController::run(float error) {
	integral = (integral + error) * 0.95;
	float derivative = prevError - error;
	prevError = error;
	return kP * error + kI * integral + kD * derivative;
}

}