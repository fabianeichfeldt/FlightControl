#include "CopterSpeed.h"

class MotorControl
{
private:
  MotorControl();
  static int LimitMotorSpeed(int speed);
public:
  static const int MAXIMUM_SPEED = 2000;
  static const int MINIMUM_SPEED = 1000;
  static void SetSpeed(CopterSpeed speed);
};