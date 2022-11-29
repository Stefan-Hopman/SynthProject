/*
  ==============================================================================

    Modulation.h
    Created: 26 Oct 2022 11:00:42pm
    Author:  Stefan Hopman But Much Inspiration Was Taken From Will Pirkle's DSP Book

  ==============================================================================
*/

#pragma once
#include "CircularBufferDelay.h"
#include "LFO.h"

inline void boundValue(double& value, double minValue, double maxValue)
{
    value = fmin(value, maxValue);
    value = fmax(value, minValue);
}

inline double bipolarToUnipolar(double value)
{
    return 0.5*value + 0.5;
}

inline double doUnipolarModulationFromMin(double unipolarModulatorValue, double minValue, double maxValue)
{
    // --- UNIPOLAR bound
    boundValue(unipolarModulatorValue, 0.0, 1.0);

    // --- modulate from minimum value upwards
    return unipolarModulatorValue*(maxValue - minValue) + minValue;
}
inline double doBipolarModulation(double bipolarModulatorValue, double minValue, double maxValue)
{
    // --- BIPOLAR bound
    boundValue(bipolarModulatorValue, -1.0, 1.0);

    // --- calculate range and midpoint
    double halfRange = (maxValue - minValue) / 2.0;
    double midpoint = halfRange + minValue;

    return bipolarModulatorValue*(halfRange) + midpoint;
}

enum class modulationAlgorithm { None, Flanger, Chorus, Vibrato };

struct ModulationParameters
{
	ModulationParameters() {}

    ModulationParameters& operator=(const ModulationParameters& params)
	{
		if (this == &params)
			return *this;

		algorithm = params.algorithm;
		lfoRate_Hz = params.lfoRate_Hz;
		lfoDepth_Pct = params.lfoDepth_Pct;
		feedback_Pct = params.feedback_Pct;
		return *this;
	}

	// individual parameters
    modulationAlgorithm algorithm = modulationAlgorithm::Flanger; // modulation algorithm
	double lfoRate_Hz = 0.0;	// mod delay LFO rate in Hz
	double lfoDepth_Pct = 0.0;	// mod delay LFO depth in %
	double feedback_Pct = 0.0;	// feedback in %
};

// The Modulation object implements the three basic algorithms: flanger, chorus, vibrato.
class Modulation
{
public:
    Modulation(){}
    ~Modulation(){}
    void reset(const float& sampleRate);
    float processAudioSample(const float& xn);
    void setParameters(ModulationParameters& params);
    // calculate the coefficients of the linear increment
    void calculateSmoothingCoeffs();
    ModulationParameters getParameters()
    {
        return _params;
    }
    
private:
    LFO lfo;
    CircularBufferDelay delay;
    ModulationParameters _params;
    float _sampleRate;
    // smoothing
    float _linInc_ZeroHundred = 0.f;
    float _linInce_int = 0.f;
    float _linInc_LFO = 0.f;
    float _s_Feedback = 0.f;
    float _s_Intesity = 0.f;
    float _s_lfoRate = 0.f;
    float _smoothingTimeMs = 20.f;
    float _IntesityRT = 0.f;
    float _lfoRateRT = 0.f;
    
};
