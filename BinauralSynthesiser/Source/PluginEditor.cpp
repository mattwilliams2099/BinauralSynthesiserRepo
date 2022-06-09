/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BinauralSynthesizerAudioProcessorEditor::BinauralSynthesizerAudioProcessorEditor (BinauralSynthesizerAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{


    for (int i = 0; i < 3; i++)
    {
        setSlider (osc_octaveSlider[i], juce::Colours::whitesmoke, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,  juce::Slider::NoTextBox);
        setSlider (osc_fineSlider[i],   juce::Colours::whitesmoke, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,  juce::Slider::NoTextBox);
        setSlider (osc_shapeSlider[i],  juce::Colours::whitesmoke, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,  juce::Slider::NoTextBox);
        setSlider (osc_mixSlider[i],    juce::Colours::whitesmoke, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,  juce::Slider::NoTextBox);
        setSlider (azimuthSlider[i],    juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearHorizontal);
        setSlider (elevationSlider[i],  juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical);
        setSlider (distanceSlider[i],   juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical);
        setSlider (LFO_freqSlider[i],   juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical,                juce::Slider::NoTextBox);
        setSlider (LFO_amtSlider[i],    juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical,                juce::Slider::NoTextBox);

        addAndMakeVisible (staticAzimuthLFOButton[i]);
        staticAzimuthLFOButton[i].setButtonText ("STATIC LFO RANGE");
        staticAzimuthLFOButton[i].addListener (this);
        staticAzimuthLFOButton[i].setColour (juce::ToggleButton::textColourId,          juce::Colours::whitesmoke);
        staticAzimuthLFOButton[i].setColour (juce::ToggleButton::tickColourId,          juce::Colours::whitesmoke);
        staticAzimuthLFOButton[i].setColour (juce::ToggleButton::tickDisabledColourId , juce::Colours::whitesmoke);

        addAndMakeVisible (directionButton[i]);
        directionButton[i].setButtonText ("CLOCKWISE");
        directionButton[i].addListener (this);
        directionButton[i].setClickingTogglesState (true);
        directionButton[i].setColour (juce::TextButton::buttonOnColourId,           juce::Colours::darkgrey);
        directionButton[i].setColour (juce::TextButton::buttonColourId,             juce::Colours::darkgrey);
        directionButton[i].setColour (juce::TextButton::ColourIds::textColourOffId, juce::Colours::whitesmoke);
        directionButton[i].setColour (juce::TextButton::ColourIds::textColourOnId,  juce::Colours::whitesmoke);

        setLabel(oscLabel[i]);
        setLabel(filterLabel[i]);
    }

    addAndMakeVisible(oneLocationButton);
    oneLocationButton.setButtonText("ONE LOCATION");
    oneLocationButton.addListener (this);
    oneLocationButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    oneLocationButton.setColour(juce::ToggleButton::textColourId, juce::Colours::whitesmoke);
    oneLocationButton.setColour(juce::ToggleButton::tickColourId, juce::Colours::whitesmoke);
    oneLocationButton.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::whitesmoke);

    addAndMakeVisible (settingsButton);
    settingsButton.setButtonText ("SETTINGS");
    settingsButton.addListener (this);
    settingsButton.setClickingTogglesState (true);
    settingsButton.setColour (juce::TextButton::buttonOnColourId,           juce::Colours::darkgrey);
    settingsButton.setColour (juce::TextButton::buttonColourId,             juce::Colours::darkgrey);
    settingsButton.setColour (juce::TextButton::ColourIds::textColourOffId, juce::Colours::whitesmoke);
    settingsButton.setColour (juce::TextButton::ColourIds::textColourOnId,  juce::Colours::whitesmoke);
    settingsButton.setToggleState (false, juce::NotificationType::dontSendNotification);

    for (int i = 0; i < 4; i++)
    {
        setLabel (oscParamLabel[i]);
        setLabel (adsrLabel[i]);
    }

    setLabel (oscTitle, 10);
    setLabel (filterTitle, 10);
    setLabel (envelopeTitle, 10);
    setLabel (azimuthLabel);
    setLabel (elevationLabel);
    setLabel (distanceLabel);
    setLabel (lfoFreqLabel);
    setLabel (lfoAmtLabel);
    setLabel (locationTitleLabel, 10);
    setLabel (lfoTitleLabel, 10);
    setLabel (oscLocSelectLabel);
    setLabel (settingsLabel[0], 11);
    setLabel (settingsLabel[1], 11);

    oscTitle.setText            ("OSCILLATORS",     juce::NotificationType::dontSendNotification);
    filterTitle.setText         ("LOW PASS FILTER", juce::NotificationType::dontSendNotification);
    envelopeTitle.setText       ("ENVELOPE",        juce::NotificationType::dontSendNotification);
    oscLabel[0].setText         ("OSC 1",           juce::NotificationType::dontSendNotification);
    oscLabel[1].setText         ("OSC 2",           juce::NotificationType::dontSendNotification);
    oscLabel[2].setText         ("OSC 3",           juce::NotificationType::dontSendNotification);
    filterLabel[0].setText      ("CUTOFF",          juce::NotificationType::dontSendNotification);
    filterLabel[1].setText      ("RES",             juce::NotificationType::dontSendNotification);
    filterLabel[2].setText      ("EG MOD",          juce::NotificationType::dontSendNotification);
    oscParamLabel[0].setText    ("SHAPE",           juce::NotificationType::dontSendNotification);
    oscParamLabel[1].setText    ("OCTAVE",          juce::NotificationType::dontSendNotification);
    oscParamLabel[2].setText    ("FINE",            juce::NotificationType::dontSendNotification);
    oscParamLabel[3].setText    ("MIX",             juce::NotificationType::dontSendNotification);
    adsrLabel[0].setText        ("ATTACK",          juce::NotificationType::dontSendNotification);
    adsrLabel[1].setText        ("DECAY",           juce::NotificationType::dontSendNotification);
    adsrLabel[2].setText        ("SUSTAIN",         juce::NotificationType::dontSendNotification);
    adsrLabel[3].setText        ("RELEASE",         juce::NotificationType::dontSendNotification);
    azimuthLabel.setText        ("AZIMUTH",         juce::NotificationType::dontSendNotification);
    elevationLabel.setText      ("ELEVATION",       juce::NotificationType::dontSendNotification);
    distanceLabel.setText       ("DISTANCE",        juce::NotificationType::dontSendNotification);
    lfoFreqLabel.setText        ("FREQ",            juce::NotificationType::dontSendNotification);
    lfoAmtLabel.setText         ("AMT",             juce::NotificationType::dontSendNotification);
    locationTitleLabel.setText  ("LOCATION",        juce::NotificationType::dontSendNotification);
    lfoTitleLabel.setText       ("AZIMUTH LFO",     juce::NotificationType::dontSendNotification);
    oscLocSelectLabel.setText   ("OSC SELECT:",     juce::NotificationType::dontSendNotification);
    settingsLabel[0].setText    ("HEAD DIAMETER",   juce::NotificationType::dontSendNotification);
    settingsLabel[1].setText    ("EFFECT INTENSITY",juce::NotificationType::dontSendNotification);

    setSlider (attackSlider,          juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (decaySlider,           juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (sustainSlider,         juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (releaseSlider,         juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (filterCutoffSlider,    juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (filterResonanceSlider, juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (filterEGAmtSlider,     juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearVertical, juce::Slider::NoTextBox);
    setSlider (headWidthSlider,       juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxBelow);
    setSlider (filterBankResSlider,   juce::Colours::whitesmoke, juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextBoxBelow);
    headWidthSlider.setTextValueSuffix("cm");

    addAndMakeVisible(settingsRect);
    addAndMakeVisible(oscMenu);
    oscMenu.addItemList({ "1", "2", "3" }, 1);
    oscMenu.addListener(this);
    oscMenu.setSelectedItemIndex(0, 1);
    oscMenu.setColour(juce::ComboBox::backgroundColourId, juce::Colours::grey);
    oscMenu.setColour(juce::ComboBox::textColourId, juce::Colours::whitesmoke);
    oscMenu.setColour(juce::ComboBox::arrowColourId, juce::Colours::whitesmoke);
    oscMenu.setColour(juce::ComboBox::outlineColourId, juce::Colours::lightgrey);

    osc1_azimuthSliderAttachment = std::make_unique<SliderAttachment>   (valueTreeState, "OSC1_AZ",     azimuthSlider[0]);
    osc1_elevationSliderAttachment = std::make_unique<SliderAttachment> (valueTreeState, "OSC1_EL",     elevationSlider[0]);
    osc1_distanceSliderAttachment = std::make_unique<SliderAttachment>  (valueTreeState, "OSC1_DIST",   distanceSlider[0]);
    osc2_azimuthSliderAttachment = std::make_unique<SliderAttachment>   (valueTreeState, "OSC2_AZ",     azimuthSlider[1]);
    osc2_elevationSliderAttachment = std::make_unique<SliderAttachment> (valueTreeState, "OSC2_EL",     elevationSlider[1]);
    osc2_distanceSliderAttachment = std::make_unique<SliderAttachment>  (valueTreeState, "OSC2_DIST",   distanceSlider[1]);
    osc3_azimuthSliderAttachment = std::make_unique<SliderAttachment>   (valueTreeState, "OSC3_AZ",     azimuthSlider[2]);
    osc3_elevationSliderAttachment = std::make_unique<SliderAttachment> (valueTreeState, "OSC3_EL",     elevationSlider[2]);
    osc3_distanceSliderAttachment = std::make_unique<SliderAttachment>  (valueTreeState, "OSC3_DIST",   distanceSlider[2]);
    osc1_octaveSliderAttachment = std::make_unique<SliderAttachment>    (valueTreeState, "OSC1_OCT",    osc_octaveSlider[0]);
    osc1_fineSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "OSC1_FINE",   osc_fineSlider[0]);
    osc1_shapeSliderAttachment = std::make_unique<SliderAttachment>     (valueTreeState, "OSC1_SHP",    osc_shapeSlider[0]);
    osc1_mixSliderAttachment = std::make_unique<SliderAttachment>       (valueTreeState, "OSC1_MIX",    osc_mixSlider[0]);
    osc2_octaveSliderAttachment = std::make_unique<SliderAttachment>    (valueTreeState, "OSC2_OCT",    osc_octaveSlider[1]);
    osc2_fineSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "OSC2_FINE",   osc_fineSlider[1]);
    osc2_shapeSliderAttachment = std::make_unique<SliderAttachment>     (valueTreeState, "OSC2_SHP",    osc_shapeSlider[1]);
    osc2_mixSliderAttachment = std::make_unique<SliderAttachment>       (valueTreeState, "OSC2_MIX",    osc_mixSlider[1]);
    osc3_octaveSliderAttachment = std::make_unique<SliderAttachment>    (valueTreeState, "OSC3_OCT",    osc_octaveSlider[2]);
    osc3_fineSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "OSC3_FINE",   osc_fineSlider[2]);
    osc3_shapeSliderAttachment = std::make_unique<SliderAttachment>     (valueTreeState, "OSC3_SHP",    osc_shapeSlider[2]);
    osc3_mixSliderAttachment = std::make_unique<SliderAttachment>       (valueTreeState, "OSC3_MIX",    osc_mixSlider[2]);
    attackSliderAttachment = std::make_unique<SliderAttachment>         (valueTreeState, "ATTACK",      attackSlider);
    decaySliderAttachment = std::make_unique<SliderAttachment>          (valueTreeState, "DECAY",       decaySlider);
    sustainSliderAttachment = std::make_unique<SliderAttachment>        (valueTreeState, "SUSTAIN",     sustainSlider);
    releaseSliderAttachment = std::make_unique<SliderAttachment>        (valueTreeState, "RELEASE",     releaseSlider);
    filterCutoffSliderAttachment = std::make_unique<SliderAttachment>   (valueTreeState, "CUTOFF",      filterCutoffSlider);
    filterResonanceSliderAttachment = std::make_unique<SliderAttachment>(valueTreeState, "RESONANCE",   filterResonanceSlider);
    filterEGAmtSliderAttachment = std::make_unique<SliderAttachment>    (valueTreeState, "FILTER_ENV",  filterEGAmtSlider);
    LFO1_freqSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "LFO1_FREQ",   LFO_freqSlider[0]);
    LFO2_freqSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "LFO2_FREQ",   LFO_freqSlider[1]);
    LFO3_freqSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "LFO3_FREQ",   LFO_freqSlider[2]);
    LFO1_amtSliderAttachment = std::make_unique<SliderAttachment>       (valueTreeState, "LFO1_AMT",    LFO_amtSlider[0]);
    LFO2_amtSliderAttachment = std::make_unique<SliderAttachment>       (valueTreeState, "LFO2_AMT",    LFO_amtSlider[1]);
    LFO3_amtSliderAttachment = std::make_unique<SliderAttachment>       (valueTreeState, "LFO3_AMT",    LFO_amtSlider[2]);
    headWidthSliderAttachment = std::make_unique<SliderAttachment>      (valueTreeState, "WIDTH",       headWidthSlider);
    filterBankResSliderAttachment = std::make_unique<SliderAttachment>  (valueTreeState, "BI_RES",      filterBankResSlider);
    osc1_staticAzimuthLFOButtonAttachment = std::make_unique<ButtonAttachment> (valueTreeState, "OSC1_AZ_LOCK", staticAzimuthLFOButton[0]);
    osc2_staticAzimuthLFOButtonAttachment = std::make_unique<ButtonAttachment> (valueTreeState, "OSC2_AZ_LOCK", staticAzimuthLFOButton[1]);
    osc3_staticAzimuthLFOButtonAttachment = std::make_unique<ButtonAttachment> (valueTreeState, "OSC3_AZ_LOCK", staticAzimuthLFOButton[2]);
    osc1_directionButtonAttachment = std::make_unique<ButtonAttachment> (valueTreeState, "OSC1_DIR",    directionButton[0]);
    osc2_directionButtonAttachment = std::make_unique<ButtonAttachment> (valueTreeState, "OSC1_DIR",    directionButton[1]);
    osc3_directionButtonAttachment = std::make_unique<ButtonAttachment> (valueTreeState, "OSC1_DIR",    directionButton[2]);
    oneLocationButtonAttachment = std::make_unique<ButtonAttachment>(valueTreeState, "ONE_LOC", oneLocationButton);
    
    filterCutoffSlider.setRange (20, 20000, 1);
    filterCutoffSlider.setSkewFactor (0.3);
    filterEGAmtSlider.setRange (20, 10000, 1);
    filterEGAmtSlider.setSkewFactor (0.3);
    addAndMakeVisible(circle);
    setSize (505, 500);
    juce::Timer::startTimerHz(24);
}

