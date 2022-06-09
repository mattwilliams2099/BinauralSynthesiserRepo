/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BinauralSynthesizerAudioProcessor::BinauralSynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, juce::Identifier("BinauralValueTree"),
                           {
                                std::make_unique<juce::AudioParameterInt>   ("OSC1_AZ",     "Osc 1 Azimuth",    -180,   179,    0),
                                std::make_unique<juce::AudioParameterInt>   ("OSC1_EL",     "Osc 1 Elevation",  0,      31,     15),
                                std::make_unique<juce::AudioParameterFloat> ("OSC1_DIST",   "Osc 1 Distance",   0.125f, 2.0f,   1.0f),                                
                                std::make_unique<juce::AudioParameterInt>   ("OSC2_AZ",     "Osc 2 Azimuth",    -180,   179,    0),
                                std::make_unique<juce::AudioParameterInt>   ("OSC2_EL",     "Osc 2 Elevation",  0,      31,     15),
                                std::make_unique<juce::AudioParameterFloat> ("OSC2_DIST",   "Osc 2 Distance",   0.125f, 2.0f,   1.0f),                                
                                std::make_unique<juce::AudioParameterInt>   ("OSC3_AZ",     "Osc 3 Azimuth",    -180,   179,    0),
                                std::make_unique<juce::AudioParameterInt>   ("OSC3_EL",     "Osc 3 Elevation",  0,      31,     15),
                                std::make_unique<juce::AudioParameterFloat> ("OSC3_DIST",   "Osc 3 Distance",   0.125f, 2.0f,   1.0f),
                                std::make_unique<juce::AudioParameterInt>   ("OSC1_OCT",    "Osc 1 Octave",     0,      4,      2),
                                std::make_unique<juce::AudioParameterFloat> ("OSC1_FINE",   "Osc 1 Fine Tune",  9.7f,   10.3f,  10.0f),
                                std::make_unique<juce::AudioParameterInt>   ("OSC1_SHP",    "Osc 1 Shape",      0,      3,      0),
                                std::make_unique<juce::AudioParameterFloat> ("OSC1_MIX",    "Osc 1 Mix",        0.0f,   0.5f,   0.25f),
                                std::make_unique<juce::AudioParameterInt>   ("OSC2_OCT",    "Osc 2 Octave",     0,      4,      2),
                                std::make_unique<juce::AudioParameterFloat> ("OSC2_FINE",   "Osc 2 Fine Tune",  9.7f,   10.3f,  10.0f),
                                std::make_unique<juce::AudioParameterInt>   ("OSC2_SHP",    "Osc 2 Shape",      0,      3,      0),
                                std::make_unique<juce::AudioParameterFloat> ("OSC2_MIX",    "Osc 2 Mix",        0.0f,   0.5f,   0.25f),
                                std::make_unique<juce::AudioParameterInt>   ("OSC3_OCT",    "Osc 3 Octave",     0,      4,      2),
                                std::make_unique<juce::AudioParameterFloat> ("OSC3_FINE",   "Osc 3 Fine Tune",  9.7f,   10.3f,  10.0f),
                                std::make_unique<juce::AudioParameterInt>   ("OSC3_SHP",    "Osc 3 Shape",      0,      3,      0),
                                std::make_unique<juce::AudioParameterFloat> ("OSC3_MIX",    "Osc 3 Mix",        0.0f,   0.5f,   0.25f),
                                std::make_unique<juce::AudioParameterInt>   ("ATTACK",      "Attack",           1,      1000,   500),
                                std::make_unique<juce::AudioParameterInt>   ("DECAY",       "Decay",            1,      1000,   500),
                                std::make_unique<juce::AudioParameterFloat> ("SUSTAIN",     "Sustain",          0.0f,   1.0f,   0.5f),
                                std::make_unique<juce::AudioParameterInt>   ("RELEASE",     "Release",          1,      1000,   500),
                                std::make_unique<juce::AudioParameterFloat> ("CUTOFF",      "Cutoff",           20.0f,  20000.0f,20000.0f),
                                std::make_unique<juce::AudioParameterFloat> ("RESONANCE",   "Resonance",        0.1,    20.0f,  0.5f),
                                std::make_unique<juce::AudioParameterFloat> ("FILTER_ENV",  "Filter EG Amt",    0.0f,   10000.0f,0.0f),
                                std::make_unique<juce::AudioParameterFloat> ("LFO1_FREQ",   "LFO 1 Freq",       0.01f,  400.0f, 1.0f),
                                std::make_unique<juce::AudioParameterFloat> ("LFO2_FREQ",   "LFO 2 Freq",       0.01f,  400.0f, 1.0f),
                                std::make_unique<juce::AudioParameterFloat> ("LFO3_FREQ",   "LFO 3 Freq",       0.01f,  400.0f, 1.0f),
                                std::make_unique<juce::AudioParameterFloat>("LFO1_AMT",   "LFO 1 Amt",       0.01f,  359.0f, 180.0f),
                                std::make_unique<juce::AudioParameterFloat>("LFO2_AMT",   "LFO 2 Amt",       0.01f,  359.0f, 180.0f),
                                std::make_unique<juce::AudioParameterFloat>("LFO3_AMT",   "LFO 3 Amt",       0.01f,  359.0f, 180.0f),
                                std::make_unique<juce::AudioParameterFloat>("WIDTH",   "Ear Spacing",       12.0f,  50.0f, 15.0f),
                                std::make_unique<juce::AudioParameterFloat>("BI_RES",   "Filter Bank Resonance Scaling",       0.5f,  5.0f, 1.0f),
                                std::make_unique<juce::AudioParameterBool>  ("OSC1_AZ_LOCK","Osc 1 Azimuth Lock", true),
                                std::make_unique<juce::AudioParameterBool>  ("OSC2_AZ_LOCK","Osc 2 Azimuth Lock", true),
                                std::make_unique<juce::AudioParameterBool>  ("OSC3_AZ_LOCK","Osc 3 Azimuth Lock", true),
                                std::make_unique<juce::AudioParameterBool>("OSC1_DIR", "Osc 1 Direction", true),
                                std::make_unique<juce::AudioParameterBool>("OSC2_DIR", "Osc 2 Direction", true),
                                std::make_unique<juce::AudioParameterBool>("OSC3_DIR", "Osc 3 Direction", true),
                                std::make_unique<juce::AudioParameterBool>("ONE_LOC", "One Location", false),

                           })
