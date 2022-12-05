/*
  ==============================================================================

    Reverb.h
    Created: 30 Sep 2022 9:31:24pm
    Author:  Nathan Erthal

  ==============================================================================
*/

#pragma once
 
#include <iostream>
#include <cmath>
#include <vector>

#include "DelayLine.h"
#include "Interpolatorinator.h"
#include "LPF.h"
#include "HPF.h"
#include "APF.h"

#define NUM_CHANNELS 2
#define DELAY_LINES_PER_CHANNEL 8
#define APFs_PER_CHANNEL 3
 
using namespace std;
 
class Reverb {
public:
 
    Reverb() = default;
    ~Reverb() = default;
 
    Reverb(const Reverb& reverb) = default;
    
    //initialize each delayline, filters and lfo
    void setMaxDelay(const double sampleRate, const int maxDelay_ms)
    {
        this->sampleRate=sampleRate;

        for (int delay = 0; delay < DELAY_LINES_PER_CHANNEL; delay++)
            for (int channel = 0; channel<NUM_CHANNELS; channel++)
            {
                delays[delay][channel].setMaxDelay(sampleRate, maxDelay_ms);
            }
        
        for (int channel = 0; channel<NUM_CHANNELS; channel++)
        {
            allPass[0][channel].setup(sampleRate, 347);
            allPass[1][channel].setup(sampleRate, 113);
            allPass[2][channel].setup(sampleRate, 37);
        }
        
        updateLPFs(15000, sampleRate);
        updateHPFs(100, sampleRate);
        
        loadFeedbackMatrix();
    }
    
    void reset(float delay_ms)
    {
        clearBuffer();
        
        this->delay_ms = delay_ms;
        delaySmoother.reset(18000, delay_ms, 0.1);
        
        warpDelayValues(delay_ms);
        
        wetSmoother.reset(sampleRate, 0.1);
    }
    
    void clearBuffer()
    {
        for (int delay = 0; delay < DELAY_LINES_PER_CHANNEL; delay++)
            for (int channel = 0; channel<NUM_CHANNELS; channel++)
                delays[delay][channel].clearBuffer();
        
        for (int apf = 0; apf < APFs_PER_CHANNEL; apf ++)
            for (int channel = 0; channel<NUM_CHANNELS; channel++)
                allPass[apf][channel].clearBuffer();
    }
    
    void setParameters(float delay_ms, float densityAmount, float wet_db, int algorithm)
    {
        this->densityAmount = densityAmount;//*0.44;
        this->algorithm = algorithm;
        
        if (this->delay_ms != delay_ms)
        {
            delaySmoother.setTargetValue(delay_ms);
            this->delay_ms = delaySmoother.getNextValue();
            
            warpDelayValues(this->delay_ms);
        }
        
        if (this->wet_db != wet_db)
        {
            wetSmoother.setTargetValue(wet_db);
            this->wet_db = wetSmoother.getNextValue();
            this->wetVolume = dbToLinear(this->wet_db);
        }
        
    }
    
    
    float processAudioSample(float sample, int channel)
    {
        wetOut = 0;
        
        // all-pass filtering
        for (int i = 0; i<APFs_PER_CHANNEL; i++)
            sample = allPass[i][channel].processAudioSample(sample, 0.7*densityAmount); //density controls fb coeff for APF's
        
        
        for (int delayLine = 0; delayLine < DELAY_LINES_PER_CHANNEL; delayLine++)
        {
            // only have output from 1 delayline so signal isnt multiplied
            if (delay_ms == 0)
                if (delayLine !=0) sample = 0;
            
            // update the delaytime param
            float delayValue = delayValues[delayLine][channel];
            delays[delayLine][channel].setParameters(delayValue, algorithm);

            // feedback: set the feedback sample for this delayline depending on the feedback coefficient matrix
            feedbackSample=0;
            for (int i = 0; i < DELAY_LINES_PER_CHANNEL; i++)
                for (int j = 0; j < NUM_CHANNELS; j++)
                    feedbackSample += delays[i][j].getFeedbackSample() * feedbackMatrix[i][j][delayLine][channel];
            
            // add feedback + density to input
            float input = sample + feedbackSample*0.44; // feedback higher than 44% becomes unstable
            
            // filter input
            input = lowPass[delayLine][channel].processAudioSample(input);
            input = highPass[delayLine][channel].processAudioSample(input);
            
            // process input for each delayline and sum to output
            wetOut += delays[delayLine][channel].processAudioSample(input);
        }
        
        return wetOut * wetVolume;
    }
    
 
private:
    
    void updateLPFs(int freq, int sampleRate){
        for (int delay = 0; delay < DELAY_LINES_PER_CHANNEL; delay++)
            for (int channel = 0; channel<2; channel++)
                lowPass[delay][channel].updateCoeffs(freq, sampleRate);
    }
    
    void updateHPFs(int freq, int sampleRate){
        for (int delay = 0; delay < DELAY_LINES_PER_CHANNEL; delay++)
            for (int channel = 0; channel<NUM_CHANNELS; channel++)
                highPass[delay][channel].updateCoeffs(freq, sampleRate);
    }
    
    void warpDelayValues(float delayValue) {
        
        float newDelayValue;
        
        for (int delayLine = 0; delayLine < DELAY_LINES_PER_CHANNEL; delayLine++) {
            for (int channel = 0; channel < NUM_CHANNELS; channel++) {
                
                // first delayline will always have time set to delayValue
                if (channel == 0 && delayLine == 0)
                    newDelayValue = delayValue;
                
                else // make the following delay value less than the previous by subtracting a fraction of 1/2.83 from the previous
                    newDelayValue = newDelayValue - (newDelayValue/2.83);
                // 2.83 was picked after analyzing supermassive's spread in delaylines.
                
                delayValues[delayLine][channel] = newDelayValue;
            }
        }
    }
    