BinauralSynthesizerAudioProcessorEditor::~BinauralSynthesizerAudioProcessorEditor()
{
    juce::Timer::stopTimer();
}

//==============================================================================
void BinauralSynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.setColour (juce::Colours::black);
    g.fillAll();
    g.setColour (juce::Colours::lightgrey);
    g.drawRect (20,  40,  200, 51);
    g.drawRect (20,  90,  200, 51);
    g.drawRect (20,  140, 200, 50);
    g.drawRect (20,  40,  51,  150);
    g.drawRect (70,  40,  51,  150);
    g.drawRect (120, 40,  51,  150);
    g.drawRect (170, 40,  50,  150);

    g.drawRect (230, 40,  36,  150);
    g.drawRect (265, 40,  36,  150);
    g.drawRect (300, 40,  35,  150);

    g.drawRect (345, 40,  36,  150);
    g.drawRect (380, 40,  36,  150);
    g.drawRect (415, 40,  36,  150);
    g.drawRect (450, 40,  35,  150);

    g.drawRect (20,  230, 150, 35);
    g.drawRect (20,  275, 36,  150);
    g.drawRect (55,  275, 35,  150);
    g.drawRect (100, 285, 36,  140);
    g.drawRect (135, 285, 35,  140);


}

void BinauralSynthesizerAudioProcessorEditor::resized()
{
    const int row_1 = 40;
    const int row_2 = 275;
    const int sl_wid = 35;
    const int sl_hi = 150;
    const int rot_size = 50;
    const int osc_y[3] = { 40, 90, 140 };

    for (int i = 0; i < 3; i++)
    {
        osc_shapeSlider[i].setBounds        (20,        osc_y[i],   rot_size,   rot_size);        
        osc_octaveSlider[i].setBounds       (70,        osc_y[i],   rot_size,   rot_size);
        osc_fineSlider[i].setBounds         (120,       osc_y[i],   rot_size,   rot_size);
        osc_mixSlider[i].setBounds          (170,       osc_y[i],   rot_size,   rot_size);
        azimuthSlider[i].setBounds          (20,        230,        sl_hi,      sl_wid);
        elevationSlider[i].setBounds        (20,        row_2,      sl_wid,     sl_hi);
        distanceSlider[i].setBounds         (55,        row_2,      sl_wid,     sl_hi);
        LFO_freqSlider[i].setBounds         (100,       row_2 + 10, sl_wid,     sl_hi - 10);
        LFO_amtSlider[i].setBounds          (135,       row_2 + 10, sl_wid,     sl_hi - 10);
        staticAzimuthLFOButton[i].setBounds (25,        430,        150,        18);
        directionButton[i].setBounds        (20,        470,        150,        15);

        if (i > 0)
        {
            azimuthSlider[i].setVisible(false);
            elevationSlider[i].setVisible(false);
            distanceSlider[i].setVisible(false);
            LFO_freqSlider[i].setVisible(false);
            LFO_amtSlider[i].setVisible(false);
            staticAzimuthLFOButton[i].setVisible(false);
            directionButton[i].setVisible(false);
        }

        oscLabel[i].setSize(51, 10);
        oscLabel[i].setCentrePosition(0, 0);
        oscLabel[i].setTransform(juce::AffineTransform::rotation(3 * (3.1415926 / 2)));
    }
    circle.setBounds(200, 200, 250, 250);
    oneLocationButton.setBounds(25, 450, 150, 18);

    oscLabel[0].setCentrePosition(15, 66);
    oscLabel[1].setCentrePosition(15, 116);
    oscLabel[2].setCentrePosition(15, 166);

    filterCutoffSlider.setBounds            (230,       row_1,      sl_wid,     sl_hi);
    filterResonanceSlider.setBounds         (265,       row_1,      sl_wid,     sl_hi);
    filterEGAmtSlider.setBounds             (300,       row_1,      sl_wid,     sl_hi);
    attackSlider.setBounds                  (345,       row_1,      sl_wid,     sl_hi);
    decaySlider.setBounds                   (380,       row_1,      sl_wid,     sl_hi);
    sustainSlider.setBounds                 (415,       row_1,      sl_wid,     sl_hi);
    releaseSlider.setBounds                 (450,       row_1,      sl_wid,     sl_hi);
    
    oscMenu.setBounds                       (120,       205,        50,         15);

    oscParamLabel[0].setBounds              (20,        row_1 - 10, 51,         10);
    oscParamLabel[1].setBounds              (70,        row_1 - 10, 51,         10);
    oscParamLabel[2].setBounds              (120,       row_1 - 10, 51,         10);
    oscParamLabel[3].setBounds              (170,       row_1 - 10, 50,         10);
    filterLabel[0].setBounds                (230,       row_1 - 10, 36,         10);
    filterLabel[1].setBounds                (265,       row_1 - 10, 36,         10);
    filterLabel[2].setBounds                (300,       row_1 - 10, 35,         10);
    adsrLabel[0].setBounds                  (345,       row_1 - 10, 36,         10);
    adsrLabel[1].setBounds                  (380,       row_1 - 10, 36,         10);
    adsrLabel[2].setBounds                  (415,       row_1 - 10, 36,         10);
    adsrLabel[3].setBounds                  (450,       row_1 - 10, 35,         10);

    oscTitle.setBounds                      (20,        20,         200,        10);
    filterTitle.setBounds                   (230,       20,         105,        10);
    envelopeTitle.setBounds                 (345,       20,         140,        10);
    lfoTitleLabel.setBounds                 (100,       266,        70,         10);
    locationTitleLabel.setBounds            (20,        195,        150,        10);

    azimuthLabel.setBounds                  (20,        220,        150,        10);
    elevationLabel.setBounds                (20,        265,        36,         10);
    distanceLabel.setBounds                 (55,        265,        35,         10);
    lfoFreqLabel.setBounds                  (100,       275,        36,         10);
    lfoAmtLabel.setBounds                   (135,       275,        35,         10);
    oscLocSelectLabel.setBounds             (20,        205,        100,        15);

    settingsButton.setBounds                (435,       5,          50,         15);
    settingsRect.setBounds                  (317,       2,          170,        170);
    settingsLabel[0].setBounds              (327,       30,         150,        20);
    settingsLabel[1].setBounds              (327,       110,        150,        20);
    headWidthSlider.setBounds               (327,       50,         sl_hi,      sl_wid);
    filterBankResSlider.setBounds           (327,       130,        sl_hi,      sl_wid);    

    settingsLabel[0].setAlwaysOnTop(true);
    settingsLabel[1].setAlwaysOnTop (true);
    headWidthSlider.setAlwaysOnTop (true);
    settingsButton.setAlwaysOnTop (true);
    filterBankResSlider.setAlwaysOnTop (true);   
    

    settingsLabel[0].setVisible (false);
    settingsLabel[1].setVisible (false);
    headWidthSlider.setVisible (false);
    settingsRect.setVisible (false);
    filterBankResSlider.setVisible (false);
}

