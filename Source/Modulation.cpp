/*
  ==============================================================================

    Modulation.cpp
    Created: 26 Oct 2022 11:00:47pm
    Author:  Stefan Hopman But Much Inspiration Was Taken From Will Pirkle's DSP Book

  ==============================================================================
*/

#include "Modulation.h"

void Modulation::reset(const float &sampleRate)
{
    // Delay
    _sampleRate = sampleRate;
    delay.reset(_sampleRate);
    delay.createBuffer(_sampleRate, 100.f);
    
    // LFO
    lfo.reset(_sampleRate);
    LFOParameters lfo_params = lfo.getParameters();
    lfo_params.waveform = generatorLFOWaveforms::Triangle;
    lfo.setParameters(lfo_params);
    calculateSmoothingCoeffs();
}

float Modulation::processAudioSample(const float& xn)
{
    // Smoothing intesity
    float intesityRT = 0.f;
    if (_params.lfoDepth_Pct > _s_Intesity)
    {
        _s_Intesity = _s_Intesity + _linInce_int;
        if(_s_Intesity > _params.lfoDepth_Pct)
        {
            _s_Intesity = _params.lfoDepth_Pct;
        }
    }
    else if (_params.lfoDepth_Pct < _s_Intesity)
    {
        _s_Intesity = _s_Intesity - _linInce_int;
        if (_s_Intesity < _params.lfoDepth_Pct)
        {
            _s_Intesity = _params.lfoDepth_Pct;
        }
    }
    if (_params.feedback_Pct == _s_Intesity)
    {
        intesityRT = _params.feedback_Pct;
    }
    else
    {
        intesityRT = _s_Intesity;
    }
    
    // Smoothing feedback
    float feedbackRT = 0.f;
    if (_params.feedback_Pct > _s_Feedback)
    {
        _s_Feedback = _s_Feedback + _linInc_ZeroHundred;
        if(_s_Feedback > _params.feedback_Pct)
        {
            _s_Feedback = _params.feedback_Pct;
        }
    }
    else if (_params.feedback_Pct < _s_Feedback)
    {
        _s_Feedback = _s_Feedback - _linInc_ZeroHundred;
        if (_s_Feedback < _params.feedback_Pct)
        {
            _s_Feedback = _params.feedback_Pct;
        }
    }
    if (_params.feedback_Pct == _s_Feedback)
    {
        feedbackRT = _params.feedback_Pct;
    }
    else
    {
        feedbackRT = _s_Feedback;
    }
    
    float lfoOutput = lfo.generateAudioSample();
    CircularBufferDelayParameters delayParams = delay.getParameters();
    float minDelay_mSec = 0.0;
    float maxDepth_mSec = 0.0;
    // set delay times, wet/dry and feedback based on modulation type
    if (_params.algorithm == modulationAlgorithm::Flanger)
	{
	    minDelay_mSec = 0.1;
		maxDepth_mSec = 7.0;
		delayParams.wetLevel_dB = -3.0;
		delayParams.dryLevel_dB = -3.0;
        delayParams.feedback_Pct = feedbackRT;
	}
	if (_params.algorithm == modulationAlgorithm::Chorus)
	{
		minDelay_mSec = 10.0;
		maxDepth_mSec = 30.0;
	    delayParams.wetLevel_dB = -3.0;
		delayParams.dryLevel_dB = -0.0;
		delayParams.feedback_Pct = 0.0;
	}
	if (_params.algorithm == modulationAlgorithm::Vibrato)
	{
	    minDelay_mSec = 0.0;
		maxDepth_mSec = 7.0;
		delayParams.wetLevel_dB = 0.0;
		delayParams.dryLevel_dB = -96.0;
	    delayParams.feedback_Pct = 0.0;
	}
    if (_params.algorithm == modulationAlgorithm::None)
    {
        return xn;
    }
    float depth = intesityRT / 100.0;
	float modulationMin = minDelay_mSec;
    float modulationMax = minDelay_mSec + maxDepth_mSec;
    if (_params.algorithm == modulationAlgorithm::Flanger)
    {
        delayParams.delayInMsec = doUnipolarModulationFromMin(bipolarToUnipolar(depth * lfoOutput ), modulationMin, modulationMax);
    }
    else
    {
        delayParams.delayInMsec = doBipolarModulation(depth * lfoOutput, modulationMin, modulationMax);
    }
	
    delay.setParameters(delayParams);
    return delay.processAudioSample(xn);
}

void Modulation::calculateSmoothingCoeffs()
{
    _linInce_int = (100.f - 0.f) / (_smoothingTimeMs * 2.5 * 0.001f * _sampleRate);
    _linInc_ZeroHundred = (100.f - 0.f) / (_smoothingTimeMs * 0.001f * _sampleRate);
    _linInc_LFO = (20.f - 0.1f) / (_smoothingTimeMs * 0.001f * _sampleRate);
    
}

void Modulation::setParameters(ModulationParameters& params)
{
    _params = params;
    LFOParameters lfoParams = lfo.getParameters();
    lfoParams.frequency_Hz = params.lfoRate_Hz;
    if(_params.algorithm == modulationAlgorithm::Vibrato)
    {
        lfoParams.waveform = generatorLFOWaveforms::Sin;
    }
    else
    {
        lfoParams.waveform =  generatorLFOWaveforms::Triangle;
    }
    lfo.setParameters(lfoParams);
    
    CircularBufferDelayParameters delayParams = delay.getParameters();
	delayParams.feedback_Pct = params.feedback_Pct;
	delay.setParameters(delayParams);
    
}


