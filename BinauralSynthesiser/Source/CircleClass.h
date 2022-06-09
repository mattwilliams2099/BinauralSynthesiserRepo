/*
  ==============================================================================

    CircleClass.h
    Created: 12 May 2022 3:17:29pm
    Author:  Matthew Williams

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CircleClass  : public juce::Component
{
public:
    CircleClass();
    ~CircleClass() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setCircleSize(float newValue) { circleSize = newValue; }
    void setComponantSize(float newValue) { componantSize = newValue; }
    void setAzimuth(float newValue, int oscNum) { azimuth[oscNum] = newValue; }
    void setDistance(float newValue, int oscNum) { dist[oscNum] = newValue * 10; }
    float getComponantSize() { return componantSize; }
    float getCricleSize() { return circleSize; }
    void setOneLocation(bool isOneLocation) { if (isOneLocation == true)  numLocs = 1; else numLocs = 3; }
private:
    int numLocs = 3;
    float circleSize = 20;
    float componantSize = 150;
    float azimuth[3];
    float dist[3];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircleClass)
};