    float dbToLinear(float val)
    {
        return pow(10.0, (val / 20.0));
    }
    
    DelayLine delays[DELAY_LINES_PER_CHANNEL][NUM_CHANNELS];
    LPF lowPass[DELAY_LINES_PER_CHANNEL][NUM_CHANNELS];
    HPF highPass[DELAY_LINES_PER_CHANNEL][NUM_CHANNELS];
    APF allPass[APFs_PER_CHANNEL][NUM_CHANNELS];
    Interpolatorinator delaySmoother;
    juce::LinearSmoothedValue<float> wetSmoother;
    float wetOut;
    float densityAmount;
    float delayValues[DELAY_LINES_PER_CHANNEL][NUM_CHANNELS];
    float delay_ms;
    int sampleRate;
    float wetVolume;
    float wet_db;
    float feedbackSample;
    int algorithm;
    float feedbackMatrix[DELAY_LINES_PER_CHANNEL][NUM_CHANNELS][DELAY_LINES_PER_CHANNEL][NUM_CHANNELS];
    
    
    void loadFeedbackMatrix(){
        
        for (int fdelay = 0; fdelay < DELAY_LINES_PER_CHANNEL; fdelay++)
            for (int fchannel = 0; fchannel<NUM_CHANNELS; fchannel++)
                for (int tdelay = 0; tdelay < DELAY_LINES_PER_CHANNEL; tdelay++)
                    for (int tchannel = 0; tchannel<NUM_CHANNELS; tchannel++)
                    {
                        feedbackMatrix[fdelay][fchannel][tdelay][tchannel]=0;
                    }
        // for 16 delaylines
        feedbackMatrix[2][1][0][0]=1;
        feedbackMatrix[3][0][0][0]=1;
        feedbackMatrix[4][1][0][0]=1;
        feedbackMatrix[5][0][0][0]=1;
        
        feedbackMatrix[2][0][0][1]=-1;
        feedbackMatrix[3][1][0][1]=-1;
        feedbackMatrix[4][0][0][1]=-1;
        feedbackMatrix[5][1][0][1]=-1;
        
        feedbackMatrix[2][0][1][0]=1;
        feedbackMatrix[3][1][1][0]=-1;
        feedbackMatrix[4][0][1][0]=1;
        feedbackMatrix[5][1][1][0]=-1;
        
        feedbackMatrix[2][1][1][1]=1;
        feedbackMatrix[3][0][1][1]=-1;
        feedbackMatrix[4][1][1][1]=1;
        feedbackMatrix[5][0][1][1]=-1;
        
        feedbackMatrix[0][1][2][0]=-1;
        feedbackMatrix[1][0][2][0]=-1;
        feedbackMatrix[6][1][2][0]=-1;
        feedbackMatrix[7][0][2][0]=-1;
        
        feedbackMatrix[0][0][2][1]=1;
        feedbackMatrix[1][1][2][1]=1;
        feedbackMatrix[6][0][2][1]=1;
        feedbackMatrix[7][1][2][1]=1;
        
        feedbackMatrix[0][0][3][0]=-1;
        feedbackMatrix[1][1][3][0]=1;
        feedbackMatrix[6][0][3][0]=-1;
        feedbackMatrix[7][1][3][0]=1;
        
        feedbackMatrix[0][1][3][1]=-1;
        feedbackMatrix[1][0][3][1]=1;
        feedbackMatrix[6][1][3][1]=-1;
        feedbackMatrix[7][0][3][1]=1;
        
        feedbackMatrix[0][1][4][0]=1;
        feedbackMatrix[1][0][4][0]=1;
        feedbackMatrix[6][1][4][0]=-1;
        feedbackMatrix[7][0][4][0]=-1;
        
        feedbackMatrix[0][0][4][1]=-1;
        feedbackMatrix[1][1][4][1]=-1;
        feedbackMatrix[6][0][4][1]=1;
        feedbackMatrix[7][1][4][1]=1;
        
        feedbackMatrix[0][0][5][0]=1;
        feedbackMatrix[1][1][5][0]=-1;
        feedbackMatrix[6][0][5][0]=-1;
        feedbackMatrix[7][1][5][0]=1;
        
        feedbackMatrix[0][1][5][1]=1;
        feedbackMatrix[1][0][5][1]=-1;
        feedbackMatrix[6][1][5][1]=-1;
        feedbackMatrix[7][0][5][1]=1;
        
        feedbackMatrix[2][1][6][0]=1;
        feedbackMatrix[3][0][6][0]=1;
        feedbackMatrix[4][1][6][0]=-1;
        feedbackMatrix[5][0][6][0]=-1;
        
        feedbackMatrix[2][0][6][1]=-1;
        feedbackMatrix[3][1][6][1]=-1;
        feedbackMatrix[4][0][6][1]=1;
        feedbackMatrix[5][1][6][1]=1;
        
        feedbackMatrix[2][0][7][0]=1;
        feedbackMatrix[3][1][7][0]=-1;
        feedbackMatrix[4][0][7][0]=-1;
        feedbackMatrix[5][1][7][0]=1;
        
        feedbackMatrix[2][1][7][1]=1;
        feedbackMatrix[3][0][7][1]=-1;
        feedbackMatrix[4][1][7][1]=-1;
        feedbackMatrix[5][0][7][1]=1;
    }
};
    


