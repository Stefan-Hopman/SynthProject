/*
  ==============================================================================

    LFO.cpp
    Created: 27 Oct 2022 12:32:09am
    Author:  Stefan Hopman But Much Inspiration Was Taken From Will Pirkle's DSP Book

  ==============================================================================
*/

#include "LFO.h"

void LFO::reset(const float& sampleRate)
{
    	_sampleRate = sampleRate;
		phaseInc = _params.frequency_Hz / sampleRate;

		// timebase variables
		modCounter = 0.0;			//modulo counter [0.0, +1.0]
		modCounterQP = 0.25;		// Quad Phase modulo counter [0.0, +1.0]
    
}

void LFO::setParameters(const LFOParameters& params)
{
    if(_params.frequency_Hz != params.frequency_Hz)
    {
	    // update phase inc based on osc freq and fs
	    phaseInc = params.frequency_Hz / _sampleRate;
    }

	_params = params;
}

float LFO::generateAudioSample()
{
    float output = 0.f;
    checkAndWrapModulo(modCounter, phaseInc);
    if(_params.waveform == generatorLFOWaveforms::Sin)
    {
        float angle = modCounter*2.0*kPi - kPi;
        output = parabolicSine(-angle);
        
    }
    else if (_params.waveform == generatorLFOWaveforms::Triangle)
    {
        output = unipolarToBipolar(modCounter);
        output = 2.0*fabs(output) - 1.0;
    }
    else if (_params.waveform == generatorLFOWaveforms::Saw)
    {
        output = unipolarToBipolar(modCounter);
    }
    advanceModulo(modCounter, phaseInc);
    return output;
    
}
