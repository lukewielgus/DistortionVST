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
class DistortionVST3AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistortionVST3AudioProcessorEditor (DistortionVST3AudioProcessor&);
    ~DistortionVST3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // knob for the drive
    std::unique_ptr<Slider> driveKnob = std::make_unique<juce::Slider>("Drive");
    // attachment for the slider
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionVST3AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionVST3AudioProcessorEditor)
};
