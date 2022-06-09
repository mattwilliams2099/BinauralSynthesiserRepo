/*
  ==============================================================================

    CircleClass.cpp
    Created: 12 May 2022 3:17:29pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CircleClass.h"

//==============================================================================
CircleClass::CircleClass()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CircleClass::~CircleClass()
{
}

void CircleClass::paint (juce::Graphics& g)
{

    juce::Path curve;
    juce::Rectangle<float> componantOutline{ 250, 250 };
    juce::Rectangle<float> centreCircle{ 115, 115, 20, 20 };
    juce::Rectangle<float> osc_circle[3];
    float osc_posX[3], osc_posY[3];
    g.setColour(juce::Colours::black);
    g.fillAll();
    g.setColour(juce::Colours::white);
    for (int i = 0; i < numLocs; i++)
    {
        osc_posX[i] = 120 - 5 * sin(azimuth[i]) * dist[i];
        osc_posY[i] = 120 - 5 * cos(azimuth[i]) * dist[i];
        osc_circle[i] = { osc_posX[i], osc_posY[i], 10, 10 };
        g.fillEllipse(osc_circle[i]);
        g.drawLine(125, 125, osc_posX[i] + 5, osc_posY[i] + 5);
    }
    g.drawEllipse(centreCircle, 1.0f);
}

void CircleClass::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
