/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 13 Oct 2022 3:46:50pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "WavetableOscillator.h"
#include <cmath>


WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate) : waveTable{std::move(waveTable)}, _sampleRate{sampleRate}
{
    _ADSRParams.attack = 0.1f;
    _ADSRParams.decay = 0.f;
    _ADSRParams.sustain = 1.0f;
    _ADSRParams.release = 0.1f;
    _ADSRManager.setParameters(_ADSRParams);
}

float WavetableOscillator::getSample()
{
    if(_ADSRManager.isIdle() == true)
    {
        this->stop();
        return 0.f;
    }
    const float sample = interpolateLinearly();
    index += indexIncrement;
    index = std::fmod(index, static_cast<float>(waveTable.size()));
    
    return sample;
}

float WavetableOscillator::applyADSR(const float& sample)
{
    return _ADSRManager.applyEnvelopeToSample(sample);
}

void WavetableOscillator::setADSR(const Stefan::ADSR::Parameters& ADSRParameters)
{
    _ADSRParams = ADSRParameters;
    _ADSRManager.setParameters(_ADSRParams);
}


void WavetableOscillator::setFrequency(float& frequency)
{
    _ADSRManager.noteOn();
    indexIncrement = frequency * static_cast<float>(waveTable.size()) / static_cast<float>(_sampleRate);
}

void WavetableOscillator::noteOff()
{
    _ADSRManager.noteOff();
}

void WavetableOscillator::stop()
{
    index = 0.f;
    indexIncrement = 0.f;
}

bool WavetableOscillator::isPlaying() const
{
    return indexIncrement != 0.f;
}

float WavetableOscillator::interpolateLinearly() const
{
    // is the largest index not larger than the index
    const auto truncatedIndex = static_cast<typename  decltype(waveTable)::size_type>(index);
    // nextIndex is the smallest integer index larger than index or 0 if trancutatedIndex is equal to waveTable.size() -1
    const auto nextIndex = static_cast<typename  decltype(waveTable)::size_type>(std::ceil(index)) % waveTable.size();
    // nextIndexWeight is the weight we put on waveTable[nextIndex] in the returned sum.
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    /*
     In linear interpolation, we want to return a * waveTable[truncatedIndex] + b * waveTable[nextIndex], where a + b == 1. Additionally, we fix the ratio b / a to be equal to the ratio (index - truncatedIndex) / (nextIndex - index) (apart from the edge case where nextIndex is 0).
     */
    return waveTable[nextIndex] * nextIndexWeight + (1.f - nextIndexWeight) * waveTable[truncatedIndex];
}

