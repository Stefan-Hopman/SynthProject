/*
  ==============================================================================

    LFO.h
    Created: 27 Oct 2022 12:32:01am
    Author:  Stefan Hopman  But Much Inspiration Was Taken From Will Pirkle's DSP Book

  ==============================================================================
*/

#pragma once
#include <math.h>

const double kPi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;

// Enum containing the different 
enum class generatorLFOWaveforms { Triangle, Sin, Saw };

struct LFOParameters
{
    LFOParameters() {};
    LFOParameters& operator=(const LFOParameters& params)
    {
        if (this == &params)
        {
            return *this;
        }
			
		waveform = params.waveform;
		frequency_Hz = params.frequency_Hz;
		return *this;
    }
    
	generatorLFOWaveforms waveform = generatorLFOWaveforms::Triangle; // the current waveform
	float frequency_Hz = 0.0;	// oscillator frequency
    
};


class LFO
{
public:
    LFO() {}
    ~LFO() {}
    void reset(const float& sampleRate);
    LFOParameters getParameters()
    {
        return _params;
    }
    void setParameters(const LFOParameters& params);
    float generateAudioSample();

protected:
    LFOParameters _params;
    float _sampleRate = 0.f;
    // timebase variables
    float modCounter = 0.0; // modulo counter [0.0, +1.0]
    float phaseInc = 0.0; //  phase inc = fo/fs
    float modCounterQP = 0.25;
    // check the modulo counter and wrap if needed */
	inline bool checkAndWrapModulo(float& moduloCounter, float phaseInc)
	{
		//for positive frequencies
		if (phaseInc > 0 && moduloCounter >= 1.0)
		{
			moduloCounter -= 1.0;
			return true;
		}

		// for negative frequencies
		if (phaseInc < 0 && moduloCounter <= 0.0)
		{
			moduloCounter += 1.0;
			return true;
		}

		return false;
	}

	// advanvce the modulo counter, then check the modulo counter and wrap if needed */
	inline bool advanceAndCheckWrapModulo(float& moduloCounter, float phaseInc)
	{
		// advance counter
		moduloCounter += phaseInc;

		// for positive frequencies
		if (phaseInc > 0 && moduloCounter >= 1.0)
		{
			moduloCounter -= 1.0;
			return true;
		}

		// for negative frequencies
		if (phaseInc < 0 && moduloCounter <= 0.0)
		{
			moduloCounter += 1.0;
			return true;
		}

		return false;
	}

	// advanvce the modulo counter
	inline void advanceModulo(float& moduloCounter, float phaseInc) { moduloCounter += phaseInc; }

	const double B = 4.0 / kPi;
	const double C = -4.0 / (kPi* kPi);
	const double P = 0.225;
	// parabolic sinusoidal calcualtion; NOTE: input is -pi to +pi http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine */
	inline float parabolicSine(float angle)
	{
		float y = B * angle + C * angle * fabs(angle);
		y = P * (y * fabs(y) - y) + y;
		return y;
	}
	inline float unipolarToBipolar(float value)
    {
	    return 2.0*value - 1.0;
    }
	
	
	
};
