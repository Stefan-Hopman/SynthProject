/*
  ==============================================================================

    WavetableSynth.cpp
    Created: 13 Oct 2022 3:20:45pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "WavetableSynth.h"
#define ARC4RANDOMMAX 4294967295 // (2^32 - 1)

std::vector<float> WavetableSynth::generateSineWaveTable()
{
    const float PI = std::atanf(1.f) * 4.f; // PI 3.14 variable
    // The length of the sinewave table
    std::vector<float> sineWaveTable = std::vector<float>(_WaveTableLength);
    
    for (auto i = 0; i < _WaveTableLength; ++i)
    {
        sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / _WaveTableLength);
    }

    return sineWaveTable;
}

std::vector<float> WavetableSynth::generateSawtoothWaveTable()
{
    const float moduloInc = 1.f/_WaveTableLength;
    float modulo = 0.f;
    // length of the sawtooth table
    std::vector<float> sawtoothWaveTable = std::vector<float>(_WaveTableLength);
    for(int i = 0; i < _WaveTableLength; i++)
    {
        if (modulo >= 1.0f)
        {
            modulo = modulo - 1.f;
        }
        float trivialSaw = 2.0f * modulo - 1.f;
        modulo = modulo + moduloInc;
        sawtoothWaveTable[i] = trivialSaw;
    }
    return sawtoothWaveTable;
}

std::vector<float> WavetableSynth::generateTriangularWaveTable()
{
    const float moduloInc = 1.f/_WaveTableLength;
    float modulo = 0.f;
    // length of the triangular table
    std::vector<float> triangularWaveTable = std::vector<float>(_WaveTableLength);
    for(int i = 0; i < _WaveTableLength; i++)
    {
        if (modulo >= 1.0f)
        {
            modulo = modulo - 1.f;
        }
        float trivialTriangle = 2.f * fabs(2.f * modulo -1.f) - 1.f;
        modulo = modulo + moduloInc;
        triangularWaveTable[i] = trivialTriangle;
    }
    return triangularWaveTable;
}

std::vector<float> WavetableSynth::generateSquareWaveTable()
{
    const float moduloInc = 1.f/_WaveTableLength;
    float modulo = 0.f;
    // length of the square table
    std::vector<float> squareWaveTable = std::vector<float>(_WaveTableLength);
    for(int i = 0; i < _WaveTableLength; i++)
    {
        if (modulo >= 1.f)
        {
            modulo = modulo - 1.f;
        }
        float trivalSquare = 0.f;
        if (modulo > 0.5f)
        {
            trivalSquare = -1.0f;
        }
        else
        {
            trivalSquare = 1.0f;
        }
        modulo = modulo + moduloInc;
        squareWaveTable[i] = trivalSquare;
    }
    return squareWaveTable;
}

std::vector<float> WavetableSynth::generateWhiteNoiseWaveTable()
{
    std::vector<float> whiteNoiseTable = std::vector<float>(_WaveTableLength);
    for (int i = 0; i < _WaveTableLength; i++)
    {
        // fNoise is 0 -> ARC4RANDOMMAX
        float fNoise = (float)arc4random();
      // normalize and make bipolar
        fNoise = 2.0*(fNoise/ARC4RANDOMMAX) - 1.0;
        whiteNoiseTable[i] = fNoise;
    }
    return whiteNoiseTable;
    
}


void WavetableSynth::initializeOscillators()
{
    // clear the data of the oscillators
    _oscillators.clear();
    // an oscillator for every MIDI note possibilty
    constexpr auto OSCILLATOR_COUNT = 128;
    std::vector<std::vector<float>> waveTables;
    // generates data for sine wave;
    const auto sineWaveTable = generateSineWaveTable();
    const auto sawtoothWaveTable = generateSawtoothWaveTable();
    const auto triangleWaveTable = generateTriangularWaveTable();
    const auto squareWaveTable = generateSquareWaveTable();
    const auto whiteNoiseTable = generateWhiteNoiseWaveTable();
    waveTables.push_back(sineWaveTable);
    waveTables.push_back(sawtoothWaveTable);
    waveTables.push_back(triangleWaveTable);
    waveTables.push_back(squareWaveTable);
    waveTables.push_back(whiteNoiseTable);
    // initiliaze the oscillators
    for (auto i = 0; i < OSCILLATOR_COUNT; ++i)
    {
        _oscillators.emplace_back(waveTables, _sampleRate);
        
    }
}

// called whenevr a new wave type is selected
void WavetableSynth::updateOscillators(WaveType wave)
{
    // an oscillator for every MIDI note possibilty
    constexpr auto OSCILLATOR_COUNT = 128;
    const int waveTableIndex = static_cast<int>(wave);
    for (auto i = 0; i < OSCILLATOR_COUNT; ++i)
    {
        _oscillators[i].setWaveIndex(waveTableIndex);
    }
}

void WavetableSynth::updateOscillatorEnvelopes()
{
    constexpr auto OSCILLATOR_COUNT = 128;
    Stefan::ADSR::Parameters adsrParams;
    adsrParams.attack = _params.attackTime;
    adsrParams.decay = _params.decayTime;
    adsrParams.release = _params.releaseTime;
    adsrParams.sustain = _params.sustainLevel;
    for (auto i = 0; i < OSCILLATOR_COUNT; ++i)
    {
        _oscillators[i].setADSR(adsrParams);
    }
}

// updatate all 128 on a new envelope with a new sustain level
void WavetableSynth::updateOscillatorEnvelopes(const float& sustainLevel)
{
    constexpr auto OSCILLATOR_COUNT = 128;
    Stefan::ADSR::Parameters adsrParams;
    adsrParams.attack = _params.attackTime;
    adsrParams.decay = _params.decayTime;
    adsrParams.release = _params.releaseTime;
    adsrParams.sustain = sustainLevel;
    for (auto i = 0; i < OSCILLATOR_COUNT; ++i)
    {
        _oscillators[i].setADSR(adsrParams);
    }
    
}


void WavetableSynth::prepareToPlay(double &sampleRate)
{
    _sampleRate = sampleRate;
    initializeOscillators(); // puts audio data into the oscillators
    calculateSmoothingCoeffs();
    _s_OscVol = 0.f;
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages)
{
    auto currentSample = 0;
    for (const auto midiMetadata : midiMessages)
    {
        const auto message = midiMetadata.getMessage(); // retrieve the MIDI message
        const auto messagePosition = static_cast<int>(message.getTimeStamp()); // getting the time stamp of that MIDI message
        render(buffer, currentSample, messagePosition); // render the audio samples of
        currentSample = messagePosition;
        handleMidiEvent(message);
    }
    // render the rest of the audio samples
    render(buffer, currentSample, buffer.getNumSamples());
}

float WavetableSynth::halfStepToFreqMultiplier(const int& halfSteps)
{
    if (halfSteps > 0.f)
    {
        return 1.f + (1.f / 12.f) * halfSteps;
    }
    else
    {
        return 1.0f + (0.04166666667f * halfSteps);
    }
    
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiMessage)
{
    // If a key was pressed (“note on”), we convert its number to frequency in Hz and inform the oscillator under that number that it should start playing by setting its frequency.
   
    if (midiMessage.isNoteOn())
    {
    
        const auto oscillatorId = midiMessage.getNoteNumber();
        
        float frequency =  midiNoteNumberToFrequency(oscillatorId + _params.pitchShift);
        activeOscillatorIds.push_back(oscillatorId);
        _oscillators[oscillatorId].setFrequency(frequency, true);
    }
    // If a key was released (“note off”), we stop the oscillator responsible for that key.
    else if (midiMessage.isNoteOff())
    {
        const auto oscillatorId = midiMessage.getNoteNumber();
        // xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
        activeOscillatorIds.erase(std::remove(activeOscillatorIds.begin(), activeOscillatorIds.end(), oscillatorId), activeOscillatorIds.end());
        _oscillators[oscillatorId].noteOff();
    }
    // If an “all notes off” message was issued, we stop all oscillators. Such messages are more likely to be present in MIDI files rather than during live performances
    else if (midiMessage.isAllNotesOff())
    {
        activeOscillatorIds.clear();
        for (auto& oscillator : _oscillators)
        {
            oscillator.stop();
        }
    }
}

void WavetableSynth::setParameters(const WavetableSynth_Parameters& params)
{
    if (this->_params.waveType != params.waveType)
    {
        _params.waveType = params.waveType;
        updateOscillators(_params.waveType);
    }
    if (this->_params.attackTime != params.attackTime || this->_params.decayTime != params.decayTime || this->_params.releaseTime != params.releaseTime || this->_params.sustainLevel != params.sustainLevel)
    {
        _params.attackTime = params.attackTime;
        _params.decayTime = params.decayTime;
        _params.releaseTime = params.releaseTime;
        _params.sustainLevel = params.sustainLevel;
        updateOscillatorEnvelopes();
    }
    if (_params.pitchShift != params.pitchShift)
    {
        for (int i = 0; i < activeOscillatorIds.size() ; i++)
        {
            float frequency = midiNoteNumberToFrequency(activeOscillatorIds[i] + params.pitchShift);
            _oscillators[activeOscillatorIds[i]].setFrequency(frequency, false);
        }
    }
    _params = params;
}

void WavetableSynth::calculateSmoothingCoeffs()
{
    _linearIncrementOscVolume = (0.f - (-60.f)) / (_smoothingTimeMs * 0.001f * _sampleRate);
    _linearIncrementSustain = (1.f - (0.f)) / (_smoothingTimeMs * 0.001f * _sampleRate);
}

float WavetableSynth::midiNoteNumberToFrequency(const int midiNoteNumber)
{
    constexpr float A4_FREQUENCY = 440.f;
    constexpr float A4_NOTE_NUMBER = 69.f;
    constexpr float NOTES_IN_AN_OCTAVE = 12.f;
    return A4_FREQUENCY * std::powf(2, (static_cast<float>(midiNoteNumber) - A4_NOTE_NUMBER) / NOTES_IN_AN_OCTAVE);
}


void WavetableSynth::render(juce::AudioBuffer<float>& buffer, const int& beginSample, const int& endSample)
{
    auto* firstChannel = buffer.getWritePointer(0);
    // check each oscillator if its on and if they need to be rendered.
    // A constant gain because has its own adsr, so apply a dynamic gain can cause clicks
    float onGain = 1.0f;
    if (_params.active == false)
    {
        onGain = 0.f;
    }
        for (auto& oscillator : _oscillators)
        {
            if (oscillator.isPlaying())
            {
                for(int i = beginSample; i < endSample; i++)
                {
                    float realTimeGain = powf(10.f, applySmoothing(_params.gain, _s_OscVol, _linearIncrementOscVolume)/ 20.f);
                    float realTimeSustainLevel = applySmoothing(_params.sustainLevel, _s_Sustain, _linearIncrementSustain);
                    if(realTimeSustainLevel != _params.sustainLevel)
                    {
                        updateOscillatorEnvelopes(realTimeSustainLevel);
                    }
                    buffer.setSample(0, i, buffer.getSample(0, i) + (realTimeGain * onGain * oscillator.applyADSR(oscillator.getSample())));
                }
            }
        }
        // copy the data from first channel to all the other channels
        for (int channel = 1; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            std::copy(firstChannel + beginSample, firstChannel + (buffer.getNumSamples()-1), channelData + beginSample);
        }
        
    
    
}

float WavetableSynth::applySmoothing(float& val, float& state, const float& linInc)
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

 
 
 
