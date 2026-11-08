#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessor::BusesProperties makeBuses()
{
    juce::AudioProcessor::BusesProperties b;
    b = b.withOutput("Main", juce::AudioChannelSet::stereo(), true);
    for (int i = 0; i < 16; ++i)
        b = b.withOutput("Pad " + juce::String(i + 1), juce::AudioChannelSet::stereo(), true);
    return b;
}

DrumsStudioTorresAudioProcessor::DrumsStudioTorresAudioProcessor()
    : AudioProcessor(makeBuses()) {}

void DrumsStudioTorresAudioProcessor::prepareToPlay(double sr, int)
{
    currentSampleRate = sr;
    for (auto& p : pads) p.position = 0.0;
}

bool DrumsStudioTorresAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.outputBuses.size() != 17) return false;
    for (int i = 0; i < layouts.outputBuses.size(); ++i)
        if (layouts.outputBuses[i] != juce::AudioChannelSet::stereo()) return false;
    return true;
}

void DrumsStudioTorresAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                    juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    for (int b = 0; b < getBusCount(false); ++b)
        if (auto* bus = getBus(false, b); bus != nullptr && bus->isEnabled())
            getBusBuffer(buffer, false, b).clear();

    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            const int note = msg.getNoteNumber();
            if (note >= 36 && note < 52) triggerPad(note - 36);
        }
    }

    const bool anySolo = std::any_of(pads.begin(), pads.end(), [](const Pad& p){ return p.solo; });
    const juce::ScopedLock lock(sampleLock);
    for (int i = 0; i < 16; ++i)
    {
        auto& p = pads[(size_t)i];
        if (!p.playing || p.audio.getNumSamples() == 0) continue;
        if (p.mute || (anySolo && !p.solo)) continue;
        if (auto* bus = getBus(false, i + 1); bus != nullptr && bus->isEnabled())
        {
            auto out = getBusBuffer(buffer, false, i + 1);
            const int n = juce::jmin(out.getNumSamples(),
                p.audio.getNumSamples() - (int)p.position);
            if (n > 0)
            {
                const float left = p.gain * (p.pan > 0.0f ? 1.0f - p.pan : 1.0f);
                const float right = p.gain * (p.pan < 0.0f ? 1.0f + p.pan : 1.0f);
                for (int ch = 0; ch < out.getNumChannels(); ++ch)
                {
                    const int srcCh = juce::jmin(ch, p.audio.getNumChannels() - 1);
                    out.addFrom(ch, 0, p.audio, srcCh, (int)p.position, n,
                                ch == 0 ? left : right);
                }
                p.position += n;
            }
            if (p.position >= p.audio.getNumSamples()) p.playing = false;
        }
    }
}

bool DrumsStudioTorresAudioProcessor::loadSample(int pad, const juce::File& file)
{
    if (pad < 0 || pad >= 16 || !file.existsAsFile()) return false;
    juce::AudioFormatManager fm;
    fm.registerBasicFormats();
    std::unique_ptr<juce::AudioFormatReader> reader(fm.createReaderFor(file));
    if (!reader) return false;
    juce::AudioBuffer<float> loaded((int)reader->numChannels, (int)reader->lengthInSamples);
    reader->read(&loaded, 0, (int)reader->lengthInSamples, 0, true, true);
    const juce::ScopedLock lock(sampleLock);
    auto& p = pads[(size_t)pad];
    p.audio = std::move(loaded);
    p.name = file.getFileNameWithoutExtension();
    p.position = 0.0;
    p.playing = false;
    return true;
}

void DrumsStudioTorresAudioProcessor::triggerPad(int pad)
{
    if (pad < 0 || pad >= 16) return;
    const juce::ScopedLock lock(sampleLock);
    auto& p = pads[(size_t)pad];
    if (p.audio.getNumSamples() > 0) { p.position = 0.0; p.playing = true; }
}

juce::String DrumsStudioTorresAudioProcessor::getSampleName(int pad) const
{
    if (pad < 0 || pad >= 16) return {};
    return pads[(size_t)pad].name;
}
void DrumsStudioTorresAudioProcessor::setPadGain(int pad, float v)
{ if (pad >= 0 && pad < 16) pads[(size_t)pad].gain = juce::jlimit(0.0f, 2.0f, v); }
void DrumsStudioTorresAudioProcessor::setPadPan(int pad, float v)
{ if (pad >= 0 && pad < 16) pads[(size_t)pad].pan = juce::jlimit(-1.0f, 1.0f, v); }
void DrumsStudioTorresAudioProcessor::setPadMute(int pad, bool v)
{ if (pad >= 0 && pad < 16) pads[(size_t)pad].mute = v; }
void DrumsStudioTorresAudioProcessor::setPadSolo(int pad, bool v)
{ if (pad >= 0 && pad < 16) pads[(size_t)pad].solo = v; }

juce::AudioProcessorEditor* DrumsStudioTorresAudioProcessor::createEditor()
{ return new DrumsStudioTorresAudioProcessorEditor(*this); }

void DrumsStudioTorresAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void DrumsStudioTorresAudioProcessor::setStateInformation(const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ return new DrumsStudioTorresAudioProcessor(); }
