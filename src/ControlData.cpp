#include "ControlData.h"
#include "MotorControl.h"

ControlData ControlData::GetDefault() {
  return ControlData();
}

int ControlData::GetSpeed() {
  return speed;
}

Rotation ControlData::GetRotation() {
  return rotation;
}

ControlData::ControlData(): rotation(0, 0, 0) {
  speed = MotorControl::MINIMUM_SPEED;
}

void ControlData::SetSpeed(int speed) {
  this->speed = speed;
}

void ControlData::SetRotation(Rotation rotation) {
  this->rotation = rotation;
}
