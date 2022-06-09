/*
  ==============================================================================

    SettingsRectangle.cpp
    Created: 5 May 2022 2:07:54pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include "SettingsRectangle.h"

#include <JuceHeader.h>


//==============================================================================
SettingsRectangle::SettingsRectangle()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SettingsRectangle::~SettingsRectangle()
{
}

void SettingsRectangle::paint(juce::Graphics& g)
{
    juce::Rectangle<float> area{ 170, 170 };
    //juce::Rectangle<float> area2{ 199, 199 };

    g.setColour(juce::Colours::whitesmoke);
    g.fillAll();
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(area);
}

void SettingsRectangle::resized()
{

}

