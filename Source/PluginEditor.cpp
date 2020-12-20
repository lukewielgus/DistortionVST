/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionVST3AudioProcessorEditor::DistortionVST3AudioProcessorEditor (DistortionVST3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(*driveKnob);
    driveKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    driveKnob->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 100);
    driveKnob->setTextBoxIsEditable(false);
    //driveKnob->showTextBox();
    
    driveKnob->setRange(0.0, 11.0);
    
    driveAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.getProcessorState(), "drive", *driveKnob);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 300);
}

DistortionVST3AudioProcessorEditor::~DistortionVST3AudioProcessorEditor()
{
}

//==============================================================================
void DistortionVST3AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    g.drawText("DRIVE", getWidth() / 2, getHeight() / 2, 100, 50, Justification::centred);
    
    driveKnob->showTextBox();
    driveKnob->TextBoxBelow = "Drive";
}

void DistortionVST3AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    driveKnob->setBounds(getWidth() / 2, getHeight() / 2, 100, 100);
}
