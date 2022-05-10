#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>
#include <windows.h>

//==============================================================================
DelayLineAudioProcessor::DelayLineAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    )
#endif
    {
        // INITIALIZE BUFFER
        dbufSize = 2000;
        dbuf.setSize(getTotalNumOutputChannels(), dbufSize);
        dbuf.clear();

        // INITIALIZE SLIDERS
        wet = 0.4f;
        dry = 0.6f;
        ds = 30;
        fb = 0.5f;
        speed = 0.00001f;
        lfoAmp = 0.5f;

        // INITIALIZE BUFFER WRITER AND READER POINTERS
        dw = 0;
        dr = abs((dw - ds) % dbufSize);
        dr_remainder = 0;

        t = 0.0f; // time
    }


DelayLineAudioProcessor::~DelayLineAudioProcessor()
{
}

//==============================================================================
const juce::String DelayLineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayLineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayLineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayLineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayLineAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayLineAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayLineAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayLineAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayLineAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayLineAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayLineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{ 
}

void DelayLineAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayLineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void DelayLineAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) // https://docs.juce.com/master/classAudioProcessor.html#:~:text=So%20for%20example%20if%20your%20processor%20has%20a%20total%20of%202%20input%20channels%20and%204%20output%20channels%2C%20then%20the%20buffer%20will%20contain%204%20channels%2C%20the%20first%20two%20being%20filled%20with%20the%20input%20data.%20Your%20processor%20should%20read%20these%2C%20do%20its%20processing%2C%20and%20replace%20the%20contents%20of%20all%204%20channels%20with%20its%20output.
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();
    float wet_now = wet;
    float dry_now = dry;
    float fb_now = fb;
    float speed_now = speed;
    float lfoAmp_now = lfoAmp;
    int ds_now = ds;
    
    float* channelOutDataL = buffer.getWritePointer(0);
    float* channelOutDataR = buffer.getWritePointer(1);
    
    const float* channelInDataL = buffer.getReadPointer(0);
    const float* channelInDataR = buffer.getReadPointer(1);

    for (int i = 0; i < numSamples; ++i) {
        // COMPUTE "TIME"
        t += speed_now;

        // READ DELAY OUTPUT, INTERPOLATE
        int sample_to_interpolate;
        if (dr_remainder >= 0) { // interpolate with next sample
            sample_to_interpolate = (dr + 1) % dbufSize;
        }
        else { // interpolate with previous sample
            dr_remainder = -dr_remainder; // take absolute value of remainder
            
            // poor man's modulo
            if (dr > 0) {
                sample_to_interpolate = dr - 1;
            }
            else {
                sample_to_interpolate = dbufSize - 1;
            }
        }
        float delayOutL = (1 - dr_remainder) * dbuf.getSample(0, dr) + dr_remainder * dbuf.getSample(0, sample_to_interpolate);
        float delayOutR = (1 - dr_remainder) * dbuf.getSample(1, dr) + dr_remainder * dbuf.getSample(1, sample_to_interpolate);
        
        // COMPUTE DELAY INPUT AS SUM OF INPUT SIGNAL AND FEEDBACK FROM DELAY OUTPUT
        auto delayInL = delayOutL*fb_now + channelInDataL[i];
        auto delayInR = delayOutR*fb_now + channelInDataR[i];

        // WRITE IN DELAY BUFFER
        dbuf.setSample(0, dw, delayInL);
        dbuf.setSample(1, dw, delayInR);
        
        // WRITE OUTPUT
        channelOutDataL[i] = dry_now * channelInDataL[i] + wet_now * delayOutL;
        channelOutDataR[i] = dry_now * channelInDataR[i] + wet_now * delayOutR;

        // COMPUTE LFO TO APPLY ON READ POINTER
        long double delta_double = (lfoAmp_now * ds_now * sinl(t));
        int delta = (int) delta_double;
        dr_remainder = delta_double - delta;
        
        // UPDATE BUFFER WRITER AND READER POINTER
        dw = (dw + 1) % dbufSize;
        auto diff = dw - ds_now + delta;
        if (diff >= 0) { // "%" IS NOT THE MODULO, IT'S THE REMAINDER
            dr = diff % dbufSize;
        }
        else {
            dr = diff + dbufSize;
        }
    }
}

//==============================================================================
bool DelayLineAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayLineAudioProcessor::createEditor()
{
    return new DelayLineAudioProcessorEditor (*this);
}

//==============================================================================
// As a bonus I tried to implement the state saving/loading mechanism.
// It is correctly loaded and saved in AudioPluginHost but
// it doesn't work in Ableton Live for example;
// Live calls getStateInformation() before setStateInformation() and
// sometimes NEVER calls setStateInformation()

void DelayLineAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.

    size_t floatSize = sizeof(float);
    size_t intSize = sizeof(int);
    size_t position = 0;

    destData.insert(&wet, floatSize, position);
    position += floatSize;
    destData.insert(&dry, floatSize, position);
    position += floatSize;
    destData.insert(&fb, floatSize, position);
    position += floatSize;
    destData.insert(&speed, floatSize, position);
    position += floatSize;
    destData.insert(&lfoAmp, floatSize, position);
    position += floatSize;
    destData.insert(&ds, intSize, position);
}

void DelayLineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::MemoryBlock mem = juce::MemoryBlock(data, sizeInBytes);
    void* address = mem.getData();
    
    // load data from memory with the most beautiful and readable code ever
    wet = * (float*)address; 
    address = (void*) ((float*)address + 1);
    dry = *(float*)address;
    address = (void*)((float*)address + 1);
    fb = *(float*)address;
    address = (void*)((float*)address + 1);
    speed = *(float*)address;
    address = (void*)((float*)address + 1);
    lfoAmp = *(float*)address;
    address = (void*)((float*)address + 1);
    ds = *(int*)address;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayLineAudioProcessor();
}

void DelayLineAudioProcessor::set_wet(float val)
{
    wet = val;
}
void DelayLineAudioProcessor::set_dry(float val)
{
    dry = val;
}
void DelayLineAudioProcessor::set_fb(float val)
{
    fb = val;
}
void DelayLineAudioProcessor::set_speed(float val)
{
    speed = val;
}
void DelayLineAudioProcessor::set_lfoAmp(float val)
{
    lfoAmp = val;
}
void DelayLineAudioProcessor::set_ds(int val)
{
    ds = val;
}