#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayLineAudioProcessorEditor::DelayLineAudioProcessorEditor (DelayLineAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(400, 400); // window size

    // CREATE SLIDERS AND SET THE STYLE
    wetSlider.setRange (0.0, 1.0);
    wetSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    wetSlider.setValue(p.wet);
    wetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    wetSlider.addListener(this);
    wetSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffe94435));
    wetSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff212121));
    wetSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0x00000000));
    wetLabel.setText ("wet", juce::dontSendNotification);
    wetLabel.setColour(juce::Label::textColourId, juce::Colour(0xfff9ca83));
    wetLabel.setFont(juce::Font("Monospace", 13.0f, juce::Font::plain));
    addAndMakeVisible (wetSlider);
    addAndMakeVisible (wetLabel);
    
    drySlider.setRange (0.0, 1.0);
    drySlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    drySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    drySlider.setValue(p.dry);
    drySlider.addListener(this);
    drySlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffe94435));
    drySlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff212121));
    drySlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0x00000000));
    dryLabel.setText ("dry", juce::dontSendNotification);
    dryLabel.setColour(juce::Label::textColourId, juce::Colour(0xfff9ca83));
    dryLabel.setFont(juce::Font("Monospace", 13.0f, juce::Font::plain));
    addAndMakeVisible (drySlider);
    addAndMakeVisible (dryLabel);
    
    sicknessSlider.setRange (10, 750, 1);
    sicknessSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    sicknessSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    sicknessSlider.setSkewFactorFromMidPoint(180);
    sicknessSlider.setValue(p.ds);
    sicknessSlider.addListener(this);
    sicknessSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff5467f5));
    sicknessSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff212121));
    sicknessSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0x00000000));
    sicknessLabel.setText ("sickness", juce::dontSendNotification);
    sicknessLabel.setColour(juce::Label::textColourId, juce::Colour(0xffe96d5c));
    sicknessLabel.setFont(juce::Font("Monospace", 13.0f, juce::Font::plain));
    addAndMakeVisible (sicknessSlider);
    addAndMakeVisible (sicknessLabel);

    airPollutionSlider.setRange(0.0, 0.8);
    airPollutionSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    airPollutionSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    airPollutionSlider.setValue(p.fb);
    airPollutionSlider.addListener(this);
    airPollutionSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff5467f5));
    airPollutionSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff212121));
    airPollutionSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0x00000000));
    airPollutionLabel.setText("air pollution", juce::dontSendNotification);
    airPollutionLabel.setColour(juce::Label::textColourId, juce::Colour(0xffe96d5c));
    airPollutionLabel.setFont(juce::Font("Monospace", 13.0f, juce::Font::plain));
    addAndMakeVisible(airPollutionSlider);
    addAndMakeVisible(airPollutionLabel);

    speedSlider.setRange(0, 0.001f);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    speedSlider.setSkewFactorFromMidPoint(0.0003000f);
    speedSlider.setValue(p.speed);
    speedSlider.addListener(this);
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff5467f5));
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff212121));
    speedSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0x00000000));
    speedLabel.setText("speed", juce::dontSendNotification);
    speedLabel.setColour(juce::Label::textColourId, juce::Colour(0xffe96d5c));
    speedLabel.setFont(juce::Font("Monospace", 13.0f, juce::Font::plain));
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);

    distanceSlider.setRange(0, 0.6f);
    distanceSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    distanceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    distanceSlider.setValue(p.lfoAmp);
    distanceSlider.addListener(this);
    distanceSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff5467f5));
    distanceSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff212121));
    distanceSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(0x00000000));
    distanceLabel.setText("distance", juce::dontSendNotification);
    distanceLabel.setColour(juce::Label::textColourId, juce::Colour(0xffe96d5c));
    distanceLabel.setFont(juce::Font("Monospace", 13.0f, juce::Font::plain));
    addAndMakeVisible(distanceSlider);
    addAndMakeVisible(distanceLabel);
    
    // UGLY CODE TO LOAD IMAGES 
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    dude[0] = juce::ImageCache::getFromMemory(BinaryData::p1_png, BinaryData::p1_pngSize);
    dude[1] = juce::ImageCache::getFromMemory(BinaryData::p2_png, BinaryData::p2_pngSize);
    dude[2] = juce::ImageCache::getFromMemory(BinaryData::p3_png, BinaryData::p3_pngSize);
    dude[3] = juce::ImageCache::getFromMemory(BinaryData::p4_png, BinaryData::p4_pngSize);
    dude[4] = juce::ImageCache::getFromMemory(BinaryData::p5_png, BinaryData::p5_pngSize);
    dude[5] = juce::ImageCache::getFromMemory(BinaryData::p6_png, BinaryData::p6_pngSize);
    dude[6] = juce::ImageCache::getFromMemory(BinaryData::p7_png, BinaryData::p7_pngSize);
    dude[7] = juce::ImageCache::getFromMemory(BinaryData::p8_png, BinaryData::p8_pngSize);
    ship[0] = juce::ImageCache::getFromMemory(BinaryData::a1_png, BinaryData::a1_pngSize);
    ship[1] = juce::ImageCache::getFromMemory(BinaryData::a2_png, BinaryData::a2_pngSize);
    ship[2] = juce::ImageCache::getFromMemory(BinaryData::a3_png, BinaryData::a3_pngSize);
    ship[3] = juce::ImageCache::getFromMemory(BinaryData::a4_png, BinaryData::a4_pngSize);
    dudeIndex = 0; // index of currently loaded face

    t = & p.t; // use "time" variable computed in processor class
}

