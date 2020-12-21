#pragma once

#include "Wire.h"
#include "Rotation.h"

class Gyroscope_new
{
    private:
        TwoWire wire;
        int address;
        int offsetGyroX;
        int offsetGyroY;
        int offsetGyroZ;
        float offsetAccX;
        float offsetAccY;
        float offsetAccZ;
        Rotation rotation;
        float gyroScale;
        long lastTimeStamp;
        Rotation ReadAccRotation();
        Rotation ReadGyroRotation();
        float ComplementFilter(float weight1, float value1, float value2);
        Rotation GetInitialOrientation();
        int ReadTwoBytes();
        float MicroSecondsToSeconds(long microSeconds);
        
    public:
        Gyroscope_new(TwoWire wire, int address);
        void Init();
        void SetGyroOffsets(int offsetGyroX, int offsetGyroY, int offsetGyroZ);
        void SetAccOffsets(float offsetAccX, float offsetAccY, float offsetAccZ);
        Rotation GetRotation();
};