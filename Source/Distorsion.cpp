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
    float mix = (_params.mixPct - 50.f)/50.f + 1.f;
    float bClean = static_cast<float>(std::cos(mix * kPi_2/4.f));
    float bDistorted = static_cast<float>(std::sin(mix * kPi_2/4.f));
    if(_params.algorithm == distorsionAlgorithm::None)
    {
        return xn;
    }
    else if(_params.algorithm == distorsionAlgorithm::Tanh)
    {
        float distortedAudio = std::tanh(_params.drive * xn) / std::tanh(_params.drive);
        
        return bDistorted * distortedAudio + bClean * xn;
    }
    else if (_params.algorithm == distorsionAlgorithm::Atan)
    {
        float distortedAudio = std::atan(_params.drive * xn) / std::atan(_params.drive);
        return bDistorted * distortedAudio + bClean * xn;
    }
    else if (_params.algorithm == distorsionAlgorithm::Fuzz)
    {
        float distortedAudio = Sign(xn) * ((1.f - std::expf(-1.f * std::abs(_params.drive * xn))) / (1.f - std::expf(-1.f * _params.drive)));
        return bDistorted * distortedAudio + bClean * xn;
    }
    return xn;
}

void Distorsion::setParameters(const Distorsion_Parameters& params)
{
    _params = params;
}

Distorsion_Parameters Distorsion::getParametes()
{
    return _params;
}