DelayLineAudioProcessorEditor::~DelayLineAudioProcessorEditor()
{
}

//==============================================================================
void DelayLineAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::orange);

    // DRAW BACKGROUND
    g.setOrigin(0, 0);
    g.drawImageAt(background, 0, 0);
    
    // DRAW DUDE
    dudeX = 200 - dudeScale * 55;
    dudeY = 285 - dudeScale * 60;
    g.setOrigin(dudeX, dudeY);
    juce::AffineTransform dudeTransform = juce::AffineTransform::scale(dudeScale);
    g.drawImageTransformed(dude[dudeIndex], dudeTransform);
    g.setOrigin(-dudeX, -dudeY);
    
    // DRAW SPACE SHIP
    g.setOrigin(200 - cosl(*t) * shipDistance, 285 - sinl(*t) * shipDistance);
    juce::AffineTransform shipTransform = juce::AffineTransform::rotation(*t, 0, 0);
    g.addTransform(juce::AffineTransform::scale(shipScale));
    g.drawImageTransformed(ship[rand()%4], shipTransform);

    DelayLineAudioProcessorEditor::repaint(); // this should be just for debugging
}

void DelayLineAudioProcessorEditor::resized()
{
    wetLabel.setBounds (10, 10, 90, 20);
    wetSlider.setBounds (100, 10, getWidth() - 110, 20);
    
    dryLabel.setBounds (10, 40, 90, 20);
    drySlider.setBounds (100, 40, getWidth() - 110, 20);
    
    sicknessLabel.setBounds (10, 70, 90, 20);
    sicknessSlider.setBounds (100, 70, getWidth() - 110, 20);

    airPollutionLabel.setBounds(10, 100, 90, 20);
    airPollutionSlider.setBounds(100, 100, getWidth() - 110, 20);

    speedLabel.setBounds(10, 130, 90, 20);
    speedSlider.setBounds(100, 130, getWidth() - 110, 20);

    distanceLabel.setBounds(10, 160, 90, 20);
    distanceSlider.setBounds(100, 160, getWidth() - 110, 20);
}

void DelayLineAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &wetSlider) {
        audioProcessor.set_wet(wetSlider.getValue());
        updateDudeScale(wetSlider.getValue());
    }
    else if (slider == &drySlider)
        audioProcessor.set_dry(drySlider.getValue());
    else if (slider == &sicknessSlider) {
        audioProcessor.set_ds(sicknessSlider.getValue());
        updateDudeIndex(sicknessSlider.getValue());
    }
    else if (slider == &airPollutionSlider) {
        audioProcessor.set_fb(airPollutionSlider.getValue());
        updateShipScale(airPollutionSlider.getValue());
    }
    else if (slider == &speedSlider) {
        audioProcessor.set_speed(speedSlider.getValue());
    }
    else if (slider == &distanceSlider) {
        audioProcessor.set_lfoAmp(distanceSlider.getValue());
        updateDistance(distanceSlider.getValue());
    }
}

void DelayLineAudioProcessorEditor::updateDudeIndex(int val) {
    float normalized = (val - 10) / 750.0f; // bring slider value in 0-1 range
    dudeIndex = (int) (normalized * 8);
}
void DelayLineAudioProcessorEditor::updateDistance(float val) {
    val = 0.6f - val; // invert range
    shipDistance = (int) 80 + 100*(val / 0.6f);
}
void DelayLineAudioProcessorEditor::updateShipScale(float val) {
    shipScale = 0.2f + val;
}
void DelayLineAudioProcessorEditor::updateDudeScale(float val) {
    dudeScale = 0.4f + val*0.5f;
}
