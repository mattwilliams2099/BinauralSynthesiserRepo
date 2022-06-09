
#pragma once
#include "JuceHeader.h"
#include "Voice.h"
#include "LFO.h"
#define NUM_VOICES 2
class BinauralSynthClass
{
private:
    LFOClass azimuthLFO[3];
    VoiceClass voices{ sampleRate };
    float sampleRate;
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    int activeNoteID[NUM_VOICES] = { 0, 0 };
    bool clockwise[3];
public:

    BinauralSynthClass(float samplerate);
    BinauralSynthClass() = default;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void prepareToPlay(double samplerate);
    float getAzimuth(int oscNumber)                     {   return voices.getAzimuth(oscNumber);            }
    float getLFORange(int oscNumber)                    {   return azimuthLFO[oscNumber].getRange();        }
    float getLFOOffset(int oscNumber)                   {   return azimuthLFO[oscNumber].getOffset();       }
    void setHeadwidth (float newHeadwidth)              {   voices.setHeadWidth(newHeadwidth);              }
    void setFilterRes (float newFilterResonances)       {   voices.setFilterRes(newFilterResonances);       }
    void setAzimuth(int newValue, int oscNumber)        {   voices.setAzimuth(newValue, oscNumber);         }
    void setLFOOffset(int newValue, int oscNumber)      {   azimuthLFO[oscNumber].setOffset(newValue);      }
    void setElevation(int newValue, int oscNumber)      {   voices.setElevation(newValue, oscNumber);       }
    void setDistance(float newValue, int oscNumber)     {   voices.setDistance(newValue, oscNumber);        }
    void setOscShape(int newValue, int oscNumber)       {   voices.setOscShape(newValue, oscNumber);        }
    void setOscMix(float newOscMixAmt, int oscNumber)   {   voices.setOscMix(newOscMixAmt, oscNumber);      }
    void setOscFineFreq(float newValue, int oscNumber)  {   voices.setOscFine(newValue, oscNumber);         }
    void setOscOctave (float newValue, int oscNumber)   {   voices.setOscOct (newValue, oscNumber);         }
    void setLFOFreq(float newValue, int oscNumber)      {   azimuthLFO[oscNumber].setFrequency(newValue);   }
    void setLFOAmt(float newValue, int oscNumber)       {   azimuthLFO[oscNumber].setRange(newValue);       }
    void setDirection(bool isClockwise, int oscNumber)  {   clockwise[oscNumber] = isClockwise;             }
    void setFilter (float newCutoff, float newResonance){   voices.setFilter (newCutoff, newResonance);     }
    void setAmpAttack (int newValue)                    {   voices.setAmpAttack (newValue);                 }
    void setAmpDecay (int newValue)                     {   voices.setAmpDecay (newValue);                  }
    void setAmpSustain (float newValue)                 {   voices.setAmpSustain (newValue);                }
    void setAmpRelease (int newValue)                   {   voices.setAmpRelease (newValue);                }
    void setFilterEGAmt (float newEnvAmt)               {   voices.setFilterEGAmt(newEnvAmt);               }
    void setOneLocation(bool isOneLocation)             {   voices.setOneLocation(isOneLocation);           }
    void setStaticLFO(bool isStatic, int oscNumber)
    {
        if (isStatic == true)
            azimuthLFO[oscNumber].setShape(LFOClass::sine);
        else if (clockwise[oscNumber] == true)
            azimuthLFO[oscNumber].setShape(LFOClass::sawDown);
        else
            azimuthLFO[oscNumber].setShape(LFOClass::sawUp);
    }
    bool isClockwise(int oscNumber)    {        return clockwise[oscNumber];    }
};