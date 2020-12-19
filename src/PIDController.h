#pragma once

class PIDController
{
    private:
        int pGain;
        int iGain;
        int dGain;
        long lastTimeStamp;
        float sumOfErrors;
        float lastError;

        float MicroSecondsToSeconds(long microSeconds);
    public:
        PIDController(int p, int i, int d);
        int GetResponse(float error);
};