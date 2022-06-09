/*
  ==============================================================================

    LFO.h
    Created: 29 Apr 2022 11:31:23am
    Author:  Matthew Williams

  ==============================================================================
*/
#define _USE_MATH_DEFINES
#pragma once
#include <iostream>
#include <cmath>


#define WAVETABLE_LENGTH 64


class LFOClass
{
public:
    float process();
    enum shape : unsigned int
    {
        sine, sawUp, sawDown, triangle
    };
    void setRange(float newRange);
    void setOffset(float newOffset);
    void setFrequency(float newFrequency);
    void setShape(LFOClass::shape wave);
    void setSampleRate(float samplerate) { sampleRate = samplerate; }
    float getRange() { return range; }
    float getOffset() { return offset; }

private:
    float wavetable [WAVETABLE_LENGTH];
    float linearInterpolate();
    int LFOwave = 0;
    float index = 0;
    float indexIncrement = 0;
    float range = 2;
    float offset = 0;
    float frequency;
    float sampleRate = 48000;
    bool global = true;
};