#endif
{
    osc1_azimuthParameter = parameters.getRawParameterValue("OSC1_AZ");
    osc1_elevationParameter = parameters.getRawParameterValue("OSC1_EL");
    osc1_distanceParameter = parameters.getRawParameterValue("OSC1_DIST");

    osc2_azimuthParameter = parameters.getRawParameterValue("OSC2_AZ");
    osc2_elevationParameter = parameters.getRawParameterValue("OSC2_EL");
    osc2_distanceParameter = parameters.getRawParameterValue("OSC2_DIST");

    osc3_azimuthParameter = parameters.getRawParameterValue("OSC3_AZ");
    osc3_elevationParameter = parameters.getRawParameterValue("OSC3_EL");
    osc3_distanceParameter = parameters.getRawParameterValue("OSC3_DIST");

    osc1_octaveParameter = parameters.getRawParameterValue("OSC1_OCT");
    osc1_fineParameter = parameters.getRawParameterValue("OSC1_FINE");
    osc1_shapeParameter = parameters.getRawParameterValue("OSC1_SHP");
    osc1_mixParameter = parameters.getRawParameterValue("OSC1_MIX");

    osc2_octaveParameter = parameters.getRawParameterValue("OSC2_OCT");
    osc2_fineParameter = parameters.getRawParameterValue("OSC2_FINE");
    osc2_shapeParameter = parameters.getRawParameterValue("OSC2_SHP");
    osc2_mixParameter = parameters.getRawParameterValue("OSC2_MIX");

    osc3_octaveParameter = parameters.getRawParameterValue("OSC3_OCT");
    osc3_fineParameter = parameters.getRawParameterValue("OSC3_FINE");
    osc3_shapeParameter = parameters.getRawParameterValue("OSC3_SHP");
    osc3_mixParameter = parameters.getRawParameterValue("OSC3_MIX");

    attackParameter = parameters.getRawParameterValue("ATTACK");
    decayParameter = parameters.getRawParameterValue("DECAY");
    sustainParameter = parameters.getRawParameterValue("SUSTAIN");
    releaseParameter = parameters.getRawParameterValue("RELEASE");

    filterCutoffParameter = parameters.getRawParameterValue("CUTOFF");
    filterResonanceParameter = parameters.getRawParameterValue("RESONANCE");
    filterEGAmtParameter = parameters.getRawParameterValue("FILTER_ENV");

    LFO1_freqParameter = parameters.getRawParameterValue("LFO1_FREQ");
    LFO2_freqParameter = parameters.getRawParameterValue("LFO2_FREQ");
    LFO3_freqParameter = parameters.getRawParameterValue("LFO3_FREQ");

    LFO1_amtParameter = parameters.getRawParameterValue("LFO1_AMT");
    LFO2_amtParameter = parameters.getRawParameterValue("LFO2_AMT");
    LFO3_amtParameter = parameters.getRawParameterValue("LFO3_AMT");

    osc1_azimuthLockParameter = parameters.getRawParameterValue("OSC1_AZ_LOCK");
    osc2_azimuthLockParameter = parameters.getRawParameterValue("OSC2_AZ_LOCK");
    osc3_azimuthLockParameter = parameters.getRawParameterValue("OSC3_AZ_LOCK");
     
    osc1_directionParameter = parameters.getRawParameterValue("OSC1_DIR");
    osc2_directionParameter = parameters.getRawParameterValue("OSC2_DIR");
    osc3_directionParameter = parameters.getRawParameterValue("OSC3_DIR");

    headWidthParameter = parameters.getRawParameterValue("WIDTH");
    filterBankResParameter = parameters.getRawParameterValue("BI_RES");

    oneLocationParameter = parameters.getRawParameterValue("ONE_LOC");



}

