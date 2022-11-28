/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProjectFourSynthAudioProcessorEditor::ProjectFourSynthAudioProcessorEditor (ProjectFourSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // Setting the label of the app
    setSize (1200, 700);
    appTitle.setText("Project 4 Synth", juce::dontSendNotification);
    appTitle.setFont(44.f);
    appTitle.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(appTitle);
    // Setting the labels for the oscillators
    oscillatorLabels[0].setText("Oscillator 1", juce::dontSendNotification);
    oscillatorLabels[1].setText("Oscillator 2", juce::dontSendNotification);
    oscillatorLabels[2].setText("Oscillator 3", juce::dontSendNotification);
    oscillatorLabels[3].setText("Oscillator 4", juce::dontSendNotification);
    
    for (int i = 0; i < 4 ; i++)
    {
        oscillatorLabels[i].setFont(16.f);
        //oscillatorLabels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(oscillatorLabels[i]);
    }
    // Setting the items in the wavechoices
    for (int i = 0; i < 4; i++)
    {
        waveChoices[i].addItem("Sine", 1);
        waveChoices[i].addItem("Sawtooth", 2);
        waveChoices[i].addItem("Triangle", 3);
        waveChoices[i].addItem("Square", 4);
        waveChoices[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(waveChoices[i]);
    }
    // Setting the sliders for the gain sliders
    for (int i = 0; i < 4; i++)
    {
        oscillatorGains[i].setSliderStyle(juce::Slider::LinearHorizontal);
        oscillatorLabels[i].setJustificationType(juce::Justification::centred);
        oscillatorGains[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 20);
        oscillatorGainLabels[i].attachToComponent(&oscillatorGains[i], false);
        oscillatorGainLabels[i].setText("Volume", juce::dontSendNotification);
        oscillatorGainLabels[i].setFont(14.f);
        oscillatorGainLabels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(oscillatorGains[i]);
    }
    // Setting the sliders for the pitch shift
    for (int i = 0; i < 4; i++)
    {
        oscillatorPitchShiftSliders[i].setSliderStyle(juce::Slider::RotaryVerticalDrag);
        oscillatorPitchShiftSliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 20);
        oscillatorPitchShiftLabels[i].attachToComponent(&oscillatorPitchShiftSliders[i], false);
        oscillatorPitchShiftLabels[i].setText("Pitch Shift", juce::dontSendNotification);
        oscillatorPitchShiftLabels[i].setFont(14.f);
        oscillatorPitchShiftLabels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(oscillatorPitchShiftSliders[i]);
    }
    
    // Setting the toggle buttons
    for (int i = 0; i < 4; i++)
    {
        oscillatorActives[i].setButtonText("Active");
        addAndMakeVisible(oscillatorActives[i]);
    }
    // Set Colors
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::purple);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::magenta);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId , juce::Colours::grey);
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colours::grey);
    getLookAndFeel().setColour(juce::ComboBox::focusedOutlineColourId, juce::Colours::grey);
    getLookAndFeel().setColour(juce::ComboBox::outlineColourId , juce::Colours::grey);
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::grey);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::magenta);
    
    // Attachments
    oscillatorOneGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator One Gain", oscillatorGains[0]);
    oscillatorTwoGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator Two Gain", oscillatorGains[1]);
    oscillatorThreeGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator Three Gain", oscillatorGains[2]);
    oscillatorFourGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator Four Gain", oscillatorGains[3]);
    oscillatorOneActiveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Oscillator One On", oscillatorActives[0]);
    oscillatorTwoActiveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Oscillator Two On", oscillatorActives[1]);
    oscillatorThreeActiveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Oscillator Three On", oscillatorActives[2]);
    oscillatorFourActiveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Oscillator Four On", oscillatorActives[3]);
    oscillatorOnePitchShiftAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator One Pitch Shift", oscillatorPitchShiftSliders[0]);
    oscillatorTwoPitchShiftAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator Two Pitch Shift", oscillatorPitchShiftSliders[1]);
    oscillatorThreePitchShiftAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator Three Pitch Shift", oscillatorPitchShiftSliders[2]);
    oscillatorFourPitchShiftAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Oscillator Four Pitch Shift", oscillatorPitchShiftSliders[3]);
    waveChoiceOneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "Wave Type Oscillator One", waveChoices[0]);
    waveChoiceTwoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "Wave Type Oscillator Two", waveChoices[1]);
    waveChoiceThreeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "Wave Type Oscillator Three", waveChoices[2]);
    waveChoiceFourAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "Wave Type Oscillator Four", waveChoices[3]);
    
}

ProjectFourSynthAudioProcessorEditor::~ProjectFourSynthAudioProcessorEditor()
{
}

//==============================================================================
void ProjectFourSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    auto purpleHue = juce::Colours::purple.getHue();
    g.fillAll (juce::Colour::fromHSV (purpleHue, 0.5f, 0.1f, 1.0f));
    g.setOpacity (1.0f);
    g.setColour (juce::Colours::white);
}

void ProjectFourSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto x = getWidth()/100;
    auto y = getHeight()/100;
    appTitle.setBounds(35*x, 5*y, 30*x, 10*y);
    auto oscillatorsStartPoint = 10 * x;
    auto oscillatorsEndPoint = 90 * x;
    auto oscillatorSection = (oscillatorsEndPoint - oscillatorsStartPoint) / 4;
    for(int i = 0; i < 4; i++)
    {
        oscillatorLabels[i].setBounds(oscillatorsStartPoint + oscillatorSection*i, 17.5*y, oscillatorSection/2, 3*y);
        oscillatorActives[i].setBounds(oscillatorsStartPoint + oscillatorSection*i + oscillatorSection/2, 17.5*y, oscillatorSection/2, 3*y);
        waveChoices[i].setBounds(oscillatorsStartPoint + oscillatorSection*i + 5*x, 22*y, oscillatorSection - 10*x, 5*y);
        oscillatorGains[i].setBounds(oscillatorsStartPoint + oscillatorSection*i + x, 32*y, oscillatorSection - 2*x, 6*y);
        oscillatorPitchShiftSliders[i].setBounds(oscillatorsStartPoint + oscillatorSection*i, 43*y, oscillatorSection, 15 *y);
    }
    
}
