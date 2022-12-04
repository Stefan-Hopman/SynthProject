/*
  ==============================================================================

    Distorsion.cpp
    Created: 29 Nov 2022 5:46:41pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "Distorsion.h"

float Distorsion::processAudioSample(const float& xn)
{
    float realTimeDrive = applySmoothing(_params.drive, _s_Drive, _linIncDrive);
    float realTimeMixPct = applySmoothing(_params.mixPct, _s_MixPct, _linIncMixPct);
    
    float mix = (realTimeMixPct - 50.f)/50.f + 1.f;
    float bClean = static_cast<float>(std::cos(mix * kPi_2/4.f));
    float bDistorted = static_cast<float>(std::sin(mix * kPi_2/4.f));
    if(_params.algorithm == distorsionAlgorithm::None)
    {
        return xn;
    }
    else if(_params.algorithm == distorsionAlgorithm::Tanh)
    {
        float distortedAudio = std::tanh(realTimeDrive * xn) / std::tanh(realTimeDrive);
        
        return bDistorted * distortedAudio + bClean * xn;
    }
    else if (_params.algorithm == distorsionAlgorithm::Atan)
    {
        float distortedAudio = std::atan(realTimeDrive * xn) / std::atan(realTimeDrive);
        return bDistorted * distortedAudio + bClean * xn;
    }
    else if (_params.algorithm == distorsionAlgorithm::Fuzz)
    {
        float distortedAudio = Sign(xn) * ((1.f - std::expf(-1.f * std::abs(realTimeDrive * xn))) / (1.f - std::expf(-1.f * realTimeDrive)));
        return bDistorted * distortedAudio + bClean * xn;
    }
    return xn;
}

void Distorsion::reset(const float& sampleRate)
{
    _sampleRate = sampleRate;
    _s_Drive = 0.f;
    _s_MixPct = 0.f;
    calculateSmoothingCoeffs();
    
}

void Distorsion::calculateSmoothingCoeffs()
{
    _linIncDrive =  (50.f - 0) / (_smoothingTimeMs * 0.001f * _sampleRate);
    _linIncMixPct =  (100.f - 0.f) / (_smoothingTimeMs * 0.001f * _sampleRate);
}

void Distorsion::setParameters(const Distorsion_Parameters& params)
{
    _params = params;
}

Distorsion_Parameters Distorsion::getParametes()
{
    return _params;
}

float Distorsion::applySmoothing(float& val, float& state, const float& linInc)
{
    if(val > state)
    {
        state = state + linInc;
        if (state > val)
        {
            state = val;
        }
    }
    else if (val < state)
    {
        state = state - linInc;
        if (state < val)
        {
            state = val;
        }
    }
    if (val == state)
    {
        return val;
    }
    else
    {
        return state;
    }
}
