/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
*/
class DelayLineAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DelayLineAudioProcessor();
    ~DelayLineAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //********************************************************************************************//
    void set_wet(float val);
    void set_dry(float val);
    void set_fb(float val);
    void set_ds(int val);
    void set_speed(float val);
    void set_lfoAmp(float val);
    //********************************************************************************************//
    float wet;
    float dry;
    float fb;
    float speed;
    float lfoAmp;
    int ds;
    float t; // not the actual time

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayLineAudioProcessor)
    
    //********************************************************************************************//
    juce::AudioSampleBuffer dbuf;
    int dbufSize;
    int dw;
    int dr;
    double dr_remainder; // used to interpolate
    //********************************************************************************************//
};
