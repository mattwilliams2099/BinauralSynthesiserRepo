/*
  ==============================================================================

    Synthesizer.cpp
    Created: 12 Apr 2022 2:11:55pm
    Author:  Matthew Williams

  ==============================================================================
*/

#include "BinauralSynth.h"

BinauralSynthClass::BinauralSynthClass(float samplerate) : sampleRate(samplerate)
{

}

void BinauralSynthClass::prepareToPlay(double samplerate)
{
    voices.setSampleRate(static_cast<float>(samplerate));
    voices.prepareToPlay();
    //}
}


void BinauralSynthClass::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto currentSample = 0;
    for (int i = 0; i < 3; i++)
    {
        int LFOOutput = static_cast<int>(azimuthLFO[i].process());        
        if (LFOOutput > 179)
            setAzimuth(-180 + (LFOOutput % 180), i);
        else if (LFOOutput < -180)
            setAzimuth(179 + (LFOOutput % -181), i);
        else
            setAzimuth(LFOOutput, i);
    }
    for (const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();
        const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

        render(buffer, currentSample, midiEventSample);
        handleMidiEvent(midiEvent);

        currentSample = midiEventSample;
    }
    render(buffer, currentSample, buffer.getNumSamples());

}

void BinauralSynthClass::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    if (midiEvent.isNoteOn())
    {
        voices.newNote(static_cast<int>(midiEvent.getNoteNumber()));
    }
    else if (midiEvent.isNoteOff())
    {
        voices.noteRelease();
    }

}

void BinauralSynthClass::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto* channelBuffer = buffer.getWritePointer(0);

    if (voices.isPlaying())
    {
        for (auto sample = startSample; sample < endSample; ++sample)
        {
            channelBuffer[sample] += voices.voiceProcess(0);
        }
    }
    channelBuffer = buffer.getWritePointer(1);

    if (voices.isPlaying())
    {
        for (auto sample = startSample; sample < endSample; ++sample)
        {
            channelBuffer[sample] += voices.voiceProcess(1);
        }
    }

}
