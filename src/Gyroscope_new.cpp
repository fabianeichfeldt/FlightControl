#include "Wire.h"
#include "Arduino.h"
#include "Rotation.h"
#include "./Gyroscope_new.h"

Gyroscope_new::Gyroscope_new(TwoWire wire, int address): rotation(0, 0, 0)
{
    this->wire = wire;
    this->address = address;
    gyroScale = 1.0/65.5; //movement in grad per second
    offsetGyroX = 0;
    offsetGyroY = 0;
    offsetGyroZ = 0;
    offsetAccX = 0;
    offsetAccY = 0;
    offsetAccZ = 0;
    lastTimeStamp = micros();
}

void Gyroscope_new::Init()
{
  wire.begin();
  wire.beginTransmission(0x68);
  wire.write(0x6B);
  wire.write(0x00);
  wire.endTransmission();
  //Configure the accelerometer (+/-8g)
  wire.beginTransmission(0x68);
  wire.write(0x1C);
  wire.write(0x10);
  wire.endTransmission();
  //Configure the gyro (500dps full scale)
  wire.beginTransmission(0x68);
  wire.write(0x1B);
  wire.write(0x08);
  wire.endTransmission();

  rotation = GetInitialOrientation();
}

void Gyroscope_new::SetGyroOffsets(int offsetGyroX, int offsetGyroY, int offsetGyroZ)
{
    this->offsetGyroX = offsetGyroX;
    this->offsetGyroY = offsetGyroY;
    this->offsetGyroZ = offsetGyroZ;
}

void Gyroscope_new::SetAccOffsets(float offsetAccX, float  offsetAccY, float  offsetAccZ)
{
    this->offsetAccX = offsetAccX;
    this->offsetAccY = offsetAccY;
    this->offsetAccZ = offsetAccZ;
}

Rotation Gyroscope_new::GetRotation()
{
  wire.beginTransmission(address);
  wire.write(0x3B);
  wire.endTransmission();
  wire.requestFrom(address, 14);  // request a total of 14 registers
  while(wire.available() < 14);

  Rotation accRotation = ReadAccRotation();
  wire.read()<<8|wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  Rotation gyroRotation = ReadGyroRotation();

  Rotation newCombinedRotation = Rotation(
          ComplementFilter(0.9996, gyroRotation.GetX(), accRotation.GetX()),
          ComplementFilter(0.9996, gyroRotation.GetY(), accRotation.GetY()),
          ComplementFilter(0.9996, gyroRotation.GetZ(), accRotation.GetZ())
          );

//  Serial.print(accRotation.GetX());Serial.print(" ");
//  Serial.print(accRotation.GetY());Serial.print(" ");
//  Serial.print(gyroRotation.GetX());Serial.print(" ");
//  Serial.print(gyroRotation.GetY());Serial.print(" ");

  rotation = Rotation(
    ComplementFilter(0.1, rotation.GetX(), newCombinedRotation.GetX()),
    ComplementFilter(0.1, rotation.GetY(), newCombinedRotation.GetY()),
    ComplementFilter(0.1, rotation.GetZ(), newCombinedRotation.GetZ())
  );

  return rotation;
}

Rotation Gyroscope_new::ReadGyroRotation()
{
  long currentTime = micros();
  float timePerCycleInSeconds = MicroSecondsToSeconds(currentTime - lastTimeStamp);
  lastTimeStamp = currentTime;

  float tmp = (ReadTwoBytes() - offsetGyroX) * gyroScale * timePerCycleInSeconds;
  float gyX = rotation.GetX() + tmp;  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  float gyY = rotation.GetY() + (ReadTwoBytes() - offsetGyroY) * gyroScale * timePerCycleInSeconds;  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  float gyZ = (ReadTwoBytes() - offsetGyroZ) * gyroScale * timePerCycleInSeconds;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //correct z axis movements
  return Rotation(gyX + gyY * sin(gyZ * PI / 180), gyY - gyX * sin(gyZ * PI / 180), gyZ);
}

Rotation Gyroscope_new::ReadAccRotation()
{
  long acX = (ReadTwoBytes());  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  long acY = (ReadTwoBytes());  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  long acZ = ReadTwoBytes();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  if(acX == 0 && acY == 0 && acZ == 0)
      return rotation;

  long acc_vector = sqrt(acX * acX + acY * acY + acZ * acZ);
  const float rad2Deg = 180.0 / PI;

  return Rotation((asin((float)acY/acc_vector) * rad2Deg) - offsetAccX, (asin((float)acX/acc_vector) * -rad2Deg) - offsetAccY, 0);
}

float Gyroscope_new::ComplementFilter(float weight1, float value1, float value2)
{
  return weight1 * value1 + (1 - weight1) * value2;
}

Rotation Gyroscope_new::GetInitialOrientation()
{
  float sumOfX = 0;
  float sumOfY = 0;

  long gyroX = 0, gyroY = 0, gyroZ = 0;
  int samples = 20;
  for(int i=0; i<samples; i++)
  {
    wire.beginTransmission(address) ;
    wire.write(0x3B);
    wire.endTransmission();
    wire.requestFrom(address, 14);
    while(wire.available() < 14);

    Rotation accRotation = ReadAccRotation();
    ReadTwoBytes(); //temp, don't care
    gyroX += ReadTwoBytes();
    gyroY += ReadTwoBytes();
    gyroZ += ReadTwoBytes();

    sumOfX += accRotation.GetX();
    sumOfY += accRotation.GetY();
    delay(50);
  }

  SetGyroOffsets(gyroX / samples, gyroY / samples, gyroZ / samples);

  Rotation rot =  Rotation(sumOfX / samples, sumOfY / samples, 0);
  Serial.print("initial x: "); Serial.println(rot.GetX());
  Serial.print("initial y: "); Serial.println(rot.GetY());
  return rot;
}

int Gyroscope_new::ReadTwoBytes()
{
  return (wire.read()<<8 | wire.read());
}

float Gyroscope_new::MicroSecondsToSeconds(long microSeconds)
{
    return microSeconds/1000000.0;
}
