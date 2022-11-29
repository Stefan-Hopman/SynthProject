/*
  ==============================================================================

    CircularBufferDelay.cpp
    Created: 27 Oct 2022 12:02:04am
    Author:  Stefan Hopman But Much Inspiration Was Taken From Will Pirkle's DSP Book

  ==============================================================================
*/

#include "CircularBufferDelay.h"


void CircularBufferDelay::reset(const float& sampleRate)
{

    if(_sampleRate == sampleRate)
    {
        delayBuffer.flushBuffer();
        return;
    }
    _sampleRate = sampleRate;
    _samplesPerMSec = sampleRate / 1000.0;
    // total buffer length including fractional part
    _bufferLength = (unsigned int)(_bufferLength_mSec*(_samplesPerMSec)) + 1;
    delayBuffer.createCircularBuffer(_bufferLength);
}

void CircularBufferDelay::createBuffer(const float& sampleRate, float bufferLength_mSec)
{
    _sampleRate = sampleRate;
    _samplesPerMSec = sampleRate / 1000.0;
    // total buffer length including fractional part
    _bufferLength_mSec = bufferLength_mSec;
    _bufferLength = (unsigned int)(_bufferLength_mSec*(_samplesPerMSec)) + 1;
    delayBuffer.createCircularBuffer(_bufferLength);
}

float CircularBufferDelay::processAudioSample(const float& xn)
{
        // read delay
		float yn = delayBuffer.readBuffer(_delayInSamples);

		// create input for delay buffer
		float dn = xn + (_params.feedback_Pct / 100.0) * yn;

		// write to delay buffer
		delayBuffer.writeBuffer(dn);

		// form mixture out = dry*xn + wet*yn
		float output = _dryMix*xn + _wetMix*yn;

		return output;
}

void CircularBufferDelay::setParameters(CircularBufferDelayParameters params)
{
    	// check mix in dB for calc
		if (_params.dryLevel_dB != params.dryLevel_dB)
			_dryMix = pow(10.0, params.dryLevel_dB / 20.0);
		if (_params.wetLevel_dB != params.wetLevel_dB)
			_wetMix = pow(10.0, _params.wetLevel_dB / 20.0);

		// save; rest of updates are cheap on CPU
		_params = params;
		
		// find the new delay valies
		float newDelayInSamples = params.delayInMsec*(_samplesPerMSec);
		_delayInSamples = newDelayInSamples;
}
