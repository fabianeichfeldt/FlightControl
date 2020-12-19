#include "Wire.h"
#include "Arduino.h"
#include "Rotation.h"
#include "./Gyroscope.h"

Gyroscope::Gyroscope(TwoWire wire, int address): rotation(0, 0, 0)
{
    this->wire = wire;
    this->address = address;
    gyroScale = 1.0/131; //movement in grad per second
    accScale = 1.0/16383; // LSB/g

    offsetGyroX = 0;
    offsetGyroY = 0;
    offsetGyroZ = 0;
    offsetAccX = 0;
    offsetAccY = 0;
    offsetAccZ = 0;
    lastTimeStamp = micros();
}

void Gyroscope::Init()
{
    wire.begin();
    wire.beginTransmission(address);
    wire.write(0x6B);
    wire.write(0);
    wire.endTransmission(true);
    delay(30);
    wire.beginTransmission(address);
    wire.write(25);
    wire.write(10);
    wire.endTransmission(true);
    wire.beginTransmission(address);
    wire.write(35);
    wire.write(B01111000);
    wire.endTransmission(true);
    delay(30);

    rotation = GetInitialOrientation();
}

void Gyroscope::SetGyroOffsets(int offsetGyroX, int offsetGyroY, int offsetGyroZ)
{
    offsetGyroX = offsetGyroX;
    offsetGyroY = offsetGyroY;
    offsetGyroZ = offsetGyroZ;
}

void Gyroscope::SetAccOffsets(int offsetAccX, int offsetAccY, int offsetAccZ)
{
    offsetAccX = offsetAccX;
    offsetAccY = offsetAccY;
    offsetAccZ = offsetAccZ;
}

Rotation Gyroscope::GetRotation()
{
  long currentTime = micros();
  float timePerCycleInSeconds = MicroSecondsToSeconds(currentTime - lastTimeStamp);
  lastTimeStamp = currentTime;

  wire.beginTransmission(address);
  wire.write(0x3B);

  wire.endTransmission(false);
  wire.requestFrom(address,14,true);  // request a total of 14 registers
  
  float AcX = ReadTwoBytes() * accScale;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  float AcY = ReadTwoBytes() * accScale;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  float AcZ = ReadTwoBytes() * accScale;  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  
  wire.read()<<8|wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  
  float GyX = rotation.GetX() + (ReadTwoBytes() - offsetGyroX) * gyroScale * timePerCycleInSeconds;  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  float GyY = rotation.GetY() + (ReadTwoBytes() - offsetGyroY) * gyroScale * timePerCycleInSeconds;  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  float GyZ = (ReadTwoBytes() - offsetGyroZ) * gyroScale * timePerCycleInSeconds;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  rotation = Rotation(
    ComplementFilter(0.98, GyX, ComplementFilter(0.8, rotation.GetX(), CalculateXAngleFromAcc(AcX, AcY, AcZ))),
    ComplementFilter(0.98, GyY, ComplementFilter(0.8, rotation.GetY(), CalculateYAngleFromAcc(AcX, AcY, AcZ))),
    ComplementFilter(0.5, rotation.GetZ(), ComplementFilter(0.95, rotation.GetZ(), AcZ))
  );

  return rotation;
}

float Gyroscope::CalculateYAngleFromAcc(float x, float y, float z)
{
  return atan2(-x, sqrt(y*y + z*z)) * 180/PI;
}

float Gyroscope::CalculateXAngleFromAcc(float x, float y, float z)
{
  return atan2(y, sqrt(x*x + z*z)) * 180/PI;
}

float Gyroscope::ComplementFilter(float weight1, float value1, float value2)
{
  return weight1 * value1 + (1 - weight1) * value2;
}

Rotation Gyroscope::GetInitialOrientation()
{
  float sumOfX = 0;
  float sumOfY = 0;

  int samples = 20;
  for(int i=0; i<samples; i++)
  {
    Wire.beginTransmission(address) ;
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(address, 6, true);

    int x = ReadTwoBytes();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    int y = ReadTwoBytes();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    int z = ReadTwoBytes();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    sumOfX += CalculateXAngleFromAcc(x, y, z);
    sumOfY += CalculateYAngleFromAcc(x, y, z);
    delay(50);
  }

  return Rotation(sumOfX /= samples, sumOfY /= samples, 0);
}

int Gyroscope::ReadTwoBytes()
{
  return (wire.read()<<8 | wire.read());
}

float Gyroscope::MicroSecondsToSeconds(long microSeconds)
{
    return microSeconds/1000000.0;
}
