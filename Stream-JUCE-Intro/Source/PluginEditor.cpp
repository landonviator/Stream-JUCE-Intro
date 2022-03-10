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
, dial(" dB", -60.0, 24.0, 0.01, 0.0)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    dialAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", dial);
    addAndMakeVisible(dial);
    dial.setDialStyle(viator_gui::Dial::DialStyle::kFullDial);
    dial.forceShadow();
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "phase", button);
    addAndMakeVisible(button);
    button.setButtonText("Phase");
    
    addAndMakeVisible(dialLabel);
    dialLabel.setText("Gain", juce::dontSendNotification);
    dialLabel.attachToComponent(&dial, false);
    
    addAndMakeVisible(border);
    border.setText("Utility");
    
    initWindow();
    setSize (600, 600);
}

StreamJUCEIntroAudioProcessorEditor::~StreamJUCEIntroAudioProcessorEditor()
{
}

//==============================================================================
void StreamJUCEIntroAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromFloatRGBA(0.18f, 0.20f, 0.24f, 1.0), getHeight() * 0.1, juce::Colour::fromFloatRGBA(0.12f, 0.14f, 0.18f, 1.0), getHeight() * 0.9));
    g.fillRect(background);
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
    border.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth() * 0.95, getWidth() * 0.95));
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
