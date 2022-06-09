/*
  ==============================================================================

    Binaural.cpp
    Created: 21 Apr 2022 3:49:36pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include "Binaural.h"

BinauralClass::BinauralClass(float samplerate)
    : sampleRate(samplerate) 
{
    HPF.setHighPassCo(250, 0.5);
    LPF.setLowPassCo(250, 0.5);

}

float BinauralClass::process(float sample, int channel)
{
    float highFreq = HPF.filterProcess(sample, channel);
    float lowFreq = LPF.filterProcess(sample, channel);
    float ILDSample = ILD.process(-highFreq, channel);
    float delayedSample = ITD.process(ILDSample + lowFreq, channel);//ITD.process((ILDSample * pow((1 / distance), 2)) + (lowFreq * ITD.getCoefficient(channel)), channel);
    return delayedSample;
}

void BinauralClass::setSampleRate(float newSampleRate)
{
    ILD.setSampleRate(newSampleRate);
    ITD.setSampleRate(newSampleRate);
    HPF.setSampleRate(newSampleRate);
    LPF.setSampleRate(newSampleRate);

}


void BinauralClass::setAzimuth(int newAzimuth)
{
    if (newAzimuth <= 0)
        azimuth = abs(newAzimuth);
    else
        azimuth = 360 - newAzimuth;
    ITD.setAzimuth(static_cast<float>(azimuth * (M_PI / 180.0f)));
    ILD.setIndex(getCoordinates());
    ITD.setIndex(getCoordinates());
}

void BinauralClass::setElevation(int newElevation)
{
    elevation = newElevation;
    ILD.setIndex(getCoordinates());
    ITD.setIndex(getCoordinates());
}

void BinauralClass::setDistance(float newDistance)
{
    distance = newDistance;
    ITD.setDistance(distance);
}

int BinauralClass::getCoordinates()
{
    return (static_cast<int>(azimuth / 5) * 32) + elevation;
}


