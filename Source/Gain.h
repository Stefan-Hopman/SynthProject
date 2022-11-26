

/*
  ==============================================================================

    Gain.h
    Created: 21 Sep 2022 3:02:41pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once

class Gain
{
public:
    Gain() {}
    ~Gain() {}
    // reset the filter states
    void reset();
    // process an audio sample
    float processAudioSample(const float& xn);
    // set the gain value for the gain plugin
    void setGainValue(const float& gainVal);
    // set the sample rate of the object
    void setSampleRate(const float &sampleRate);
    // calculate the coefficients of the linear increment
    // get the value of the gain
    float getGainValue();
    // calculate the coefficients of the linear increment
    void calculateSmoothingCoeffs();
    

private:
    float _linInc = 0.f;
    float _sampleRate = 44100.f;
    float _s_One = 0.f;
    float _gainVal_dB = 0.f;
    float _smoothingTimeMs = 20.f;
    
};

