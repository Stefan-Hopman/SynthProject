/*
  ==============================================================================

    Biquad.h
    Created: 20 Sep 2022 10:43:03am
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
// All the different types of filters that the biquad object can handle
// Peak/Notch filter


class BiquadDesign
{
public:
    BiquadDesign() {}
    ~BiquadDesign() {}
    void reset()
    {
        _x1 = 0;
        _x2 = 0;
    }
    // processes an audio sample
    float processAudioSample(const float& xn)
    {
        float output = _b0 * xn + _x1;
        _x1 = _b1 * xn - _a1 * output + _x2;
        _x2 = _b2 * xn - _a2 * output;
        return output;
    }
    // sets the coefficients of the filter
    void setCoeffs(const float& b0, const float& b1, const float& b2, const float& a1, const float& a2)
    {
        _b0 = b0;
        _b1 = b1;
        _b2 = b2;
        _a1 = a1;
        _a2 = a2;
    }
private:
    float _x1 = 0, _x2 = 0;
    float _b0 = 0, _b1 = 0, _b2 = 0, _a1 = 0, _a2 = 0; // filter coefficients of a biquad
};

struct SecondOrderPeakNotchFilterParameters
{
    SecondOrderPeakNotchFilterParameters(){}
    SecondOrderPeakNotchFilterParameters& operator=(const SecondOrderPeakNotchFilterParameters& params)
    {
        if (this == &params)
        {
            return *this;
        }
        Q = params.Q;
        gain = params.gain;
        crossOverFrequency = params.crossOverFrequency;
        return *this;
        
    }
    // parameters
    float Q = 0.71f;
    float gain = 0.f;
    float crossOverFrequency = 1000.f;
};


class SecondOrderPeakNotchFilter
{
public:
    SecondOrderPeakNotchFilter() {}
    ~SecondOrderPeakNotchFilter() {}
    void reset();
    // calculates the coefficients of the filter
    void calculateCoefficients();
    // called when setting the sample rate of the filter
    void setSampleRate(const float& sampleRate);
    // set the parameters of the filter
    void setParameters(const SecondOrderPeakNotchFilterParameters& params);
    SecondOrderPeakNotchFilterParameters getParamters()
    {
        return _params;
    }
    // processes an audio sample
    float processAudioSample(const float& xn);
private:
    BiquadDesign _biquad;
    SecondOrderPeakNotchFilterParameters _params;
    float _sampleRate = 44100;
};


enum class BiquadFilterType{NONE, LPF2, LPF4, HPF2, HPF4, BPF2, BPF4};
struct BiquadTempalteFilterParamters
{
    BiquadTempalteFilterParamters(){}
    BiquadTempalteFilterParamters& operator=(const BiquadTempalteFilterParamters& params)
    {
        if (this == &params)
        {
            return *this;
        }
        Q = params.Q;
        
        crossOverFrequency = params.crossOverFrequency;
        filterType = params.filterType;
        return *this;
    }
    // parameters
    float Q = 0.71f;
    float crossOverFrequency = 1000.f;
    BiquadFilterType filterType = BiquadFilterType::LPF2;
};

class BiquadTemplateFilter
{
public:
    BiquadTemplateFilter();
    ~BiquadTemplateFilter() {}
    void reset();
    void setSampleRate(const float& sampleRate);
    // calculate the coefficients of the filter
    void calculateCoefficients();
    void calculateCoefficients(const float& realTimeFc, const float& realTimeQ);
    // set parameters of the filter
    void setParameters(const BiquadTempalteFilterParamters& params);
    // returns the parameters of the filter
    BiquadTempalteFilterParamters getParameters()
    {
        return _params;
    }
    // process audio sample
    float processAudioSample(const float& xn);
    // whether the filter is active
    bool isActive()
    {
        if (_params.filterType == BiquadFilterType::NONE)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    // reset biquad chains
    void resetBiquadChains(const int biquadChainCount);
    
    // smoothing coefficients
    void calculateSmoothingCoeffs();
    // apply smoothing
    float applySmoothing(float& val, float& state, const float& linInc);
private:
    BiquadTempalteFilterParamters _params;
    std::vector<BiquadDesign> _biquadChains;
    float _sampleRate = 44100.f;
    int _biquadChainCount = 0;
    // Smoothing Parameters
    float _smoothingTimeMs = 100.f;
    float _linearIncrementFc = 0.f;
    float _linearIncrementQ = 0.f;
    float _s_Fc = 1000.f;
    float _s_Q = 0.71;
    bool smoothingEnabled = true;
};

class FourthOrderHighPassButterworth
{
public:
    FourthOrderHighPassButterworth() {}
    ~FourthOrderHighPassButterworth() {}
    void reset();
    // calculates the coefficients of the filter
    void calculateCoefficients();
    void setCrossOverFrequency(const float& crossOverFrequency);
    // called when setting the sample rate of the filter
    void setSampleRate(const float& sampleRate);
    // processes an audio sample
    float processAudioSample(const float& xn);
private:
    BiquadDesign _biquadChainOne;
    BiquadDesign _biquadChainTwo;
    float _crossoverFrequency = 1000.f; // 1000 is a just random default value
    float _sampleRate = 44100;
};

class FourthOrderLowPassButterworth
{
public:
    FourthOrderLowPassButterworth() {}
    ~FourthOrderLowPassButterworth() {}
    void reset();
    // calculates the coefficients of the filter
    void calculateCoefficients();
    // called when setting the crossover frequency of the filter
    void setCrossOverFrequency(const float& crossOverFrequency);
    // called when setting the sample rate of the filter
    void setSampleRate(const float& sampleRate);
    // processes an audio sample
    float processAudioSample(const float& xn);
private:
    BiquadDesign _biquadChainOne;
    BiquadDesign _biquadChainTwo;
    float _crossoverFrequency = 1000.f; // 1000 is a just random default value
    float _sampleRate = 44100;
};

struct FourthOrderBandPassFilterParameters
{
    FourthOrderBandPassFilterParameters(){}
    FourthOrderBandPassFilterParameters& operator=(const FourthOrderBandPassFilterParameters& params)
    {
        if (this == &params)
        {
            return *this;
        }
        Q = params.Q;
        crossOverFrequency = params.crossOverFrequency;
        return *this;
        
    }
    // parameters
    float Q = 0.71f;
    float crossOverFrequency = 1000.f;
};

class FourthOrderBandPassFilter
{
public:
    FourthOrderBandPassFilter() {}
    ~FourthOrderBandPassFilter() {}
    void reset();
    // calculates the coefficients of the filter
    void calculateCoefficients();
    // called when setting the crossover frequency of the filter
    void setParameters(const FourthOrderBandPassFilterParameters& params);
    // called when setting the sample rate of the filter
    void setSampleRate(const float& sampleRate);
    // returns the parameters of the filter
    FourthOrderBandPassFilterParameters getParameters()
    {
        return _params;
    }
    // processes an audio sample
    float processAudioSample(const float& xn);
private:
    FourthOrderBandPassFilterParameters _params;
    BiquadDesign _biquadChainOne;
    BiquadDesign _biquadChainTwo;
    float _sampleRate;
};
 
