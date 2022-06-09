/*
  ==============================================================================

    ITD.h
    Created: 21 Apr 2022 3:50:00pm
    Author:  Matthew Williams

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include "DelayLine.h"
#include <JuceHeader.h>
#include <cmath>
#include <iostream>
#include <fstream>
class ITDClass
{
private: 
    DelayLineClass delayLine;
    void setDelay();
    float distance = 1;
    float distanceCoefficient[2];
    float sampleRate;
    float samplePeriod;
    float headWidth = 0.075;
    void loadCoordinateDatabase();
    float coordinatesDatabase[2304][3];
    int index;
    float distanceCo;
    float azimuth;

public:
    ITDClass(float sampleRate = 48000);
    float process(float sample, int channel);
    void setAzimuth(float newAzimuth) { azimuth = newAzimuth; }
    void setIndex(int newIndex);
    void setDistance(float newDistance);
    float getCoefficient(int channel) { return distanceCoefficient[channel]; }

    void setHeadwidth(float newHeadWidth) { headWidth = newHeadWidth; }
    void setSampleRate(float newSampleRate) { sampleRate = newSampleRate;     samplePeriod = 1.0f / sampleRate; }
};