/*
  ==============================================================================

    Binaural.h
    Created: 21 Apr 2022 3:49:36pm
    Author:  Matthew Williams

  ==============================================================================
*/

#pragma once
#include "ILD.h"
#include "ITD.h"
#include "Filter.h"

class BinauralClass
{
private:
    FilterClass LPF;
    FilterClass HPF;
    ILDClass ILD;// { sampleRate };
    ITDClass ITD;// { sampleRate };
    float azimuth = 0.0f;
    float elevation = 0.0f;

    float distance = 1.0f;
    float headWidth = 0.075;
    float sampleRate;
    int getCoordinates();
    int LFO = 0;
    int count = 0;
public:
    BinauralClass(float samplerate = 48000.0f);
    float getAzimuth() { return azimuth; }
    float process(float sample, int channel);
    void setAzimuth(int newAzimuth);
    void setElevation(int newElevation);
    void setDistance(float newDistance);
    void setHeadWidth(float newHeadwidth) { ITD.setHeadwidth(newHeadwidth); }
    void setFilterRes(float newFilterResonances) { ILD.setFilterRes(newFilterResonances); }
    void setSampleRate(float newSampleRate);
};