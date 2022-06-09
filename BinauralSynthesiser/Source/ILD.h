/*
  ==============================================================================

    ILD.h
    Created: 21 Apr 2022 3:50:26pm
    Author:  Matthew Williams

  ==============================================================================
*/

#pragma once
#include "Filter.h"
#include <iostream>
#include <fstream>
#include <JuceHeader.h>
class ILDClass
{
private:
    FilterClass bandPassFilter[4];
    void loadAmplitudeDatabase();
    float amplitudes[2304][4][2];
    int amplitudeIndex;
    float sampleRate;
    float filterRes = 1.0f;
public:
    ILDClass(float samplerate = 44100.0f);
    float process(float sample, int channel);
    void setIndex(int newIndex) { amplitudeIndex = newIndex; }
    void setFilterRes(float newFilterResonances);
    void setSampleRate(float newSampleRate);
    juce::String test;
};