void BinauralSynthesizerAudioProcessorEditor::setSlider (juce::Slider& slider, juce::Colour colour, juce::Slider::SliderStyle style, juce::Slider::TextEntryBoxPosition textBox)
{
    addAndMakeVisible (slider);
    slider.setSliderStyle (style);
    slider.setTextBoxStyle (textBox, false, 150, 15);
    slider.setColour (juce::Slider::textBoxOutlineColourId,     juce::Colours::lightgrey);
    slider.setColour (juce::Slider::textBoxTextColourId,        juce::Colours::lightgrey);
    slider.setColour (juce::Slider::thumbColourId, colour);
    slider.setColour (juce::Slider::backgroundColourId,         juce::Colours::darkgrey);
    slider.setColour (juce::Slider::trackColourId,              juce::Colours::darkgrey);
    slider.setColour (juce::Slider::rotarySliderFillColourId,   juce::Colours::darkgrey);
    slider.setColour (juce::Slider::rotarySliderOutlineColourId,juce::Colours::darkgrey);

}

void BinauralSynthesizerAudioProcessorEditor::setLabel (juce::Label& label, int fontSize)
{
    addAndMakeVisible (label);
    label.setColour (juce::Label::textColourId,         juce::Colours::whitesmoke);
    label.setColour (juce::Label::backgroundColourId,   juce::Colours::grey);
    label.setColour (juce::Label::outlineColourId,      juce::Colours::whitesmoke);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (fontSize);
}

void BinauralSynthesizerAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    for(int i = 0; i < 3; i++)
    {
        azimuthSlider[i].setVisible (false);
        elevationSlider[i].setVisible (false);
        distanceSlider[i].setVisible (false);
        LFO_freqSlider[i].setVisible (false);
        LFO_amtSlider[i].setVisible(false);
        staticAzimuthLFOButton[i].setVisible (false);
        directionButton[i].setVisible (false);
    }
    
    int itemIndex = oscMenu.getSelectedItemIndex();

    azimuthSlider[itemIndex].setVisible (true);
    elevationSlider[itemIndex].setVisible (true);
    distanceSlider[itemIndex].setVisible (true);
    LFO_freqSlider[itemIndex].setVisible (true);
    staticAzimuthLFOButton[itemIndex].setVisible (true);
    staticAzimuthLFOButton[itemIndex].getToggleState() == true ? LFO_amtSlider[itemIndex].setVisible (true) : LFO_amtSlider[itemIndex].setVisible (false);
    staticAzimuthLFOButton[itemIndex].getToggleState() == true ? azimuthSlider[itemIndex].setVisible (true) : azimuthSlider[itemIndex].setVisible (false);
    staticAzimuthLFOButton[itemIndex].getToggleState() == false ? directionButton[itemIndex].setVisible (true) : directionButton[itemIndex].setVisible (false);
}

