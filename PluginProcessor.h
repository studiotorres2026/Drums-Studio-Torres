#pragma once
#include <JuceHeader.h>

class DrumsStudioTorresAudioProcessor : public juce::AudioProcessor
{
public:
    DrumsStudioTorresAudioProcessor();
    ~DrumsStudioTorresAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 10.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    bool loadSample(int pad, const juce::File& file);
    void triggerPad(int pad);
    juce::String getSampleName(int pad) const;
    void setPadGain(int pad, float gain);
    void setPadPan(int pad, float pan);
    void setPadMute(int pad, bool mute);
    void setPadSolo(int pad, bool solo);

private:
    struct Pad
    {
        juce::AudioBuffer<float> audio;
        juce::String name { "Empty" };
        double position = 0.0;
        float gain = 1.0f, pan = 0.0f;
        bool playing = false, mute = false, solo = false;
    };
    std::array<Pad, 16> pads;
    double currentSampleRate = 44100.0;
    juce::CriticalSection sampleLock;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsStudioTorresAudioProcessor)
};
