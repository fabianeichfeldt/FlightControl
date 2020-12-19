#pragma once;

#include "CopterSpeed.h"
#include "Rotation.h"

class ControlData
{
private:
  Rotation rotation;
  int speed;

public:
  ControlData();
  static ControlData GetDefault();
  int GetSpeed();
  Rotation GetRotation();
  void SetSpeed(int speed);
  void SetRotation(Rotation rotation);
};