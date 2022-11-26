
#include "Gain.h"
#include <cmath>


void Gain::reset()
{
    _s_One = 0.f;
}

float Gain::processAudioSample(const float& xn)
{
    float _realTimeGaindB = 0.f;
    if (_gainVal_dB > _s_One)
    {
        _s_One = _s_One + _linInc;
        if(_s_One > _gainVal_dB)
        {
            _s_One = _gainVal_dB;
        }
    }
    else if (_gainVal_dB < _s_One)
    {
        _s_One = _s_One - _linInc;
        if (_s_One < _gainVal_dB)
        {
            _s_One = _gainVal_dB;
        }
    }
    if (_gainVal_dB == _s_One)
    {
        _realTimeGaindB = _gainVal_dB;
    }
    else
    {
        _realTimeGaindB = _s_One;
    }
    float realTimeGainLinear = powf(10.f, (_realTimeGaindB/20.f));
    return xn * realTimeGainLinear;
}

void Gain::setGainValue(const float& gainVal)
{
    _gainVal_dB = gainVal;
}

void Gain::setSampleRate(const float &sampleRate)
{
    _sampleRate = sampleRate;
    calculateSmoothingCoeffs();
    reset();
}

float Gain::getGainValue()
{
    return _gainVal_dB;
}

void Gain::calculateSmoothingCoeffs()
{
    _linInc = (0.f - (-60.f)) / (_smoothingTimeMs * 0.001f * _sampleRate);
}
