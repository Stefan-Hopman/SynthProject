/*
  ==============================================================================

    AdditiveWavetableSynth.h
    Created: 26 Nov 2022 7:42:57pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once

#include "WavetableSynth.h"


struct AdditiveWavetableSynth_Parameters
{
    AdditiveWavetableSynth_Parameters() {}
    AdditiveWavetableSynth_Parameters& operator=(const AdditiveWavetableSynth_Parameters& params)
    {
        if (this == &params)
        {
            return *this;
        }
        waveTypes = params.waveTypes;
        attackTime = params.attackTime;
        decayTime = params.decayTime;
        releaseTime = params.releaseTime;
        sustainLevel = params.sustainLevel;
        gains = params.gains;
        activeStates = params.activeStates;
        return *this;
    }
    
    float attackTime = 0.01f;
    float decayTime = 0.01f;
    float releaseTime = 0.01f;
    float sustainLevel = 1.0f;
    std::vector<float> gains;
    std::vector<bool> activeStates;
    std::vector<WaveType> waveTypes;
};


class AdditiveWavetableSynth
{
public:
    AdditiveWavetableSynth(){};
    AdditiveWavetableSynth(const int synthCount);
    ~AdditiveWavetableSynth();
    // sets the sample rate for processing (analogously to prepareToPlay() from PluginProcessor).
    void prepareToPlay(double& sampleRate);
    // process an audio block from JUCE
    // called from the PluginProcessor's processBlock()
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    // generates samples in the [beginSample, endSample) range (Standard Template Library-style range).
    void render(juce::AudioBuffer<float>& buffer, const int& beginSample, const int& endSample);
    // translates a MIDI message to the synthesizer’s parameters change.
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    // updates the parameters of the synth
    void setParameters(const AdditiveWavetableSynth_Parameters& params);
    // retrieve the parameters of the synth
    AdditiveWavetableSynth_Parameters getParameters()
    {
        return _params;
    }
private:
    AdditiveWavetableSynth_Parameters _params;
    int _synthCount = 0;;
    double _sampleRate;
    std::vector<WavetableSynth> _synths;
};
