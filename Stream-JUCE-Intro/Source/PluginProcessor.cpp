/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StreamJUCEIntroAudioProcessor::StreamJUCEIntroAudioProcessor()
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

StreamJUCEIntroAudioProcessor::~StreamJUCEIntroAudioProcessor()
{
}

//==============================================================================
const juce::String StreamJUCEIntroAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StreamJUCEIntroAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StreamJUCEIntroAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StreamJUCEIntroAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StreamJUCEIntroAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StreamJUCEIntroAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StreamJUCEIntroAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StreamJUCEIntroAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StreamJUCEIntroAudioProcessor::getProgramName (int index)
{
    return {};
}

void StreamJUCEIntroAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StreamJUCEIntroAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StreamJUCEIntroAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StreamJUCEIntroAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void StreamJUCEIntroAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // My audio block object
    juce::dsp::AudioBlock<float> block (buffer);
    
    // My DSP block
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        
        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            channelData[sample] *= 4.0;
        }
    }
}

//==============================================================================
bool StreamJUCEIntroAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StreamJUCEIntroAudioProcessor::createEditor()
{
    return new StreamJUCEIntroAudioProcessorEditor (*this);
}

//==============================================================================
void StreamJUCEIntroAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StreamJUCEIntroAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StreamJUCEIntroAudioProcessor();
}