BinauralSynthesizerAudioProcessor::~BinauralSynthesizerAudioProcessor()
{

}

//==============================================================================
const juce::String BinauralSynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BinauralSynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BinauralSynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BinauralSynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BinauralSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BinauralSynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BinauralSynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BinauralSynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BinauralSynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void BinauralSynthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BinauralSynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synthesizer.prepareToPlay(44100);

    

    synthesizer.setLFOOffset    (*osc1_azimuthParameter,    0);
    synthesizer.setElevation    (*osc1_elevationParameter,  0);
    synthesizer.setDistance     (*osc1_distanceParameter,   0);    
    synthesizer.setLFOOffset    (*osc2_azimuthParameter,    1);
    synthesizer.setElevation    (*osc2_elevationParameter,  1);
    synthesizer.setDistance     (*osc2_distanceParameter,   1);    
    synthesizer.setLFOOffset    (*osc3_azimuthParameter,    2);
    synthesizer.setElevation    (*osc3_elevationParameter,  2);
    synthesizer.setDistance     (*osc3_distanceParameter,   2);
    synthesizer.setOscShape     (*osc1_shapeParameter,      0);
    synthesizer.setOscOctave    (*osc1_octaveParameter,     0);
    synthesizer.setOscFineFreq  (*osc1_fineParameter / 10,  0);
    synthesizer.setOscMix       (*osc1_mixParameter,        0);
    synthesizer.setOscShape     (*osc2_shapeParameter,      1);
    synthesizer.setOscOctave    (*osc2_octaveParameter,     1);
    synthesizer.setOscFineFreq  (*osc2_fineParameter / 10,  1);
    synthesizer.setOscMix       (*osc2_mixParameter,        1);
    synthesizer.setOscShape     (*osc3_shapeParameter,      2);
    synthesizer.setOscOctave    (*osc3_octaveParameter,     2);
    synthesizer.setOscFineFreq  (*osc3_fineParameter / 10,  2);
    synthesizer.setOscMix       (*osc3_mixParameter,        2);
    synthesizer.setAmpAttack    (*attackParameter);
    synthesizer.setAmpDecay     (*decayParameter);
    synthesizer.setAmpSustain   (*sustainParameter);
    synthesizer.setAmpRelease   (*releaseParameter);
    synthesizer.setFilter       (*filterCutoffParameter, *filterResonanceParameter);
    synthesizer.setFilterEGAmt  (*filterEGAmtParameter);
    synthesizer.setLFOFreq      (*LFO1_freqParameter,       0);
    synthesizer.setLFOFreq      (*LFO2_freqParameter,       1);
    synthesizer.setLFOFreq      (*LFO3_freqParameter,       2);
    synthesizer.setLFOAmt       (*LFO1_amtParameter,        0);
    synthesizer.setLFOAmt       (*LFO2_amtParameter,        1);
    synthesizer.setLFOAmt       (*LFO3_amtParameter,        2);
    synthesizer.setDirection    (*osc1_directionParameter,  0);
    synthesizer.setDirection    (*osc2_directionParameter,  1);
    synthesizer.setDirection    (*osc3_directionParameter,  2);
    synthesizer.setStaticLFO    (*osc1_azimuthLockParameter, 0);
    synthesizer.setStaticLFO    (*osc2_azimuthLockParameter, 1);
    synthesizer.setStaticLFO    (*osc3_azimuthLockParameter, 2);
    synthesizer.setHeadwidth    (*headWidthParameter / 200);
    synthesizer.setFilterRes    (*filterBankResParameter);
    synthesizer.setOneLocation  (*oneLocationParameter);

}

void BinauralSynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BinauralSynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BinauralSynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    synthesizer.setLFOOffset    (*osc1_azimuthParameter,    0);
    synthesizer.setElevation    (*osc1_elevationParameter,  0);
    synthesizer.setDistance     (*osc1_distanceParameter,   0);    
    synthesizer.setLFOOffset    (*osc2_azimuthParameter,    1);
    synthesizer.setElevation    (*osc2_elevationParameter,  1);
    synthesizer.setDistance     (*osc2_distanceParameter,   1);    
    synthesizer.setLFOOffset    (*osc3_azimuthParameter,    2);
    synthesizer.setElevation    (*osc3_elevationParameter,  2);
    synthesizer.setDistance     (*osc3_distanceParameter,   2);
    synthesizer.setOscShape     (*osc1_shapeParameter,      0);
    synthesizer.setOscOctave    (*osc1_octaveParameter,     0);
    synthesizer.setOscFineFreq  (*osc1_fineParameter / 10,  0);
    synthesizer.setOscMix       (*osc1_mixParameter,        0);
    synthesizer.setOscShape     (*osc2_shapeParameter,      1);
    synthesizer.setOscOctave    (*osc2_octaveParameter,     1);
    synthesizer.setOscFineFreq  (*osc2_fineParameter / 10,  1);
    synthesizer.setOscMix       (*osc2_mixParameter,        1);
    synthesizer.setOscShape     (*osc3_shapeParameter,      2);
    synthesizer.setOscOctave    (*osc3_octaveParameter,     2);
    synthesizer.setOscFineFreq  (*osc3_fineParameter / 10,  2);
    synthesizer.setOscMix       (*osc3_mixParameter,        2);
    synthesizer.setAmpAttack    (*attackParameter);
    synthesizer.setAmpDecay     (*decayParameter);
    synthesizer.setAmpSustain   (*sustainParameter);
    synthesizer.setAmpRelease   (*releaseParameter);
    synthesizer.setFilter       (*filterCutoffParameter, *filterResonanceParameter);
    synthesizer.setFilterEGAmt  (*filterEGAmtParameter);
    synthesizer.setLFOFreq      (*LFO1_freqParameter,       0);
    synthesizer.setLFOFreq      (*LFO2_freqParameter,       1);
    synthesizer.setLFOFreq      (*LFO3_freqParameter,       2);
    synthesizer.setLFOAmt       (*LFO1_amtParameter,        0);
    synthesizer.setLFOAmt       (*LFO2_amtParameter,        1);
    synthesizer.setLFOAmt       (*LFO3_amtParameter,        2);
    synthesizer.setDirection    (*osc1_directionParameter,  0);
    synthesizer.setDirection    (*osc2_directionParameter,  1);
    synthesizer.setDirection    (*osc3_directionParameter,  2);
    synthesizer.setStaticLFO    (*osc1_azimuthLockParameter, 0);
    synthesizer.setStaticLFO    (*osc2_azimuthLockParameter, 1);
    synthesizer.setStaticLFO    (*osc3_azimuthLockParameter, 2);
    synthesizer.setHeadwidth    (*headWidthParameter / 200);
    synthesizer.setFilterRes    (*filterBankResParameter);
    synthesizer.setOneLocation  (*oneLocationParameter);


    synthesizer.processBlock(buffer, midiMessages);
}

//==============================================================================
bool BinauralSynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BinauralSynthesizerAudioProcessor::createEditor()
{
    return new BinauralSynthesizerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void BinauralSynthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BinauralSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BinauralSynthesizerAudioProcessor();
}


