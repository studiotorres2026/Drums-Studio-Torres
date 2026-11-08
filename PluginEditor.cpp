#include "PluginEditor.h"

DrumsStudioTorresAudioProcessorEditor::DrumsStudioTorresAudioProcessorEditor(
    DrumsStudioTorresAudioProcessor& p) : AudioProcessorEditor(&p), processor(p)
{
    setSize(760, 560);
    title.setText("DRUMS STUDIO TORRES", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    title.setFont(juce::Font(juce::FontOptions(27.0f, juce::Font::bold)));
    title.setColour(juce::Label::textColourId, juce::Colour(0xffffc857));
    addAndMakeVisible(title);

    for (int i = 0; i < 16; ++i)
    {
        auto& b = pads[(size_t)i];
        b.setButtonText("PAD " + juce::String(i + 1));
        b.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff292d35));
        b.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        b.onClick = [this, i] { selectedPad = i; processor.triggerPad(i); repaint(); };
        addAndMakeVisible(b);
    }
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffa87519));
    loadButton.onClick = [this] { chooseSample(); };
    addAndMakeVisible(loadButton);
}

void DrumsStudioTorresAudioProcessorEditor::chooseSample()
{
    auto chooser = std::make_shared<juce::FileChooser>(
        "Choose a WAV sample", juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav");
    chooser->launchAsync(juce::FileBrowserComponent::openMode |
                         juce::FileBrowserComponent::canSelectFiles,
        [this, chooser](const juce::FileChooser& fc)
        {
            const auto file = fc.getResult();
            if (file.existsAsFile())
                juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon,
                    "Sample selected", "WAV loading is the next implementation step. Selected: "
                    + file.getFileName());
        });
}

void DrumsStudioTorresAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff111318));
    g.setColour(juce::Colour(0xffffc857));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8), 12.0f, 1.5f);
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("16-PAD PERCUSSION SAMPLER  |  VST3 MULTI-OUT PROTOTYPE",
               20, 520, 720, 22, juce::Justification::centred);
}
void DrumsStudioTorresAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(22);
    title.setBounds(area.removeFromTop(58));
    area.removeFromTop(12);
    auto grid = area.removeFromTop(390);
    const int gap = 10, w = (grid.getWidth() - gap * 3) / 4;
    const int h = (grid.getHeight() - gap * 3) / 4;
    for (int i = 0; i < 16; ++i)
    {
        const int row = i / 4, col = i % 4;
        pads[(size_t)i].setBounds(grid.getX() + col * (w + gap),
                                  grid.getY() + row * (h + gap), w, h);
    }
    loadButton.setBounds(area.removeFromTop(42).withSizeKeepingCentre(180, 38));
}
