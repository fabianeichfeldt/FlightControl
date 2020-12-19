#pragma once

#include "Arduino.h"

class Rotation
{
    private:
        float angleX;
        float angleY;
        float angleZ;
        
    public:
        Rotation(float angleX, float angleY, float angleZ);
        float GetX();
        float GetY();
        float GetZ();

  static Rotation Default();
};