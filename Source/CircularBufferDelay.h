/*
  ==============================================================================

    CircularBufferDelay.h
    Created: 26 Oct 2022 11:15:30pm
    Author: Stefan Hopman But Much Inspiration Was Taken From Will Pirkle's DSP Book

  ==============================================================================
*/

#pragma once
#include "CircularBuffer.h"

struct CircularBufferDelayParameters
{
    CircularBufferDelayParameters() {}
    CircularBufferDelayParameters& operator=(const CircularBufferDelayParameters& params)
    {
        if(this == &params)
        {
            return *this;
        }
		wetLevel_dB = params.wetLevel_dB;
		dryLevel_dB = params.dryLevel_dB;
		feedback_Pct = params.feedback_Pct;
        delayInMsec = params.delayInMsec;
        return *this;
    }
    float wetLevel_dB = -3.f;  // wet output level in dB
    float dryLevel_dB = -3.f; // dry output level in dB
	float feedback_Pct = 0.f; // feedback as a % value
	float delayInMsec = 0.f;
};

class CircularBufferDelay
{
public:
    void reset(const float& sampleRate);
    float processAudioSample(const float& xn);
    CircularBufferDelayParameters getParameters()
    {
        return _params;
    }
    void setParameters(CircularBufferDelayParameters params);
    void createBuffer(const float& sampleRate, float bufferLength_mSec);
private:
// object parameters
CircularBufferDelayParameters _params;

float _sampleRate = 0.f;
float _samplesPerMSec = 0.f; // samples per millisecond, for easy access calculation
float _delayInSamples = 0.f;
float _bufferLength_mSec = 0.f;
unsigned int _bufferLength = 0;
float _wetMix = 0.707;
float _dryMix = 0.707;

CircularBuffer<float> delayBuffer;

    
};
