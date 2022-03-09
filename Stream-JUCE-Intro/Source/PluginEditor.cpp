/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StreamJUCEIntroAudioProcessorEditor::StreamJUCEIntroAudioProcessorEditor (StreamJUCEIntroAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    //initDial();
    dialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", dial);
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    dial.setRange(-24.0, 24.0, 0.1);
    dial.setDoubleClickReturnValue(true, 0.0);
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    dial.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(dial);
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "phase", button);
    addAndMakeVisible(button);
    button.setButtonText("Phase");
    button.setClickingTogglesState(true);
    button.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    button.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::dimgrey);
    button.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);
    button.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::black);
    
    addAndMakeVisible(dialLabel);
    dialLabel.setText("Gain", juce::dontSendNotification);
    dialLabel.attachToComponent(&dial, false);
    dialLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(border);
    border.setText("Utility");
    
    initWindow();
    setSize (300, 300);
}

StreamJUCEIntroAudioProcessorEditor::~StreamJUCEIntroAudioProcessorEditor()
{
}

//==============================================================================
void StreamJUCEIntroAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black.brighter(0.1));
}

void StreamJUCEIntroAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.17;
    auto topMargin = getHeight() * 0.15;
    auto dialSize = getWidth() * 0.64;
    auto buttonWidth = dialSize * 0.33;
    auto buttonHeight = buttonWidth * 0.5;
    
    dial.setBounds(leftMargin, topMargin, dialSize, dialSize);
    button.setBounds(dial.getX() + dial.getWidth() * 0.33, dial.getY() + dial.getHeight(), buttonWidth, buttonHeight);
    border.setBounds(leftMargin, topMargin * 0.25, dialSize, dial.getY() + dial.getHeight() + button.getHeight() + 10);
}

void StreamJUCEIntroAudioProcessorEditor::initDial()
{
    addAndMakeVisible(dial);
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    dial.setRange(-24.0, 24.0, 0.1);
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    dial.setDoubleClickReturnValue(true, 0.0);
}
