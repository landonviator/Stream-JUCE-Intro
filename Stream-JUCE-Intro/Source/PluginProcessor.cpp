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
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("gain", this);
    treeState.addParameterListener("phase", this);
}

StreamJUCEIntroAudioProcessor::~StreamJUCEIntroAudioProcessor()
{
    treeState.removeParameterListener("gain", this);
    treeState.removeParameterListener("phase", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout StreamJUCEIntroAudioProcessor::createParameterLayout()
{
  std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
  juce::StringArray choices = {"Compressor", "EQ", "Reverb"};

  auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Gain", -24.0, 24.0, 0.0);
  auto pPhase = std::make_unique<juce::AudioParameterBool>("phase", "Phase", false);
  auto pChoice = std::make_unique<juce::AudioParameterChoice>("choice", "Choice", choices, 0);
    
  params.push_back(std::move(pGain));
  params.push_back(std::move(pPhase));
  params.push_back(std::move(pChoice));
    
  return { params.begin(), params.end() };
}

void StreamJUCEIntroAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "gain")
    {
        rawGain = juce::Decibels::decibelsToGain(newValue);
    }
    
    if (parameterID == "phase")
    {
        phase = newValue;
    }
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
    phase = *treeState.getRawParameterValue("phase");
    rawGain = juce::Decibels::decibelsToGain(static_cast<float>(*treeState.getRawParameterValue("gain")));
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
            if (phase)
            {
                channelData[sample] *= rawGain * -1.0;
            }
            
            else
            {
                channelData[sample] *= rawGain;
            }
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
    //return new StreamJUCEIntroAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void StreamJUCEIntroAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void StreamJUCEIntroAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    
    if (tree.isValid())
    {
        treeState.state = tree;
        phase = *treeState.getRawParameterValue("phase");
        rawGain = juce::Decibels::decibelsToGain(static_cast<float>(*treeState.getRawParameterValue("gain")));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StreamJUCEIntroAudioProcessor();
}
