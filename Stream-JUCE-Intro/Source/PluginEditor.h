/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class StreamJUCEIntroAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StreamJUCEIntroAudioProcessorEditor (StreamJUCEIntroAudioProcessor&);
    ~StreamJUCEIntroAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StreamJUCEIntroAudioProcessor& audioProcessor;
    
    void initWindow();
    
    viator_gui::Dial dial;
    void initDial();
    
    viator_gui::PushButton button;
    viator_gui::Label dialLabel;
    viator_gui::Border border;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dialAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StreamJUCEIntroAudioProcessorEditor)
};
