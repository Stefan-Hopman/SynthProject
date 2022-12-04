/*
  ==============================================================================

    Distorsion.h
    Created: 29 Nov 2022 5:46:31pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once
#include <cmath>

enum class distorsionAlgorithm { None, Tanh, Atan, Fuzz };

// Constant value for pi
const double kPi_2 = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;

struct Distorsion_Parameters
{
    Distorsion_Parameters() {}
    Distorsion_Parameters& operator=(const Distorsion_Parameters& params)
    {
        if(this == &params)
            return *this;
        algorithm = params.algorithm;
        drive = params.drive;
        mixPct = params.mixPct;
        return *this;
    }
    // individual parameters
    distorsionAlgorithm algorithm = distorsionAlgorithm::None;
    float drive = 1.f;
    float mixPct = 50.f;
    
};

inline float Sign(const float &value)
{
    if (value > 0.f)
    {
        return 1.f;
    }
    if (value < 0.f)
    {
        return -1.f;
    }
    return 0.f;
}

class Distorsion
{
public:
    Distorsion() {}
    ~Distorsion() {}
    // process an audio sample
    float processAudioSample(const float& xn);
    // reset the plugin
    void reset(const float& sampleRate);
    // set parameters of the effect
    void setParameters(const Distorsion_Parameters& params);
    // calculate the parameters
    void calculateSmoothingCoeffs();
    Distorsion_Parameters getParametes();
    // apply smoothing
    float applySmoothing(float& val, float& state, const float& linInc);
private:
    float _sampleRate = 44100.f;
    float _linIncDrive = 0.f;
    float _linIncMixPct = 0.f;
    float _s_Drive = 1.f;
    float _s_MixPct = 1.f;
    float _smoothingTimeMs = 200.f;
    Distorsion_Parameters _params;
};

