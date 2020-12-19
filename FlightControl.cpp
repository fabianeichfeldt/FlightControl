#include <Wire.h>
#include "src/CopterSpeed.h"
#include "src/PIDController.h"
#include "src/Gyroscope.h"
#include "src/MotorControl.h"
#include "src/RemoteControl.h"

Gyroscope gyro(Wire, 0x68);
PIDController controllerX(-6, 0, 0); // i = -0.5, d = -2.0
PIDController controllerY(-6, 0, 0); // i = -0.5, d = -2.0
PIDController controllerZ(0, 0, 0);
RemoteControl remote;

CopterSpeed CalculateMotorSpeeds(Rotation currentPos, ControlData targetRotation);

void setup() {
  Serial.begin(115200);
  DDRB |= B11110000;

  MotorControl::SetSpeed(CopterSpeed::GetDefault());
  /* SREG |= B10000000;
   PCICR |= 1;
   PCMSK0 |= 1;*/

  gyro.SetAccOffsets(-437, 1662, 14351);
  gyro.SetGyroOffsets(-312, 154, -326);
  gyro.Init();

  remote.Init();
}

unsigned int speedHasBeenZero = 0;

void loop() {
  Rotation currentPos = gyro.GetRotation();
  remote.ReadData();

  if (remote.HasConfigurationData()) {
    return;
  }

  ControlData targetData = remote.GetControlData();

  if (targetData.GetSpeed() == MotorControl::MINIMUM_SPEED)
    speedHasBeenZero = true;

  if (!speedHasBeenZero)
    return;

  CopterSpeed speed = CalculateMotorSpeeds(currentPos, targetData);
  MotorControl::SetSpeed(speed);
}

CopterSpeed CalculateMotorSpeeds(Rotation currentPos, ControlData targetData) {
  int correctX = 0, correctY = 0, correctZ = 0;
  Rotation targetRotation = targetData.GetRotation();
  int motorSpeed = targetData.GetSpeed();

  if (motorSpeed > 1100) {
    correctX = controllerX.GetResponse(targetRotation.GetX() - currentPos.GetX());
    correctY = controllerY.GetResponse(targetRotation.GetY() - currentPos.GetY());
    correctZ = controllerZ.GetResponse(targetRotation.GetZ() - currentPos.GetZ());
  }

  int frontLeft = (motorSpeed - correctY + motorSpeed - correctZ) / 2;
  int rearRight = (motorSpeed + correctY + motorSpeed - correctZ) / 2;
  int frontRight = (motorSpeed - correctX + motorSpeed + correctZ) / 2;
  int rearLeft = (motorSpeed + correctX + motorSpeed + correctZ) / 2;

  return CopterSpeed(frontLeft, frontRight, rearLeft, rearRight);
}
