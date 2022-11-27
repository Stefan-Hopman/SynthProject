/*
  ==============================================================================

    AdditiveWavetableSynth.cpp
    Created: 26 Nov 2022 7:43:13pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "AdditiveWavetableSynth.h"

AdditiveWavetableSynth::AdditiveWavetableSynth(const int& synthCount)
{
    _synthCount = synthCount;
    // Create a number of synths corresponding to the synth count;
    for(int i = 0; i < synthCount; i++)
    {
        WavetableSynth *synth;
        _synths.emplace_back(synth);
    }
}

AdditiveWavetableSynth::~AdditiveWavetableSynth()
{
    // release memory of additive synths
    for(int i = 0; i < _synthCount; i++)
    {
        _synths[i].release();
    }
}

void AdditiveWavetableSynth::prepareToPlay(double& sampleRate)
{
    _sampleRate = sampleRate;
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i]->prepareToPlay(sampleRate);
    }
}

void AdditiveWavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto currentSample = 0;
    for (const auto midiMetadata : midiMessages)
    {
        const auto message = midiMetadata.getMessage(); // retrieve the MIDI message
        const auto messagePosition = static_cast<int>(message.getTimeStamp()); // getting the time stamp of that MIDI message
        render(buffer, currentSample, messagePosition); // render the audio samples of
        currentSample = messagePosition;
        handleMidiEvent(message);
    }
    // render the rest of the audio samples
    render(buffer, currentSample, buffer.getNumSamples());
}

void AdditiveWavetableSynth::render(juce::AudioBuffer<float>& buffer, const int& beginSample, const int& endSample)
{
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i]->render(buffer, beginSample, endSample);
    }
}

void AdditiveWavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    for (int i = 0; i < _synthCount; i++)
    {
        _synths[i]->handleMidiEvent(midiEvent);
    }
}

// updates the parameters of the synth
void AdditiveWavetableSynth::setParameters(const AdditiveWavetableSynth_Parameters& params)
{
    for (int i = 0; i < _synthCount; i++)
    {
        WavetableSynth_Parameters wavetableParams;
        wavetableParams.active = params.activeStates[i];
        wavetableParams.gain = params.gains[i];
        wavetableParams.attackTime = params.attackTime;
        wavetableParams.releaseTime = params.releaseTime;
        wavetableParams.decayTime = params.decayTime;
        wavetableParams.sustainLevel = params.sustainLevel;
        wavetableParams.waveType = params.waveTypes[i];
        _synths[i]->setParameters(wavetableParams);
    }
    
}
