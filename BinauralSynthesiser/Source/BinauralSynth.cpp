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
        //const auto oscillatorId = midiEvent.getNoteNumber();
        //const auto frequency = midiToHz(oscillatorId);
        //for (int i = 0; i < NUM_VOICES; i++)
        //{
            //if (voices.isPlaying() == false)
            //{
        voices.newNote(static_cast<int>(midiEvent.getNoteNumber()));
        //activeNoteID[i] = static_cast<int>(midiEvent.getNoteNumber());
        //break;
    //}
//}
    }
    else if (midiEvent.isNoteOff())
    {
        //const auto oscillatorId = midiEvent.getNoteNumber();
        //oscillators[oscillatorId].stop();
        //for (int i = 0; i < NUM_VOICES; i++)
        //{
            //if (activeNoteID[i] == static_cast<int>(midiEvent.getNoteNumber()))
            //{
        voices.noteRelease();
        //    break;
        //}
   // }
    }
    /*else if (midiEvent.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }

    }
    */
}

void BinauralSynthClass::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto* channelBuffer = buffer.getWritePointer(0);
    //for (VoiceClass voice : voices)

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
    //for (auto channel = 1; channel < buffer.getNumChannels(); channel++)
    //{
    //    std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
    //
}
