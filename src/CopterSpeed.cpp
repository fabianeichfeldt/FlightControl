#include "./CopterSpeed.h"

CopterSpeed::CopterSpeed(int frontLeft, int frontRight, int rearLeft, int rearRight)
{
    frontLeft = frontLeft;
    frontRight = frontRight;
    rearLeft = rearLeft;
    rearRight = rearRight;
}

int CopterSpeed::GetFrontLeftSpeed()
{
    return frontLeft;
}

int CopterSpeed::GetRearLeftSpeed()
{
    return rearLeft;
}

int CopterSpeed::GetFrontRightSpeed()
{
    return frontRight;
}

int CopterSpeed::GetRearRightSpeed()
{
    return rearRight;
}

CopterSpeed CopterSpeed::GetDefault() {
    return CopterSpeed(0, 0, 0, 0);
}
