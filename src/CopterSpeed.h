#pragma once

class CopterSpeed
{
    public:
        CopterSpeed(int frontLeft, int frontRight, int rearLeft, int rearRight);

  CopterSpeed();

  int GetFrontLeftSpeed();
        int GetRearLeftSpeed();
        int GetFrontRightSpeed();
        int GetRearRightSpeed();

  static CopterSpeed GetDefault();

private:
        int frontLeft;
        int frontRight;
        int rearLeft;
        int rearRight;
};