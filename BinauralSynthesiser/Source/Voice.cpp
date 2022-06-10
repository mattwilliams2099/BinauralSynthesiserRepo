/*
  ==============================================================================

    Voice.cpp
    Created: 12 Apr 2022 2:12:07pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include "Voice.h"

VoiceClass::VoiceClass(float samplerate) : sampleRate(samplerate)
{
    //sampleRate = samplerate;

}

float VoiceClass::voiceProcess(int channel)
{
    float envelopeOutput = envelope[channel].ADSRProcess();
    applyFilterEnvelope(envelopeOutput);
    float oscOutput = 0.0f;
    if (oneLocation == false)
    {
        for (int i = 0; i < NUM_OSCILLATORS; i++)
        {
            oscOutput += binaural[i].process(oscillator[channel][i].oscillatorProcess() * oscMix[i] * envelopeOutput, channel);
        }
    }
    else
    {
        for (int i = 0; i < NUM_OSCILLATORS; i++)
        {
            oscOutput += oscillator[channel][i].oscillatorProcess() * oscMix[i] * envelopeOutput;
        }
        oscOutput = binaural[0].process(oscOutput, channel);
    }

    float filterOutput = LPF.filterProcess((oscOutput), channel);
    return filterOutput;
}

void VoiceClass::applyFilterEnvelope(float envOutput)
{
    if ((cutoff + (envOutput * filterEGAmt)) < 20000.0f)
        LPF.setLowPassCo((cutoff + (envOutput * filterEGAmt)), resonance);
    else 
        LPF.setLowPassCo(20000.0f, resonance);
}

void VoiceClass::newNote(int midiNote)
{
    auto frequencyHz = midiNoteToHz(midiNote);
    for (int channel = 0; channel < 2; channel++)
    {
        for (int osc = 0; osc < NUM_OSCILLATORS; osc++)
        {
            oscillator[channel][osc].setFrequency(frequencyHz);
        }
        envelope[channel].keyDown();
    }
}

float VoiceClass::midiNoteToHz(int midiNote)
{
    return 400.0f * std::powf(2.0f, (midiNote - 69.0f) / 12.0f);
}

void VoiceClass::noteRelease()
{
    envelope[0].keyUp();
    envelope[1].keyUp();

    /*
    for (int channel = 0; channel < 2; channel++)
    {
        for (int osc = 0; osc < NUM_OSCILLATORS; osc++)
        {
            oscillator[channel][osc].stop();
        }
    }
    */
}

bool VoiceClass::isPlaying()
{
    //return oscillator[0][0].isPlaying();
    return envelope[0].isActive();
}

void VoiceClass::prepareToPlay()
{

}

void VoiceClass::setSampleRate(float newValue = 44100)
{
    sampleRate = newValue;
    for (int channel = 0; channel < 2; channel++)
    {
        for (int osc = 0; osc < NUM_OSCILLATORS; osc++)
        {
            oscillator[channel][osc].setSampleRate(sampleRate);
            binaural[osc].setSampleRate(sampleRate);
        }
        envelope[channel].setSampleRate(sampleRate);
    }
    //oscillator[0].setSampleRate(sampleRate);
    //oscillator[1].setSampleRate(sampleRate);
    //oscillator[2].setSampleRate(sampleRate);
    LPF.setSampleRate(sampleRate);

}


