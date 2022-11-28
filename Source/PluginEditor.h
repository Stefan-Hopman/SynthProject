/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ProjectFourSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ProjectFourSynthAudioProcessorEditor (ProjectFourSynthAudioProcessor&);
    ~ProjectFourSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ProjectFourSynthAudioProcessor& audioProcessor;
    
    juce::Label appTitle;
    
    juce::Label oscillatorLabels[4];
    juce::ComboBox waveChoices[4];
    juce::Slider oscillatorGains[4];
    juce::Label oscillatorGainLabels[4];
    juce::Slider oscillatorPitchShiftSliders[4];
    juce::Label oscillatorPitchShiftLabels[4];
    juce::ToggleButton oscillatorActives[4];
    
    
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorOneGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorTwoGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorThreeGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorFourGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oscillatorOneActiveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oscillatorTwoActiveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oscillatorThreeActiveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oscillatorFourActiveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorOnePitchShiftAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorTwoPitchShiftAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorThreePitchShiftAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscillatorFourPitchShiftAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceOneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceTwoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceThreeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceFourAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectFourSynthAudioProcessorEditor)
};
