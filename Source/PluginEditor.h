/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "XyPad.h"
#include "PresetPannel.h"
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
    
    // ADSR Labels
    juce::Label adsrTitle;
    juce::Slider attackTimeSlider;
    juce::Label attackTimeLabel;
    juce::Slider decayTimeSlider;
    juce::Label decayTimeLabel;
    juce::Slider releaseTimeSlider;
    juce::Label releaseTimeLabel;
    juce::Slider sustainLevelSlider;
    juce::Label sustainLevelLabel;
    
    // Attachments for oscillators
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
    
    // Attachments for ADSR
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainLevelTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseTimeAttachment;
    
    // Modulation
    juce::Label modulationLabel;
    juce::ComboBox modTypeBox;
    juce::Label modTypeLabel;
    juce::Slider lfoRateSlider;
    juce::Label lfoRateLabel;
    juce::Slider intensitySlider;
    juce::Label intensityLabel;
    juce::Slider feedbackSlider;
    juce::Label feedbackLabel;
    
    // Distorsion
    juce::Label distorsionLabel;
    juce::ComboBox distorsionTypeBox;
    juce::Label driveLabel;
    juce::Slider driveSlider;
    juce::Label mixPctLabel;
    juce::Slider mixPctSlider;
    
    // Filtering
    juce::Label filterLabel;
    juce::ComboBox filterTypeBox;
    juce::Label filterQLabel;
    juce::Slider filterQSlider;
    juce::Label filterFcLabel;
    juce::Slider filterFcSlider;
    
    // XY Pad
    GUI::XyPad xyPad;
    // XY Pad Combo Boxes
    juce::Label xyPadBoxXAxisLabel;
    juce::ComboBox xyPadBoxXAxis;
    juce::Label xyPadBoxYAxisLabel;
    juce::ComboBox xyPadBoxYAxis;
     
    // Preset
    GUI::PresetPanel presetPanel;
    
    // Modulation Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modTypeBoxAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> intesityAttachment;
    
    // Pitch Shift Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchShiftOneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchShiftTwoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchShiftThreeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchShiftFourAttachment;
    
    // Distorsion Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> distorsionTypeBoxAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixPctAttachment;
    
    // Filter Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fcSliderAttachment;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectFourSynthAudioProcessorEditor)
};
