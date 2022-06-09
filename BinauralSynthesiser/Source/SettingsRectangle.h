/*
  ==============================================================================

    circle.h
    Created: 7 Apr 2022 2:19:04pm
    Author:  Matthew Williams

  ==============================================================================
*/
#define _USE_MATH_DEFINES
#pragma once

#include <JuceHeader.h>
#include <cmath>

//==============================================================================
/*
*/
class SettingsRectangle : public juce::Component
{
public:
    SettingsRectangle();
    ~SettingsRectangle() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setComponantSize(float newValue) { componantSize = newValue; }



private:

    float componantSize = 150;
    float azimuth = 3.14;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsRectangle)
};
