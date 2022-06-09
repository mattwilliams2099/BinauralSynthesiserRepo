/*
  ==============================================================================

    Oscillator.h
    Created: 28 Apr 2022 9:04:44am
    Author:  Matthew Williams

  ==============================================================================
*/

#pragma once
#define WAVETABLE_LENGTH 64
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>


class OscillatorClass
{

public:
    enum shape : unsigned int
    {
        sine, saw, triangle, square
    };

    OscillatorClass(float samplerate = 44100.0f);
    float oscillatorProcess();
    void setFrequency(float newFrequency);
    void setSampleRate(float newValue) { sampleRate = newValue; }
    void setWaveShape(shape waveShape) { fillWavetable(waveShape); }
    void setFineFreq(float newFineFreq) { fineFreq = newFineFreq; setFrequency(frequency); }
    void setOctave(int newOctave);
    void stop();
    bool isPlaying();

private:
    float wavetable[WAVETABLE_LENGTH];
    float fineFreq = 1.0f;
    float octave = 1.0f;
    float frequency;
    float sampleRate;
    float index = 0.0f;
    float indexIncrement = 0.0f;
    float linearInterpolate();
    void fillWavetable(shape waveShape);
};