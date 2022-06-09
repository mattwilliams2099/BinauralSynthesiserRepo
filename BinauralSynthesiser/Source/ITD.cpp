/*
  ==============================================================================

    ITD.cpp
    Created: 21 Apr 2022 3:50:00pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include "ITD.h"

ITDClass::ITDClass(float samplerate) : sampleRate(samplerate)
{
    samplePeriod = 1.0f / sampleRate;
    loadCoordinateDatabase();
}

float ITDClass::process(float sample, int channel)
{
    return delayLine.delayProcess(sample * distanceCoefficient[channel], channel);// *pow((1 / distance), 2); //distanceCoefficient[channel];
}

void ITDClass::setDelay()
{
    float distance_long = sqrt(pow((distance * coordinatesDatabase[index][0]), 2) + pow ((distance * coordinatesDatabase[index][1]) + headWidth, 2) + pow ((distance * coordinatesDatabase[index][2]), 2));
    float distance_short = sqrt(pow((distance * coordinatesDatabase[index][0]), 2) + pow((distance * coordinatesDatabase[index][1]) - headWidth, 2) + pow((distance * coordinatesDatabase[index][2]), 2));
    
    if (index < 576)
    {
        delayLine.setDelayLength((distance / 343.0f) / samplePeriod, 0);
        delayLine.setDelayLength(((((headWidth) * (azimuth + sin(azimuth))) + distance) / 343.0f) / samplePeriod, 1);
        distanceCoefficient[0] = pow((1 / distance_short), 2);
        distanceCoefficient[1] = pow((1 / distance_long), 2);
    }
    else if (index < 1152)
    {
        delayLine.setDelayLength((distance / 343.0f) / samplePeriod, 0);
        delayLine.setDelayLength(((((headWidth) * (M_PI - azimuth + sin(azimuth))) + distance) / 343.0f) / samplePeriod, 1);
        distanceCoefficient[0] = pow((1 / distance_short), 2);
        distanceCoefficient[1] = pow((1 / distance_long), 2);
    }
    else if (index < 1728)
    {
        delayLine.setDelayLength(((((headWidth) * ((azimuth - M_PI) + sin(azimuth - M_PI))) + distance) / 343.0f) / samplePeriod, 0);
        delayLine.setDelayLength((distance / 343.0f) / samplePeriod, 1);
        distanceCoefficient[0] = pow((1 / distance_long), 2);
        distanceCoefficient[1] = pow((1 / distance_short), 2);
    }
    else
    {
        delayLine.setDelayLength(((((headWidth) * (M_PI - (azimuth - M_PI) + sin(azimuth - M_PI))) + distance) / 343.0f) / samplePeriod, 0);
        delayLine.setDelayLength((distance / 343.0f) / samplePeriod, 1);
        distanceCoefficient[0] = pow((1 / distance_long), 2);
        distanceCoefficient[1] = pow((1 / distance_short), 2);
    }
}

void ITDClass::loadCoordinateDatabase()
{
    std::ifstream coordinatesFile[3];

    std::string loc[3];
    loc[0] = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("Binaural_Synthesizer_Resources\\x_coordinates.txt").getFullPathName().toStdString();
    loc[1] = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("Binaural_Synthesizer_Resources\\y_coordinates.txt").getFullPathName().toStdString();
    loc[2] = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("Binaural_Synthesizer_Resources\\z_coordinates.txt").getFullPathName().toStdString();
    for (int i = 0; i < 3; i++)
    {
        coordinatesFile[i].open(loc[i]);
        int n = 0;
        if (coordinatesFile[i].is_open())
        {
            while (coordinatesFile[i] >> coordinatesDatabase[n][i])
            {
                n++;
            }
            coordinatesFile[i].close();
        }
    }
   
}

void ITDClass::setIndex(int newIndex)
{
    index = newIndex;
    setDelay();
}

void ITDClass::setDistance(float newDistance)
{
    distance = newDistance;
    distanceCo = pow((1 / distance), 2);
    setDelay();
}