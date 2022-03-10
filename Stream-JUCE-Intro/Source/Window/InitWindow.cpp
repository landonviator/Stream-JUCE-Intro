/*
  ==============================================================================

    InitWindow.cpp
    Created: 7 Mar 2022 6:33:18pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "../PluginEditor.h"

void StreamJUCEIntroAudioProcessorEditor::initWindow()
{
    setResizable(true, true);
    setResizeLimits(600, 600, 900, 900);
    getConstrainer()->setFixedAspectRatio(1.0);
}
