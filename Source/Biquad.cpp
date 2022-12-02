/*
  ==============================================================================

    Biquad.cpp
    Created: 13 Nov 2022 8:49:24pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "Biquad.h"

#include <cmath>
// Constants

const double kPi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;

// Second Order Peak Filter

void SecondOrderPeakNotchFilter::reset()
{
    _biquad.reset();
}

void SecondOrderPeakNotchFilter::setSampleRate(const float& sampleRate)
{
    _sampleRate = sampleRate;
    _biquad.reset();
    calculateCoefficients();
}
void SecondOrderPeakNotchFilter::calculateCoefficients()
{
    float V0 = pow(10.f, (_params.gain/20.f));
    float K = tan(kPi * _params.crossOverFrequency / _sampleRate);
    float b0 = 0.f;
    float b1 = 0.f;
    float b2 = 0.f;
    float a1 = 0.f;
    float a2 = 0.f;
    if (_params.gain >= 0.f) // boost
    {
        b0 = (1.f + (V0/_params.Q)*K + pow(K,2.f)) / (1.f + (1.f/_params.Q)*K + pow(K,2.f));
        b1 = (2.f * (pow(K,2.f) - 1.f)) / (1.f + (1.f/_params.Q)*K + pow(K,2.f));
        b2 = (1.f - (V0/_params.Q)*K + pow(K,2.f)) / (1.f + (1.f/_params.Q)*K + pow(K,2.f));
        a1 = (2.f * (pow(K,2.f) -1.f)) / (1.f + (1.f/_params.Q) * K + pow(K,2.f));
        a2 = (1.f - (1.f/_params.Q)*K + pow(K,2.f)) / (1.f + (1.f/_params.Q)*K + pow(K,2.f));
    }
    else // cut
    {
        b0 = (1.f + (1.f/_params.Q)*K + pow(K,2.f)) / (1.f + (1.f/(V0 *_params.Q))*K + pow(K,2.f));
        b1 = (2.f * (pow(K,2) - 1.f)) / (1.f + (1/(V0 * _params.Q))*K + pow(K,2.f));
        b2 = (1.f - (1.f/_params.Q)*K + pow(K,2)) / (1.f + (1.f/(V0*_params.Q))*K + pow(K,2));
        a1 = (2.f * (pow(K,2.f) - 1.f)) / (1.f + (1.f/(V0 * _params.Q))*K + pow(K,2.f));
        a2 = (1.f - (1.f/(V0*_params.Q))*K + pow(K, 2)) / (1.f + (1.f/(V0*_params.Q))*K + pow(K,2));
    }
    _biquad.setCoeffs(b0, b1, b2, a1, a2);
}


void SecondOrderPeakNotchFilter::setParameters(const SecondOrderPeakNotchFilterParameters& params)
{
    if(_params.Q != params.Q || _params.crossOverFrequency != params.crossOverFrequency || _params.gain != params.gain)
    {
        _params = params;
        calculateCoefficients();
    }
}

float SecondOrderPeakNotchFilter::processAudioSample(const float& xn)
{
    return _biquad.processAudioSample(xn);
}




BiquadTemplateFilter::BiquadTemplateFilter()
{
    _biquadChainCount = 1;
    _biquadChains.emplace_back(BiquadDesign());
    calculateCoefficients();
}

void BiquadTemplateFilter::reset()
{
    for(int i = 0; i < _biquadChains.size(); i++)
    {
        _biquadChains[i].reset();
    }
}


void BiquadTemplateFilter::calculateCoefficients()
{
    if (_params.filterType == BiquadFilterType::HPF2)
    {
        float theta_c = 2.f * static_cast<float>(kPi) * _params.crossOverFrequency / _sampleRate;
        float d = 1.f / _params.Q;
        float betaNumerator = 1.f - ((d / 2.f) * (std::sinf(theta_c)));
        float betaDenominator = 1.f + ((d / 2.f) * (std::sinf(theta_c)));
        float beta = 0.5f * (betaNumerator / betaDenominator);
        float gamma = (0.5f + beta) * (std::cosf(theta_c));
        float alpha = (0.5f + beta + gamma) / 2.f;
        
        float b0 = alpha;
        float b1 = -2.f * alpha;
        float b2 = alpha;
        float a1 = -2.f * gamma;
        float a2 = 2.f * beta;
        
        int biquadChainCount = 1;
        resetBiquadChains(biquadChainCount);
        for(int i = 0; i < _biquadChains.size(); i++)
        {
            _biquadChains[i].setCoeffs(b0, b1, b2, a1, a2);
        }
        return;
    }
    else if (_params.filterType == BiquadFilterType::HPF4)
    {
        float theta_c = 2.f * static_cast<float>(kPi) * _params.crossOverFrequency / _sampleRate;
        float d = 1.f / _params.Q;
        float betaNumerator = 1.f - ((d / 2.f) * (std::sinf(theta_c)));
        float betaDenominator = 1.f + ((d / 2.f) * (std::sinf(theta_c)));
        float beta = 0.5f * (betaNumerator / betaDenominator);
        float gamma = (0.5f + beta) * (std::cosf(theta_c));
        float alpha = (0.5f + beta + gamma) / 2.f;
        
        float b0 = alpha;
        float b1 = -2.f * alpha;
        float b2 = alpha;
        float a1 = -2.f * gamma;
        float a2 = 2.f * beta;
        
        int biquadChainCount = 2;
        resetBiquadChains(biquadChainCount);
        for(int i = 0; i < _biquadChains.size(); i++)
        {
            _biquadChains[i].setCoeffs(b0, b1, b2, a1, a2);
        }
        return;
    }
    else if (_params.filterType == BiquadFilterType::LPF2)
    {
        float theta_c = 2.f * static_cast<float>(kPi) * _params.crossOverFrequency / _sampleRate;
        float d = 1.f / _params.Q;
        float betaNumerator = 1.f - ((d / 2.f) * (std::sinf(theta_c)));
        float betaDenominator = 1.f + ((d / 2.f) * (std::sinf(theta_c)));
        
        float beta = 0.5f * (betaNumerator / betaDenominator);
        float gamma = (0.5f + beta) * (std::cosf(theta_c));
        float alpha = (0.5f + beta - gamma) / 2.f;
        
        float b0 = alpha;
        float b1 = 2.f * alpha;
        float b2 = alpha;
        float a1 = -2.f * gamma;
        float a2 = 2.f * beta;
        
        int biquadChainCount = 1;
        resetBiquadChains(biquadChainCount);
        for(int i = 0; i < _biquadChains.size(); i++)
        {
            _biquadChains[i].setCoeffs(b0, b1, b2, a1, a2);
        }
        return;
    }
    else if (_params.filterType == BiquadFilterType::LPF4)
    {
        float theta_c = 2.f * static_cast<float>(kPi) * _params.crossOverFrequency / _sampleRate;
        float d = 1.f / _params.Q;
        float betaNumerator = 1.f - ((d / 2.f) * (std::sinf(theta_c)));
        float betaDenominator = 1.f + ((d / 2.f) * (std::sinf(theta_c)));
        
        float beta = 0.5f * (betaNumerator / betaDenominator);
        float gamma = (0.5f + beta) * (std::cosf(theta_c));
        float alpha = (0.5f + beta - gamma) / 2.f;
        
        float b0 = alpha;
        float b1 = 2.f * alpha;
        float b2 = alpha;
        float a1 = -2.f * gamma;
        float a2 = 2.f * beta;
        
        int biquadChainCount = 1;
        resetBiquadChains(biquadChainCount);
        for(int i = 0; i < _biquadChains.size(); i++)
        {
            _biquadChains[i].setCoeffs(b0, b1, b2, a1, a2);
        }
        return;
    }
    else if (_params.filterType == BiquadFilterType::BPF2)
    {
        float K = std::tanf(static_cast<float>(kPi) *_params.crossOverFrequency/ _sampleRate);
        float delta = K * K * _params.Q + K +_params.Q;
        float b0 = K / delta;
        float b1 = 0.f;
        float b2 = -1.f * K / delta;
        float a1 = 2.f * _params.Q * (K * K - 1.f) / delta;
        float a2 = (K * K * _params.Q - K + _params.Q) / delta;
        
        int biquadChainCount = 1;
        resetBiquadChains(biquadChainCount);
        for(int i = 0; i < _biquadChains.size(); i++)
        {
            _biquadChains[i].setCoeffs(b0, b1, b2, a1, a2);
        }
        return;
    }
    else if (_params.filterType == BiquadFilterType::BPF4)
    {
        float K = std::tanf(static_cast<float>(kPi) *_params.crossOverFrequency/ _sampleRate);
        float delta = K * K * _params.Q + K +_params.Q;
        float b0 = K / delta;
        float b1 = 0.f;
        float b2 = -1.f * K / delta;
        float a1 = 2.f * _params.Q * (K * K - 1.f) / delta;
        float a2 = (K * K * _params.Q - K + _params.Q) / delta;
        
        int biquadChainCount = 2;
        resetBiquadChains(biquadChainCount);
        for(int i = 0; i < _biquadChains.size(); i++)
        {
            _biquadChains[i].setCoeffs(b0, b1, b2, a1, a2);
        }
        return;
    }
}

void BiquadTemplateFilter::setSampleRate(const float& sampleRate)
{
    _sampleRate = sampleRate;
    calculateCoefficients();
}

void BiquadTemplateFilter::resetBiquadChains(const int biquadChainCount)
{
    if(_biquadChains.size() == biquadChainCount)
    {
        return;
    }
    else if (_biquadChains.size() < biquadChainCount)
    {
        while(_biquadChains.size() != biquadChainCount)
        {
            _biquadChains.emplace_back(BiquadDesign());
        }
    }
    else if (_biquadChains.size() > biquadChainCount)
    {
        while(_biquadChains.size() != biquadChainCount)
        {
            _biquadChains.pop_back();
        }
    }
}

void BiquadTemplateFilter::setParameters(const BiquadTempalteFilterParamters& params)
{
    if(_params.Q != params.Q || _params.crossOverFrequency != params.crossOverFrequency || _params.filterType != params.filterType)
    {
        _params = params;
        calculateCoefficients();
    }
}

float BiquadTemplateFilter::processAudioSample(const float& xn)
{
    float yn = xn;
    for(int i = 0; i < _biquadChains.size(); i++)
    {
        yn = _biquadChains[i].processAudioSample(yn);
    }
    return yn;
}



// Fourth Order High Pass Filter Butterworth
void FourthOrderHighPassButterworth::reset()
{
    _biquadChainOne.reset();
    _biquadChainTwo.reset();
}

void FourthOrderHighPassButterworth::setCrossOverFrequency(const float& crossOverFrequency)
{
    _crossoverFrequency = crossOverFrequency;
    calculateCoefficients();
}

void FourthOrderHighPassButterworth::calculateCoefficients()
{
    float C = tan(kPi * _crossoverFrequency / _sampleRate);
    float b0 = 1.f / (1.f + pow(2.f, 0.5f)*C + pow(C,2.f));
    float b1 = -2.f * b0;
    float b2 = b0;
    float a1 = 2.f * b0 * (pow(C,2.f) -1.f);
    float a2 = b0 * (1.f - pow(2.f, 0.5f)*C + pow(C,2));
    _biquadChainOne.setCoeffs(b0, b1, b2, a1, a2);
    _biquadChainTwo.setCoeffs(b0, b1, b2, a1, a2);
}

void FourthOrderHighPassButterworth::setSampleRate(const float& sampleRate)
{
    _sampleRate = sampleRate;
    reset();
    calculateCoefficients();
}

float FourthOrderHighPassButterworth::processAudioSample(const float& xn)
{
    float intermidiateChainSample = _biquadChainOne.processAudioSample(xn);
    return _biquadChainTwo.processAudioSample(intermidiateChainSample);
}

// Fourth Order Low Pass Filter
void FourthOrderLowPassButterworth::reset()
{
    _biquadChainOne.reset();
    _biquadChainTwo.reset();
}

void FourthOrderLowPassButterworth::calculateCoefficients()
{
    float C = 1.f / tan(kPi * _crossoverFrequency / _sampleRate);
    float b0 = 1.f / (1.f + pow(2.f, 0.5f)*C + pow(C,2.f));
    float b1 = 2.f * b0;
    float b2 = b0;
    float a1 = 2.f * b0 * (1.f - pow(C,2.f));
    float a2 = b0 * (1.f - pow(2.f, 0.5f)*C + pow(C,2.f));
    _biquadChainOne.setCoeffs(b0, b1, b2, a1, a2);
    _biquadChainTwo.setCoeffs(b0, b1, b2, a1, a2);
}

void FourthOrderLowPassButterworth::setCrossOverFrequency(const float& crossOverFrequency)
{
    _crossoverFrequency = crossOverFrequency;
    calculateCoefficients();
}


void FourthOrderLowPassButterworth::setSampleRate(const float& sampleRate)
{
    _sampleRate = sampleRate;
    reset();
    calculateCoefficients();
}

float FourthOrderLowPassButterworth::processAudioSample(const float& xn)
{
    float intermidiateChainSample = _biquadChainOne.processAudioSample(xn);
    return _biquadChainTwo.processAudioSample(intermidiateChainSample);
}

void FourthOrderBandPassFilter::reset()
{
    _biquadChainOne.reset();
    _biquadChainTwo.reset();
}

void FourthOrderBandPassFilter::calculateCoefficients()
{
    float K = tan(kPi * _params.crossOverFrequency / _sampleRate);
    float b0 = K / (pow(K, 2.f)*_params.Q + K + _params.Q);
    float b1 = 0;
    float b2 = -b0;
    float a1 = (2.f * _params.Q * (pow(K, 2.f) - 1)) / (pow(K, 2.f) * _params.Q + K + _params.Q);
    float a2 = (pow(K, 2.f)*_params.Q - K + _params.Q)/ (pow(K,2.f) + K + _params.Q);
    _biquadChainOne.setCoeffs(b0, b1, b2, a1, a2);
    _biquadChainTwo.setCoeffs(b0, b1, b2, a1, a2);
}

float FourthOrderBandPassFilter::processAudioSample(const float& xn)
{
    float intermidiateChainSample = _biquadChainOne.processAudioSample(xn);
    return _biquadChainTwo.processAudioSample(intermidiateChainSample);
    
}

void FourthOrderBandPassFilter::setParameters(const FourthOrderBandPassFilterParameters& params)
{
    if (_params.Q != params.Q || _params.crossOverFrequency != params.crossOverFrequency)
    {
        _params = params;
        calculateCoefficients();
    }
}

void FourthOrderBandPassFilter::setSampleRate(const float& sampleRate)
{
    _sampleRate = sampleRate;
    reset();
    calculateCoefficients();
}
