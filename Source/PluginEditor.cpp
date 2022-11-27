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
    setSize (1000, 600);
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
        oscillatorLabels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(oscillatorLabels[i]);
    }
    // Setting the items in the wavechoices
    for (int i = 0; i < 4; i++)
    {
        waveChoices[i].addItem("Sine", 1);
        waveChoices[i].addItem("Sawtooth", 2);
        waveChoices[i].addItem("Triangle", 3);
        waveChoices[i].addItem("Square", 4);
        addAndMakeVisible(waveChoices[i]);
    }
    // Setting the sliders for the gain sliders
    for (int i = 0; i < 4; i++)
    {
        oscillatorGains[i].setSliderStyle(juce::Slider::LinearHorizontal);
        oscillatorLabels[i].setJustificationType(juce::Justification::centred);
        oscillatorGainLabels[i].attachToComponent(&oscillatorGains[i], false);
        oscillatorGainLabels[i].setText("Volume", juce::dontSendNotification);
        oscillatorGainLabels[i].setFont(12.f);
        oscillatorGainLabels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(oscillatorGains[i]);
    }
    // Setting the sliders for the pitch shift
    for (int i = 0; i < 4; i++)
    {
        oscillatorPitchShiftSliders[i].setSliderStyle(juce::Slider::LinearHorizontal);
        oscillatorPitchShiftLabels[i].attachToComponent(&oscillatorPitchShiftSliders[i], false);
        oscillatorPitchShiftLabels[i].setText("Pitch Shift", juce::dontSendNotification);
        oscillatorPitchShiftLabels[i].setFont(12.f);
        oscillatorPitchShiftLabels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(oscillatorPitchShiftSliders[i]);
    }
    
    // Setting the toggle buttons
    for (int i = 0; i < 4; i++)
    {
        oscillatorActives[i].setButtonText("Active");
        addAndMakeVisible(oscillatorActives[i]);
    }
}

ProjectFourSynthAudioProcessorEditor::~ProjectFourSynthAudioProcessorEditor()
{
}

//==============================================================================
void ProjectFourSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
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
        oscillatorLabels[i].setBounds(oscillatorsStartPoint + oscillatorSection*i, 17.5*y, oscillatorSection, 4*y);
        waveChoices[i].setBounds(oscillatorsStartPoint + oscillatorSection*i, 22.5*y, oscillatorSection/2, 5*y);
    }
    
}
