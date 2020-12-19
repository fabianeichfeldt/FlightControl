#pragma once

#include "Wire.h"
#include "Rotation.h"

class Gyroscope
{
    private:
        TwoWire wire;
        int address;
        int offsetGyroX;
        int offsetGyroY;
        int offsetGyroZ;
        int offsetAccX;
        int offsetAccY;
        int offsetAccZ;
        Rotation rotation;
        float gyroScale;
        float accScale;
        long lastTimeStamp;
        float CalculateXAngleFromAcc(float x, float y, float z);
        float CalculateYAngleFromAcc(float x, float y, float z);
        float ComplementFilter(float weight1, float value1, float value2);
        Rotation GetInitialOrientation();
        int ReadTwoBytes();
        float MicroSecondsToSeconds(long microSeconds);
        
    public:
        Gyroscope(TwoWire wire, int address);
        void Init();
        void SetGyroOffsets(int offsetGyroX, int offsetGyroY, int offsetGyroZ);
        void SetAccOffsets(int offsetAccX, int offsetAccY, int offsetAccZ);
        Rotation GetRotation();
};