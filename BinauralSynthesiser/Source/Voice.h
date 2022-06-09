/*
  ==============================================================================

    Voice.h
    Created: 12 Apr 2022 2:12:07pm
    Author:  Matthew Williams

  ==============================================================================
*/

#pragma once
#include "Oscillator.h"
#include "ADSR.h"
#include "Filter.h"
#include "Binaural.h"

#define NUM_OSCILLATORS 3
class VoiceClass
{

public:

    VoiceClass(float samplerate);
    VoiceClass() = default;
    float voiceProcess(int channel);
    void newNote(int midiNote);
    void noteRelease();
    bool isPlaying();
    void setSampleRate(float newValue);
    void prepareToPlay();
    float getAzimuth(int oscNumber)
    {
        return binaural[oscNumber].getAzimuth();
    }
    void setHeadWidth(float newHeadwidth)
    {
        for (int i = 0; i < NUM_OSCILLATORS; i++)
        {
            binaural[i].setHeadWidth(newHeadwidth);
        }
    }

    void setFilterRes(float newFilterResonances)
    {
        for (int i = 0; i < NUM_OSCILLATORS; i++)
        {
            binaural[i].setFilterRes(newFilterResonances);
        }
    }

    void setAzimuth(int newValue, int oscNumber)
    {
        binaural[oscNumber].setAzimuth(newValue);
    }

    void setElevation(int newValue, int oscNumber)
    {
        binaural[oscNumber].setElevation(newValue);
    }

    void setDistance(float newValue, int oscNumber)
    {
        binaural[oscNumber].setDistance(newValue);
    }

    void setOscShape(int newValue, int oscNumber) 
    { 
        oscillator[0][oscNumber].setWaveShape(static_cast<OscillatorClass::shape>(newValue)); 
        oscillator[1][oscNumber].setWaveShape(static_cast<OscillatorClass::shape>(newValue));
    }
    
    void setOscFine(float newValue, int oscNumber) 
    { 
        oscillator[0][oscNumber].setFineFreq(newValue); 
        oscillator[1][oscNumber].setFineFreq(newValue);
    }

    void setOscOct(int newValue, int oscNumber)
    { 
        oscillator[0][oscNumber].setOctave(newValue);
        oscillator[1][oscNumber].setOctave(newValue);
    }

    void setFilter(float newCutoff, float newResonance) 
    { 
        cutoff = newCutoff;
        resonance = newResonance;
        LPF.setLowPassCo(newCutoff, newResonance); 
    }

    void setAmpAttack(int newValue) 
    { 
        envelope[0].setAttack(newValue); 
        envelope[1].setAttack(newValue);
    }

    void setAmpDecay(int newValue) 
    { 
        envelope[0].setDecay(newValue); 
        envelope[1].setDecay(newValue);
    }

    void setAmpSustain(float newValue) 
    {   
        envelope[0].setSustain(newValue); 
        envelope[1].setSustain(newValue); 
    }
    
    void setAmpRelease(int newValue) 
    { 
        envelope[0].setRelease(newValue); 
        envelope[1].setRelease(newValue);
    }

    void setOscMix(float newOscMixAmt, int osc)
    {
        oscMix[osc] = newOscMixAmt;
    }

    void setFilterEGAmt(float newEnvAmt)
    {
        filterEGAmt = newEnvAmt;
    }

    void setOneLocation(bool isOneLocation)
    {
        oneLocation = isOneLocation;
    }

private:
    OscillatorClass oscillator[2][NUM_OSCILLATORS]{ sampleRate };
    ADSRClass envelope[2]{ sampleRate };
    FilterClass LPF{ sampleRate };
    BinauralClass binaural[NUM_OSCILLATORS]{ sampleRate };
    float midiNoteToHz(int midiNote);
    float sampleRate;
    float oscMix[NUM_OSCILLATORS];
    float cutoff;
    float resonance;
    float filterEGAmt;
    void applyFilterEnvelope(float envOutput);
    bool oneLocation = false;
};
