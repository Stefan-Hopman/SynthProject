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
                       )
#endif
{
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
    float oscillatorOneGain = apvts.getRawParameterValue("Oscillator One Gain")->load();
    float oscillatorTwoGain = apvts.getRawParameterValue("Oscillator Two Gain")->load();
    float oscillatorThreeGain = apvts.getRawParameterValue("Oscillator Three Gain")->load();
    float oscillatorFourGain = apvts.getRawParameterValue("Oscillator Four Gain")->load();
    bool oscillatorOneOn = apvts.getRawParameterValue("Oscillator One On")->load();
    bool oscillatorTwoOn = apvts.getRawParameterValue("Oscillator Two On")->load();
    bool oscillatorThreeOn = apvts.getRawParameterValue("Oscillator Three On")->load();
    bool oscillatorFourOn = apvts.getRawParameterValue("Oscillator Four On")->load();
    
    //WaveType waveChoice = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type")->load());
    WaveType waveChoiceOne = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator One")->load());
    WaveType waveChoiceTwo = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator Two")->load());
    WaveType waveChoiceThree = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator Three")->load());
    WaveType waveChoiceFour = static_cast<WaveType>(apvts.getRawParameterValue("Wave Type Oscillator Four")->load());
    
    float oscillatorOnePitchShift = apvts.getRawParameterValue("Oscillator One Pitch Shift")->load();
    float oscillatorTwoPitchShift = apvts.getRawParameterValue("Oscillator Two Pitch Shift")->load();
    float oscillatorThreePitchShift = apvts.getRawParameterValue("Oscillator Three Pitch Shift")->load();
    float oscillatorFourPitchShift = apvts.getRawParameterValue("Oscillator Four Pitch Shift")->load();
    
}

juce::AudioProcessorValueTreeState::ParameterLayout ProjectFourSynthAudioProcessor::createParameterLayout()
{
    auto attributes = juce::AudioParameterChoiceAttributes().withLabel ("selected");
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator One Gain", "Oscillator One Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Two Gain", "Oscillator Two Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Three Gain", "Oscillator Three Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Four Gain", "Oscillator Four Gain", juce::NormalisableRange<float>(-60.f, 0.f, 1.f, 1.f), -6.f));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator One On", "Oscillator One On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator Two On", "Oscillator Two On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator Three On", "Oscillator Three On", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Oscillator Four On", "Oscillator Four On", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator One Pitch Shift", "Oscillator One Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Two Pitch Shift", "Oscillator Two Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Three Pitch Shift", "Oscillator Three Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Oscillator Four Pitch Shift", "Oscillator Four Pitch Shift", juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator One", "Wave Type Oscillator One", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square"}, 0, attributes));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator Two", "Wave Type Oscillator Two", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square"}, 0, attributes));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator Three", "Wave Type Oscillator Three", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square"}, 0, attributes));
    layout.add(std::make_unique<juce::AudioParameterChoice>("Wave Type Oscillator Four", "Wave Type Oscillator Four", juce::StringArray {"Sine", "Sawtooth", "Triangle", "Square"}, 0, attributes));

    return layout;
   
}

//==============================================================================
void ProjectFourSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.prepareToPlay(sampleRate);
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
    synth.processBlock(buffer, midiMessages);
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
