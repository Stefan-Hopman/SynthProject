/*
  ==============================================================================

    WavetableOscillator.h
    Created: 13 Oct 2022 3:46:42pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include <vector>
#include "ADSR.h"


class WavetableOscillator
{
public:
    WavetableOscillator(std::vector<std::vector<float>> waveTables, double sampleRate);
    WavetableOscillator(const WavetableOscillator&) = default;
    WavetableOscillator& operator=(const WavetableOscillator&) = default;
    WavetableOscillator(WavetableOscillator&&) = default;
    WavetableOscillator& operator=(WavetableOscillator&&) = default;
    
    float getSample();
    float applyADSR(const float& sample);
    void setADSR(const Stefan::ADSR::Parameters& ADSRParameters);
    void getSamples(float* buffer, const int& beginSample, const int &endSample);
    void setFrequency(float& frequency);
    void setWaveIndex(const int& waveIndex);
    void noteOff();
    void stop();
    
    
    bool isPlaying() const;
private:
    Stefan::ADSR _ADSRManager;
    Stefan::ADSR::Parameters _ADSRParams;
    bool releaseComplete = false;
    float interpolateLinearly() const;
    std::vector<std::vector<float>> waveTables;
    double _sampleRate = 44100.0;
    float index = 0.f;
    float indexIncrement = 0.f;
    int _waveIndex = 0;
    
    
};

