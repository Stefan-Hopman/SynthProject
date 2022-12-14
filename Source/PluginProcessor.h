/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AdditiveWavetableSynth.h"
#include "Modulation.h"
#include "Distorsion.h"
#include "Biquad.h"
#include "PresetManager.h"
#include "Reverb.h"
#include "Gain.h"

#define MAX_DELAY_MS 2000
#define MAX_DELAY_NOTE 16
//==============================================================================
/**
*/
class ProjectFourSynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ProjectFourSynthAudioProcessor();
    ~ProjectFourSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};
    Service::PresetManager& getPresetManager() {return *presetManager; }
    void reset() override;
private:
    //==============================================================================
    void updateParameters();
    AdditiveWavetableSynth additiveWaveTableSynth = AdditiveWavetableSynth(4); // 4 synths in the additive 
    Modulation modulationFx; // effect for different modulations (vibrato, chorus, flanger)
    Distorsion distorsionFx; // effect for different distorsions
    BiquadTemplateFilter synthFilter;
    Reverb reverbFx; // effect for reverb
    Gain masterGainFx; // effect for the master
    // Audio plugin parameters
    std::unique_ptr<Service::PresetManager> presetManager;
    // smoothing parameters
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectFourSynthAudioProcessor)
};
