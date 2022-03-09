/*
  ==============================================================================

    DemoDial.cpp
    Created: 7 Mar 2022 6:32:56pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void StreamJUCEIntroAudioProcessorEditor::initDial()
{
    dial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    dial.setRange(-24.0, 24.0, 0.1);
    dial.setDoubleClickReturnValue(true, 0.0);
    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.5f));
    dial.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(dial);
}
