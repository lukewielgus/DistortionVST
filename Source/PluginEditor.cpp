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
    // Drive knob
    addAndMakeVisible(*driveKnob);
    driveKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    driveKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    //driveKnob->setTextBoxIsEditable(false);
    //driveKnob->showTextBox();
    
    //driveKnob->setRange(0.0, 11.0);
    
    driveAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.getProcessorState(), "drive", *driveKnob);
    
    // Gain slider
    addAndMakeVisible(*gainSlider);
    gainSlider->setSliderStyle(Slider::LinearHorizontal);
    gainSlider->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    
    gainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(p.getProcessorState(), "gain", *gainSlider);
    
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
    g.drawText("Axel Dunn Distortion", 0, 0, 300, 50, Justification::centred);
    g.drawText("DRIVE", getLocalBounds(), Justification::centred, 1);
    g.drawText("Output Gain", 25, 200, 100, 100, Justification::left);
    //driveKnob->showTextBox();
    //driveKnob->TextBoxBelow = "Drive";
}

void DistortionVST3AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    driveKnob->setBounds(100, 100, 100, 100);
    gainSlider->setBounds(100, 200, 100, 100);
}
