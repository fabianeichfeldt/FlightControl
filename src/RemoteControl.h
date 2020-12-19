#pragma once
#include "Arduino.h"
#include "ConfigData.h"
#include "ControlData.h"

class RemoteControl
{
private:
  ConfigData config;
  ControlData control;
  float GetFloatFromSerial(byte* buffer);
  void ReadFromSerial();
  float CalculateAngle(int p);

public:
  RemoteControl();
  void Init();
  void ReadData();
  byte HasConfigurationData();
  byte HasControlData();
  ConfigData GetConfigData();
  ControlData GetControlData();
};
