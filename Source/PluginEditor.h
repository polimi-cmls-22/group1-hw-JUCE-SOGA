/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once // https://en.wikipedia.org/wiki/Pragma_once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

//********************************************************************************************//
// 2) Make the class Listener as base class
//********************************************************************************************//
class DelayLineAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener // le parti public e private ereditano classi diverse
{
public:
    DelayLineAudioProcessorEditor (DelayLineAudioProcessor&); // dichiarazione del costruttore; prende il processor come parametro;
    ~DelayLineAudioProcessorEditor() override; // de-costruttore

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayLineAudioProcessor& audioProcessor;
    
    
    //********************************************************************************************//
    juce::Slider wetSlider;
    juce::Label wetLabel;
    juce::Slider drySlider;
    juce::Label dryLabel;
    juce::Slider sicknessSlider;
    juce::Label sicknessLabel;
    juce::Slider airPollutionSlider;
    juce::Label airPollutionLabel;
    juce::Slider speedSlider;
    juce::Label speedLabel;
    juce::Slider distanceSlider;
    juce::Label distanceLabel;

    juce::Image background;
    juce::Image dude[8];
    juce::Image ship[4];

    int dudeX, dudeY, shipX, shipY; // coordinates for the images
    int dudeIndex; // index of face
    int shipDistance; 
    float shipScale;
    float dudeScale;

    float* t;
    //********************************************************************************************//
    
    //********************************************************************************************//
    void sliderValueChanged(juce::Slider* slider) override;

    void updateDudeIndex(int val);
    void updateDistance(float val);
    void updateShipScale(float val);
    void updateDudeScale(float val);
    //********************************************************************************************//
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayLineAudioProcessorEditor)
};
