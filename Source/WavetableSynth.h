/*
  ==============================================================================

    WavetableSynth.h
    Created: 13 Oct 2022 3:20:27pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "WavetableOscillator.h"

enum class WaveType
{
    Sine,Sawtooth,Triangle,Square,WhiteNoise
};

struct WavetableSynth_Parameters
{
    WavetableSynth_Parameters() {}
    WavetableSynth_Parameters& operator=(const WavetableSynth_Parameters& params)
    {
        if (this == &params)
        {
            return *this;
        }
        waveType = params.waveType;
        attackTime = params.attackTime;
        decayTime = params.decayTime;
        releaseTime = params.releaseTime;
        sustainLevel = params.sustainLevel;
        gain = params.gain;
        active = params.active;
        pitchShift = params.pitchShift;
        return *this;
    }
    WaveType waveType = WaveType::Sine;
    float attackTime = 0.01f;
    float decayTime = 0.01f;
    float releaseTime = 0.01f;
    float sustainLevel = 1.0f;
    float gain = 1.0f;
    float pitchShift = 0.f;
    bool active = false;
};


class WavetableSynth
{
public:
    WavetableSynth() = default;
    WavetableSynth(const WavetableSynth&) = default;
    WavetableSynth(WavetableSynth&&) = default;
    
    // sets the sample rate for processing (analogously to prepareToPlay() from PluginProcessor).
    void prepareToPlay(double& sampleRate);
    // called from the PluginProcessor's processBlock()
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    // updates the parameters of the synth
    void setParameters(const WavetableSynth_Parameters& params);
    // retrieve the parameters of the synth
    WavetableSynth_Parameters getParameters()
    {
        return _params;
    }
    // generates samples in the [beginSample, endSample) range (Standard Template Library-style range).
    void render(juce::AudioBuffer<float>& buffer, const int& beginSample, const int& endSample);
    // Multiplier to allow for pitch shifting
    float halfStepToFreqMultiplier(const int& halfSteps);
    // translates a MIDI message to the synthesizer???s parameters change.
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    
    // calculate smoothing parameters
    void calculateSmoothingCoeffs();
    // apply smoothing
    float applySmoothing(float& val, float& state, const float& linInc);
private:
    // Structs for keeping data
    WavetableSynth_Parameters _params;
    // - variables
    double _sampleRate;
   
    const int _oscillatorCount = 128;
    const int _WaveTableLength = 4096;
    std::vector<WavetableOscillator> _oscillators;
    // - helper functions
    // generates samples of a sine wave period.
    std::vector<float> generateSineWaveTable();
    // generates samples of a sawtooth wave period.
    std::vector<float> generateSawtoothWaveTable();
    // generates samples of a triangle wave period.
    std::vector<float> generateTriangularWaveTable();
    // generates samples of a square wave period.
    std::vector<float> generateSquareWaveTable();
    // generates samples of a white noise period
    std::vector<float> generateWhiteNoiseWaveTable();
    // keeps track of the number of active MIDI note numbers
    std::vector<int> activeOscillatorIds;
    
    // converts a MIDI note number (an integer corresponding to a key on a MIDI keyboard) to frequency in Hz (assuming a certain tuning of the piano).
    static float midiNoteNumberToFrequency(int midiNoteNumber);
    // initializes 128 oscillators as wave table oscillators.
    void initializeOscillators();
    // update all 128 osicillators for a certain wave type
    void updateOscillators(WaveType wave);
    // updatate all 128 on a new envelope
    void updateOscillatorEnvelopes();
    // updatate all 128 on a new envelope with a new sustain level
    void updateOscillatorEnvelopes(const float &sustainLevel);
    
    // Smoothing Parameters
    float _smoothingTimeMs = 100.f;
    float _linearIncrementOscVolume = 0.f;
    float _s_OscVol = 0.f;
    float _linearIncrementSustain = 0.f;
    float _s_Sustain = 1.f;
};

