/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProjectFourSynthAudioProcessor::ProjectFourSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    apvts(*this, nullptr, ProjectInfo::projectName, createParameterLayout())
    {
        apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
        apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);
        presetManager = std::make_unique<Service::PresetManager>(apvts);

    }


ProjectFourSynthAudioProcessor::~ProjectFourSynthAudioProcessor()
{
}

//==============================================================================
const juce::String ProjectFourSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ProjectFourSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ProjectFourSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ProjectFourSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ProjectFourSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ProjectFourSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ProjectFourSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ProjectFourSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ProjectFourSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void ProjectFourSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void ProjectFourSynthAudioProcessor::updateParameters()
{
    
    AdditiveWavetableSynth_Parameters waveTableSynthParams = additiveWaveTableSynth.getParameters();
    
    waveTableSynthParams.gains[0] = apvts.getRawParameterValue("Oscillator One Gain")->load();
    waveTableSynthParams.gains[1] = apvts.getRawParameterValue("Oscillator Two Gain")->load();
    waveTableSynthParams.gains[2] = apvts.getRawParameterValue("Oscillator Three Gain")->load();
    waveTableSynthParams.gains[3] = apvts.getRawParameterValue("Oscillator Four Gain")->load();
    
    
    waveTableSynthParams.activeStates[0] = apvts.getRawParameterValue("Oscillator One On")->load();
    waveTableSynthParams.activeStates[1] = apvts.getRawParameterValue("Oscillator Two On")->load();
    waveTableSynthParams.activeStates[2] = apvts.getRawParameterValue("Oscillator Three On")->load();
    waveTableSynthParams.activeStates[3] = apvts.getRawParameterValue("Oscillator Four On")->load();
    
    waveTableSynthParams.waveTypes[0] = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator One")->load());
    waveTableSynthParams.waveTypes[1] = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator Two")->load());
    waveTableSynthParams.waveTypes[2] = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator Three")->load());
    waveTableSynthParams.waveTypes[3] = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator Four")->load());
    
    waveTableSynthParams.attackTime = apvts.getRawParameterValue("Attack Time")->load();
    waveTableSynthParams.decayTime = apvts.getRawParameterValue("Decay Time")->load();
    waveTableSynthParams.sustainLevel = apvts.getRawParameterValue("Sustain Level")->load();
    waveTableSynthParams.releaseTime = apvts.getRawParameterValue("Release Time")->load();
    
    waveTableSynthParams.pitchShift[0] = apvts.getRawParameterValue("Oscillator One Pitch Shift")->load();
    waveTableSynthParams.pitchShift[1] = apvts.getRawParameterValue("Oscillator Two Pitch Shift")->load();
    waveTableSynthParams.pitchShift[2] = apvts.getRawParameterValue("Oscillator Three Pitch Shift")->load();
    waveTableSynthParams.pitchShift[3] = apvts.getRawParameterValue("Oscillator Four Pitch Shift")->load();
    
    additiveWaveTableSynth.setParameters(waveTableSynthParams);
    ModulationParameters modulationParameters = modulationFx.getParameters();
    modulationParameters.algorithm = static_cast<modulationAlgorithm>(apvts.getRawParameterValue("Modulation Type")->load());
    modulationParameters.feedback_Pct = apvts.getRawParameterValue("Feedback")->load();
    modulationParameters.lfoRate_Hz = apvts.getRawParameterValue("LFO Rate")->load();
    modulationParameters.lfoDepth_Pct = apvts.getRawParameterValue("Intensity")->load();
    
    modulationFx.setParameters(modulationParameters);
    
    // Distorsion parameters
    Distorsion_Parameters distortionParams = distorsionFx.getParametes();
    distortionParams.algorithm = static_cast<distorsionAlgorithm>(apvts.getRawParameterValue("Distorsion Type")->load());
    distortionParams.drive = apvts.getRawParameterValue("Drive")->load();
    distortionParams.mixPct = apvts.getRawParameterValue("MixPct")->load();
    distorsionFx.setParameters(distortionParams);
    
    // Filter Parameters
    BiquadTempalteFilterParamters filterParameters = synthFilter.getParameters();
    filterParameters.Q = apvts.getRawParameterValue("Q Value")->load();
    filterParameters.crossOverFrequency = apvts.getRawParameterValue("Cross Over Frequency")->load();
    filterParameters.filterType = static_cast<BiquadFilterType>(apvts.getRawParameterValue("Filter Types")->load());
    synthFilter.setParameters(filterParameters);
    
    
    
}


