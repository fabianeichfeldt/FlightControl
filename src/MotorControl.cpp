#include <Arduino.h>
#include "./CopterSpeed.h"
#include "./MotorControl.h"

void MotorControl::SetSpeed(CopterSpeed speed) {
  speed = CopterSpeed(
          LimitMotorSpeed(speed.GetFrontLeftSpeed()),
          LimitMotorSpeed(speed.GetFrontRightSpeed()),
          LimitMotorSpeed(speed.GetRearLeftSpeed()),
          LimitMotorSpeed(speed.GetRearRightSpeed())
          );

  unsigned long currentTime;
  byte i=0;
  long startTime =micros();
  PORTB |= B11110000;
  long timerFroLe = speed.GetFrontLeftSpeed() + startTime;
  long timerReaLe = speed.GetRearLeftSpeed() + startTime;
  long timerFroRi = speed.GetFrontRightSpeed() + startTime;
  long timerReaRi = speed.GetRearRightSpeed() + startTime;

  bool frontLeftEnable = true, frontRightEnable = true, rearLeftEnable = true, rearRightEnable = true;

  while (i<4)
  {
    currentTime = micros();
    if(frontLeftEnable && currentTime > timerFroLe)
    {
      PORTB &= B01111111;
      frontLeftEnable = false;
      i++;
    }
    if(rearRightEnable && currentTime > timerReaRi)
    {
      PORTB &= B10111111;
      rearRightEnable = false;
      i++;
    }
    if(frontRightEnable && currentTime > timerFroRi)
    {
      PORTB &= B11101111;
      frontRightEnable = false;
      i++;
    }
    if(rearLeftEnable && currentTime > timerReaLe)
    {
      PORTB &= B11011111;
      rearLeftEnable = false;
      i++;
    }
  }
  //wait some time to guarantee a constant pwm/frequency
  while(micros() < (startTime + MAXIMUM_SPEED + 500));
}

MotorControl::MotorControl() {
}

int MotorControl::LimitMotorSpeed(int speed) {
  return (speed > MAXIMUM_SPEED) ? MAXIMUM_SPEED : (speed < MINIMUM_SPEED) ? MINIMUM_SPEED : speed;
}
