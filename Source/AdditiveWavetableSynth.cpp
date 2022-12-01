/*
  ==============================================================================

    AdditiveWavetableSynth.cpp
    Created: 26 Nov 2022 7:43:13pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "AdditiveWavetableSynth.h"

AdditiveWavetableSynth::AdditiveWavetableSynth(const int synthCount)
{
    _synthCount = synthCount;
    // Create a number of synths corresponding to the synth count;
    for(int i = 0; i < synthCount; i++)
    {
        
        WavetableSynth synthObj;
        
        _synths.emplace_back(synthObj);
    }
    for(int i = 0; i < synthCount; i++)
    {
        _params.gains.emplace_back(1.f);
        _params.activeStates.emplace_back(false);
        _params.waveTypes.emplace_back(WaveType::Sine);
        _params.pitchShift.emplace_back(0.f);
    }
    for(int i = 0; i < synthCount; i++)
    {
        WavetableSynth_Parameters synthParameters;
        synthParameters.attackTime = _params.attackTime;
        synthParameters.releaseTime = _params.releaseTime;
        synthParameters.sustainLevel = _params.sustainLevel;
        synthParameters.decayTime = _params.decayTime;
        synthParameters.active = _params.activeStates[i];
        synthParameters.waveType = _params.waveTypes[i];
        synthParameters.gain = _params.gains[i];
        synthParameters.pitchShift = _params.pitchShift[i];
        _synths[i].setParameters(synthParameters);
    }
}

AdditiveWavetableSynth::~AdditiveWavetableSynth()
{
   
}

void AdditiveWavetableSynth::prepareToPlay(double& sampleRate)
{
    _sampleRate = sampleRate;
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i].prepareToPlay(sampleRate);
    }
}

void AdditiveWavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i].processBlock(buffer, midiMessages);
    }
}

void AdditiveWavetableSynth::render(juce::AudioBuffer<float>& buffer, const int& beginSample, const int& endSample)
{
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i].render(buffer, beginSample, endSample);
    }
}

void AdditiveWavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i].handleMidiEvent(midiEvent);
    }
}

// updates the parameters of the synth
void AdditiveWavetableSynth::setParameters(const AdditiveWavetableSynth_Parameters& params)
{
    _params = params;
    for (int i = 0; i < _synthCount; i++)
    {
        WavetableSynth_Parameters wavetableParams = _synths[i].getParameters();
        wavetableParams.active = params.activeStates[i];
        wavetableParams.gain = params.gains[i];
        wavetableParams.attackTime = params.attackTime;
        wavetableParams.releaseTime = params.releaseTime;
        wavetableParams.decayTime = params.decayTime;
        wavetableParams.sustainLevel = params.sustainLevel;
        wavetableParams.waveType = params.waveTypes[i];
        wavetableParams.pitchShift = params.pitchShift[i];
        _synths[i].setParameters(wavetableParams);
    }
    
}
