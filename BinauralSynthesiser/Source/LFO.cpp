/*
  ==============================================================================

    LFO.cpp
    Created: 29 Apr 2022 11:31:23am
    Author:  Matthew Williams

  ==============================================================================
*/

#include "LFO.h"

float LFOClass::process()
{
    float sample = linearInterpolate();
    index += indexIncrement;
    index = std::fmod(index, static_cast<float>(WAVETABLE_LENGTH - 1));
    return sample;
}

void LFOClass::setShape(LFOClass::shape wave)
{
    float output = 0;
    switch (wave)
    {
    case sawUp:
        LFOwave = sawUp;
        output = offset - (range / 2);
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            output += (range / static_cast<float>(WAVETABLE_LENGTH));
            wavetable[i] = output;
        }
        break;
    case sawDown:
        LFOwave = sawDown;
        output = offset + (range / 2);
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            output -= (range / static_cast<float>(WAVETABLE_LENGTH));
            wavetable[i] = output;
        }
        break;

    case triangle:
        LFOwave = triangle;
        output = offset;
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            wavetable[i] = output;
            if (i < WAVETABLE_LENGTH / 4 || i >= (WAVETABLE_LENGTH / 4) * 3)
                output += 2.0f * range / static_cast<float>(WAVETABLE_LENGTH);
            else
                output -= 2.0f * range / static_cast<float>(WAVETABLE_LENGTH);
        }
        break;

    case sine:
        for (int i = 0; i < WAVETABLE_LENGTH; i++)
        {
            wavetable[i] = (range / 2) * (sin((2 * M_PI * static_cast<float>(i)) / static_cast<float>(WAVETABLE_LENGTH))) + offset;
        }
        break;

    }

}

float LFOClass::linearInterpolate()
{
    const auto truncatedIndex = static_cast<int>(index);
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(WAVETABLE_LENGTH);
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    const auto truncatedIndexWeight = 1.0f - nextIndexWeight;
    return truncatedIndexWeight * wavetable[truncatedIndex] + nextIndexWeight * wavetable[nextIndex];
}

void LFOClass::setFrequency(float newFrequency)
{
    indexIncrement = newFrequency * static_cast<float>(WAVETABLE_LENGTH) / sampleRate;
}

void LFOClass::setRange(float newRange)
{
    range = newRange;
    setShape(static_cast<LFOClass::shape>(LFOwave));
}

void LFOClass::setOffset(float newOffset)
{
    offset = newOffset;
    setShape(static_cast<LFOClass::shape>(LFOwave));
}