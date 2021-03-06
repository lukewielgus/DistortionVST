/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionVST3AudioProcessor::DistortionVST3AudioProcessor()
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
    pState = std::make_unique<AudioProcessorValueTreeState>(*this, nullptr);
    
    pState->createAndAddParameter("drive", "Drive", "Drive", NormalisableRange<float>(0.f, 100.f, 0.1), 0.0, nullptr, nullptr);
    pState->createAndAddParameter("gain", "Output Gain", "Output Gain", NormalisableRange<float>(0.f, 1.f, 0.01), 1.f, nullptr, nullptr);
    pState->state = ValueTree("drive");
    pState->state = ValueTree("gain");
}

DistortionVST3AudioProcessor::~DistortionVST3AudioProcessor()
{
}

//==============================================================================
const juce::String DistortionVST3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionVST3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionVST3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionVST3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionVST3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionVST3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionVST3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionVST3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionVST3AudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionVST3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionVST3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistortionVST3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionVST3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void DistortionVST3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    float driveVal = *pState->getRawParameterValue("drive");
    float gainVal = *pState->getRawParameterValue("gain");
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // grab the clean signal to use for blending with distorted signal later
            float cleanSignal = *channelData;
            if (driveVal < 1)
            {
                *channelData = cleanSignal * gainVal;
            }
            else
            {
                // do the actual distortion
                *channelData *= driveVal;
                // distortion algorithm
                *channelData = ((((2.f / float_Pi) * atan(*channelData)) + cleanSignal) / 2) * gainVal;
            }
            // increment channel data
            ++channelData;
        }
    }
}

//==============================================================================
bool DistortionVST3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionVST3AudioProcessor::createEditor()
{
    return new DistortionVST3AudioProcessorEditor (*this);
}

//==============================================================================
void DistortionVST3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    MemoryOutputStream stream(destData, false);
    pState->state.writeToStream(stream);
}

void DistortionVST3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    ValueTree valTree = ValueTree::readFromData(data, sizeInBytes);
    
    if (valTree.isValid())
    {
        pState->state = valTree;
    }
}

//==============================================================================
AudioProcessorValueTreeState& DistortionVST3AudioProcessor::getProcessorState()
{
    return *pState;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionVST3AudioProcessor();
}