void BinauralSynthesizerAudioProcessorEditor::buttonStateChanged(juce::Button* button)
{
}

void BinauralSynthesizerAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    
    for (int i = 0; i < 3; i++)
    {
        if (button == &staticAzimuthLFOButton[i] && staticAzimuthLFOButton[i].getToggleState() == true)
        {
            LFO_amtSlider[i].setVisible (true);
            azimuthSlider[i].setVisible (true);
            directionButton[i].setVisible (false);
            break;
        }

        else if (button == &staticAzimuthLFOButton[i] && staticAzimuthLFOButton[i].getToggleState() == false)
        {
            LFO_amtSlider[i].setValue (359);
            LFO_amtSlider[i].setVisible (false);
            azimuthSlider[i].setVisible (false);
            directionButton[i].setVisible (true);
            break;
        }

        else if (button == &directionButton[i] && directionButton[i].getToggleState() == true)
        {
            directionButton[i].setButtonText ("ANTI-CLOCKWISE");
            break;
        }

        else if (button == &directionButton[i] && directionButton[i].getToggleState() == false)
        {
            directionButton[i].setButtonText ("CLOCKWISE");
            break;
        }
    }

    if (button == &settingsButton && settingsButton.getToggleState() == false)
    {
        settingsLabel[0].setVisible (false);
        settingsLabel[1].setVisible (false);
        headWidthSlider.setVisible (false);
        settingsRect.setVisible (false);
        filterBankResSlider.setVisible (false);
    }

    else if (button == &settingsButton && settingsButton.getToggleState() == true)
    {
        settingsLabel[0].setVisible (true);
        settingsLabel[1].setVisible (true);
        headWidthSlider.setVisible (true);
        settingsRect.setVisible (true);
        filterBankResSlider.setVisible (true);
    }

    else if (button == &oneLocationButton && oneLocationButton.getToggleState() == true)
    {
        elevationSlider[0].setVisible(true);
        distanceSlider[0].setVisible(true);
        LFO_freqSlider[0].setVisible(true);
        staticAzimuthLFOButton[0].setVisible(true);
        if (staticAzimuthLFOButton[0].getToggleState() == false)
        {
            LFO_amtSlider[0].setVisible(false);
            azimuthSlider[0].setVisible(false);
            directionButton[0].setVisible(true);
        }
        else if (staticAzimuthLFOButton[0].getToggleState() == true)
        {
            LFO_amtSlider[0].setVisible(true);
            azimuthSlider[0].setVisible(true);
            directionButton[0].setVisible(false);
        }
        circle.setOneLocation(true);

        oscMenu.setVisible(false);
        for (int i = 1; i < 3; i++)
        {
            azimuthSlider[i].setVisible(false);
            elevationSlider[i].setVisible(false);
            distanceSlider[i].setVisible(false);
            LFO_freqSlider[i].setVisible(false);
            LFO_amtSlider[i].setVisible(false);
            staticAzimuthLFOButton[i].setVisible(false);
            directionButton[i].setVisible(false);
        }
    }

    else if (button == &oneLocationButton && oneLocationButton.getToggleState() == false)
    {
        for (int i = 0; i < 3; i++)
        {
            azimuthSlider[i].setVisible(false);
            elevationSlider[i].setVisible(false);
            distanceSlider[i].setVisible(false);
            LFO_freqSlider[i].setVisible(false);
            LFO_amtSlider[i].setVisible(false);
            staticAzimuthLFOButton[i].setVisible(false);
            directionButton[i].setVisible(false);
        }
        oscMenu.setVisible(true);
        circle.setOneLocation(false);
        int itemIndex = oscMenu.getSelectedItemIndex();

        azimuthSlider[itemIndex].setVisible(true);
        elevationSlider[itemIndex].setVisible(true);
        distanceSlider[itemIndex].setVisible(true);
        LFO_freqSlider[itemIndex].setVisible(true);
        staticAzimuthLFOButton[itemIndex].setVisible(true);
        staticAzimuthLFOButton[itemIndex].getToggleState() == true ? LFO_amtSlider[itemIndex].setVisible(true) : LFO_amtSlider[itemIndex].setVisible(false);
        staticAzimuthLFOButton[itemIndex].getToggleState() == true ? azimuthSlider[itemIndex].setVisible(true) : azimuthSlider[itemIndex].setVisible(false);
        staticAzimuthLFOButton[itemIndex].getToggleState() == false ? directionButton[itemIndex].setVisible(true) : directionButton[itemIndex].setVisible(false);
    }
}

void BinauralSynthesizerAudioProcessorEditor::timerCallback()
{
    for (int i = 0; i < 3; i++)
    {
        circle.setAzimuth(audioProcessor.synthesizer.getAzimuth(i) * (3.14159f / 180.0f), i);
        circle.setDistance(static_cast<float>(distanceSlider[i].getValue()), i);
    }

    circle.repaint();
}