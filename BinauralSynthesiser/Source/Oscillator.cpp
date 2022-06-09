/*
  ==============================================================================

    Oscillator.cpp
    Created: 12 Apr 2022 2:34:05pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include "Oscillator.h"

OscillatorClass::OscillatorClass(float samplerate) : sampleRate(samplerate)
{
    //fillWavetable(square);
}

void OscillatorClass::fillWavetable(shape waveShape)
{
    std::fill_n(wavetable, WAVETABLE_LENGTH, 0.0f);
    float increment;
    float counter = 0.0f;
    switch (waveShape)
    {
    case sine: 
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            wavetable[i] = sin((2 * M_PI * static_cast<float>(i)) / static_cast<float>(WAVETABLE_LENGTH));
        }
        break;
    case saw: 
        counter = -1.0f;
        increment = 2.0f / static_cast<float>(WAVETABLE_LENGTH);
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            counter += increment;
            wavetable[i] = counter;
        }
        break;
    case triangle:
        increment = 4.0f / static_cast<float>(WAVETABLE_LENGTH);
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            if (i == WAVETABLE_LENGTH / 4 || i == (WAVETABLE_LENGTH / 4) * 3)
                increment = (-increment);
            counter += increment;
            wavetable[i] = counter;
        }
        break;
    case square:
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            if (i < WAVETABLE_LENGTH / 2)
                wavetable[i] = 1.0f;
            else
                wavetable[i] = -1.0f;
        }
        break;
    }
}

void OscillatorClass::setFrequency(float newFrequency)
{
    frequency = newFrequency;
    indexIncrement = (fineFreq * octave * frequency) * static_cast<float>(WAVETABLE_LENGTH) / sampleRate;
}

void OscillatorClass::setOctave(int newOctave)
{ 
    switch (newOctave)
    {
    case 0: 
        octave = 0.25f;
        break;
    case 1:
        octave = 0.5f;
        break;
    case 2:
        octave = 1.0f;
        break;
    case 3:
        octave = 2.0f;
        break;    
    case 4:
        octave = 4.0f;
        break;
    }
    setFrequency(frequency);
}

float OscillatorClass::oscillatorProcess()
{ 
    float sample = linearInterpolate();
    index += indexIncrement;
    index = std::fmod(index, static_cast<float>(WAVETABLE_LENGTH));
    return sample;
}

float OscillatorClass::linearInterpolate()
{
    const auto truncatedIndex = static_cast<int>(index);
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(WAVETABLE_LENGTH);
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    const auto truncatedIndexWeight = 1.0f - nextIndexWeight;

    return truncatedIndexWeight * wavetable[truncatedIndex] + nextIndexWeight * wavetable[nextIndex];
}

void OscillatorClass::stop()
{
    index = 0;
    indexIncrement = 0;
}

bool OscillatorClass::isPlaying()
{
    return indexIncrement != 0;
}
