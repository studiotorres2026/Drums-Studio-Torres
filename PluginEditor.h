#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class DrumsStudioTorresAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit DrumsStudioTorresAudioProcessorEditor(DrumsStudioTorresAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    DrumsStudioTorresAudioProcessor& processor;
    std::array<juce::TextButton, 16> pads;
    juce::TextButton loadButton { "LOAD WAV" };
    juce::Label title;
    int selectedPad = 0;
    void chooseSample();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsStudioTorresAudioProcessorEditor)
};
