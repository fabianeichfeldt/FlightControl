#include "ControlData.h"
#include "MotorControl.h"

ControlData ControlData::GetDefault() {
  return ControlData();
}void SetSpeed();

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
  speed = speed;
}

void ControlData::SetRotation(Rotation rotation) {
  rotation = rotation;
}
