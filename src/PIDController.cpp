#pragma once

#include "Arduino.h"
#include "./PIDController.h"

PIDController::PIDController(int p, int i, int d)
{
    pGain = p;
    iGain = i;
    dGain = d;
    lastTimeStamp = micros();
    lastError = 0;
    sumOfErrors = 0;
}

float PIDController::MicroSecondsToSeconds(long microSeconds)
{
    return microSeconds/1000000.0;
}

int PIDController::GetResponse(float error)
{
    long currentTime = micros();
    float timePerCycleInSeconds = MicroSecondsToSeconds(currentTime - lastTimeStamp);

    int response = (int) (pGain * error + iGain * timePerCycleInSeconds * sumOfErrors + dGain * (error - lastError)/timePerCycleInSeconds);
    
    lastTimeStamp = currentTime;
    lastError = error;
    sumOfErrors += error;

    return response;
}