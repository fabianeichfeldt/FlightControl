#include "./Rotation.h"

Rotation::Rotation(float angleX, float angleY, float angleZ)
{
    angleX = angleX; 
    angleY = angleY;
    angleZ = angleZ;
}

float Rotation::GetX()
{
    return angleX;
}

float Rotation::GetY()
{
    return angleY;
}

float Rotation::GetZ()
{
    return angleZ;
}

Rotation Rotation::Default() {
    return Rotation(0, 0, 0);
}
