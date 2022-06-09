/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES

#include <JuceHeader.h>
#include <cmath>
#include "PluginProcessor.h"
#include "SettingsRectangle.h"
#include "CircleClass.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class BinauralSynthesizerAudioProcessorEditor  :    public juce::AudioProcessorEditor,
                                                    public juce::ComboBox::Listener,
                                                    public juce::Button::Listener,
    public juce::Timer
{
public:
    BinauralSynthesizerAudioProcessorEditor (BinauralSynthesizerAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~BinauralSynthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBox) override;
    void buttonStateChanged (juce::Button* button) override;
    void buttonClicked(juce::Button* button) override;
    void timerCallback();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CircleClass circle;
    BinauralSynthesizerAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider elevationSlider[3];
    juce::Slider azimuthSlider[3];
    juce::Slider distanceSlider[3];
    juce::Slider osc_octaveSlider[3];
    juce::Slider osc_fineSlider[3];
    juce::Slider osc_shapeSlider[3];
    juce::Slider osc_mixSlider[3];
    juce::Slider LFO_freqSlider[3];
    juce::Slider LFO_amtSlider[3];
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    juce::Slider filterEGAmtSlider;
    juce::Slider headWidthSlider;
    juce::Slider filterBankResSlider;

    juce::ToggleButton staticAzimuthLFOButton[3];
    juce::ToggleButton oneLocationButton;
    juce::TextButton directionButton[3];
    juce::TextButton settingsButton;
    juce::ComboBox oscMenu;

    juce::Label oscTitle;
    juce::Label filterTitle;
    juce::Label envelopeTitle;
    juce::Label oscLabel[3];
    juce::Label oscParamLabel[4];
    juce::Label adsrLabel[4];
    juce::Label filterLabel[3];
    juce::Label azimuthLabel;
    juce::Label elevationLabel;
    juce::Label distanceLabel;
    juce::Label locationTitleLabel;
    juce::Label lfoFreqLabel;
    juce::Label lfoAmtLabel;
    juce::Label lfoTitleLabel;
    juce::Label oscLocSelectLabel;
    juce::Label settingsLabel[2];

    SettingsRectangle settingsRect;

    std::unique_ptr<ButtonAttachment> osc1_staticAzimuthLFOButtonAttachment;
    std::unique_ptr<ButtonAttachment> osc2_staticAzimuthLFOButtonAttachment;
    std::unique_ptr<ButtonAttachment> osc3_staticAzimuthLFOButtonAttachment;
    std::unique_ptr<ButtonAttachment> osc1_directionButtonAttachment;
    std::unique_ptr<ButtonAttachment> osc2_directionButtonAttachment;
    std::unique_ptr<ButtonAttachment> osc3_directionButtonAttachment;
    std::unique_ptr<ButtonAttachment> oneLocationButtonAttachment;

    std::unique_ptr<SliderAttachment> osc1_elevationSliderAttachment;
    std::unique_ptr<SliderAttachment> osc1_azimuthSliderAttachment;
    std::unique_ptr<SliderAttachment> osc1_distanceSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_elevationSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_azimuthSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_distanceSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_elevationSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_azimuthSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_distanceSliderAttachment;
    std::unique_ptr<SliderAttachment> osc1_octaveSliderAttachment;
    std::unique_ptr<SliderAttachment> osc1_fineSliderAttachment;
    std::unique_ptr<SliderAttachment> osc1_shapeSliderAttachment;
    std::unique_ptr<SliderAttachment> osc1_mixSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_octaveSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_fineSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_shapeSliderAttachment;
    std::unique_ptr<SliderAttachment> osc2_mixSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_octaveSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_fineSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_shapeSliderAttachment;
    std::unique_ptr<SliderAttachment> osc3_mixSliderAttachment;
    std::unique_ptr<SliderAttachment> attackSliderAttachment;
    std::unique_ptr<SliderAttachment> decaySliderAttachment;
    std::unique_ptr<SliderAttachment> sustainSliderAttachment;
    std::unique_ptr<SliderAttachment> releaseSliderAttachment;
    std::unique_ptr<SliderAttachment> filterCutoffSliderAttachment;
    std::unique_ptr<SliderAttachment> filterResonanceSliderAttachment;
    std::unique_ptr<SliderAttachment> filterEGAmtSliderAttachment;
    std::unique_ptr<SliderAttachment> LFO1_freqSliderAttachment;
    std::unique_ptr<SliderAttachment> LFO2_freqSliderAttachment;
    std::unique_ptr<SliderAttachment> LFO3_freqSliderAttachment;
    std::unique_ptr<SliderAttachment> LFO1_amtSliderAttachment;
    std::unique_ptr<SliderAttachment> LFO2_amtSliderAttachment;
    std::unique_ptr<SliderAttachment> LFO3_amtSliderAttachment;
    std::unique_ptr<SliderAttachment> headWidthSliderAttachment;
    std::unique_ptr<SliderAttachment> filterBankResSliderAttachment;

    void setSlider(juce::Slider& slider, juce::Colour colour, juce::Slider::SliderStyle style, juce::Slider::TextEntryBoxPosition textBox = juce::Slider::TextBoxBelow);
    void setLabel(juce::Label& label, int fontSize = 8);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralSynthesizerAudioProcessorEditor)
};