juce::AudioProcessorValueTreeState::ParameterLayout ProjectFourSynthAudioProcessor::createParameterLayout()
{
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator One Gain", "Oscillator One Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.25f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Two Gain", "Oscillator Two Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.25f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Three Gain", "Oscillator Three Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.25f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Four Gain", "Oscillator Four Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.25f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator One On", "Oscillator One On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator Two On", "Oscillator Two On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator Three On", "Oscillator Three On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator Four On", "Oscillator Four On", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator One Pitch Shift", "Oscillator One Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Two Pitch Shift", "Oscillator Two Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Three Pitch Shift", "Oscillator Three Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Four Pitch Shift", "Oscillator Four Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator One", "Wave Type Oscillator One", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square", "White Noise"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator Two", "Wave Type Oscillator Two", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square", "White Noise"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator Three", "Wave Type Oscillator Three", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square", "White Noise"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator Four", "Wave Type Oscillator Four", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square", "White Noise"}, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Attack Time", "Attack", juce::NormalisableRange<float>(0.0f, 1.f, 0.01f, 0.7f), 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Decay Time", "Decay Time", juce::NormalisableRange<float>(0.0f, 1.f, 0.01f, 0.7f), 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Sustain Level", "Sustain Level", juce::NormalisableRange<float>(0.0f, 1.f, 0.01f, 1.0f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Release Time", "Release Time", juce::NormalisableRange<float>(0.0f, 1.f, 0.01f, 0.7f), 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO Rate", "LFO Rate", juce::NormalisableRange<float>(0.1f, 20.f, 0.1f, 0.65f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Intensity", "Intensity", juce::NormalisableRange<float>(0.0f, 100.f, 1.f, 1.f), 50.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Feedback", "Feedback", juce::NormalisableRange<float>(0.0f, 100.f, 1.f, 1.f), 50.0f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Modulation Type", "Modulation Type", juce::StringArray {"None", "Chorus", "Flanger", "Vibrato"}, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Distorsion Type", "Distorsion Type", juce::StringArray {"None", "Hyperbolic Tangent", "Arc Tangent", "Fuzz"}, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Drive", "Drive", juce::NormalisableRange<float>(1.0f, 50.f, 0.5f, 1.f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MixPct", "MixPcy", juce::NormalisableRange<float>(0.0f, 100.f, 1.f, 1.f), 50.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Q Value", "Q Value", juce::NormalisableRange<float>(0.1f, 9.8f, 0.1, 0.45f), 0.71f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Cross Over Frequency", "Cross Over Frequency", juce::NormalisableRange<float>(10.f, 20000.f, 1.f, 0.33f), 1000.f));
    
    layout.add(std::make_unique<juce::AudioParameterChoice>("Filter Types", "Filter Types", juce::StringArray {"None", "LPF2", "LPF4", "HPF2", "HPF4", "BPF2", "BPF4"}, 0));
    
    // Reverb parameters
    auto normRangeDelay = juce::NormalisableRange<float>(0,MAX_DELAY_MS);
    normRangeDelay.setSkewForCentre(500);
    auto normRangeVolume = juce::NormalisableRange<float>(-120,0);
    normRangeVolume.setSkewForCentre(-18);
    
    juce::StringArray algorithmList = {"Default","Reverse","Sped-up","Slowed"};
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("wet", "Wet",  normRangeVolume, -14));
    layout.add(std::make_unique<juce::AudioParameterFloat>("delay", "Delay",  normRangeDelay, 500));
    layout.add(std::make_unique<juce::AudioParameterFloat>("density", "Density",  0, 100, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("algorithm", "Algorithm", algorithmList, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("Reverb Active", "Reverb Active", false));

    return layout;
   
}

//==============================================================================
void ProjectFourSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    additiveWaveTableSynth.prepareToPlay(sampleRate);
    modulationFx.reset(sampleRate);
    synthFilter.setSampleRate(static_cast<float>(sampleRate));
    synthFilter.reset();
    distorsionFx.reset(sampleRate);
    reverbFx.setMaxDelay(sampleRate, MAX_DELAY_MS);
    float delayValue = apvts.getRawParameterValue ("delay")->load();
    reverbFx.reset(delayValue);
}

// gets called whenever starting up the audio again
void ProjectFourSynthAudioProcessor::reset()
{
    float delayValue = apvts.getRawParameterValue ("delay")->load();
    reverbFx.reset(delayValue);
}

void ProjectFourSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ProjectFourSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ProjectFourSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    updateParameters();
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    additiveWaveTableSynth.processBlock(buffer, midiMessages);
    // Apply Modulation Effect
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        buffer.setSample(0, i, modulationFx.processAudioSample(buffer.getSample(0, i)));
    }
    // Apply Distorsion Effect
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        buffer.setSample(0, i, distorsionFx.processAudioSample(buffer.getSample(0, i)));
    }
    // Apply Reverb Effect
    if (apvts.getRawParameterValue("Reverb Active")->load() == true)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            // Reverb Parameters
            reverbFx.setParameters(apvts.getRawParameterValue ("delay")->load(),
                                   apvts.getRawParameterValue ("density")->load()/100,
                                   apvts.getRawParameterValue ("wet")->load(),
                                   apvts.getRawParameterValue ("algorithm")->load());
            buffer.setSample(0, i, reverbFx.processAudioSample(buffer.getSample(0, i), 0));
        }
    }
    
    
    
    // Apply Filtering
    if (synthFilter.isActive() == true)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            buffer.setSample(0, i, synthFilter.processAudioSample(buffer.getSample(0, i)));
        }
        
    }
    auto* firstChannel = buffer.getWritePointer(0);
    for (int channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        std::copy(firstChannel, firstChannel + buffer.getNumSamples(), channelData);
    }
}

//==============================================================================
bool ProjectFourSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ProjectFourSynthAudioProcessor::createEditor()
{
    return new ProjectFourSynthAudioProcessorEditor (*this);
}

//==============================================================================
void ProjectFourSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ProjectFourSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProjectFourSynthAudioProcessor();
}
