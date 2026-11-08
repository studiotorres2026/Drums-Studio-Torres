cmake_minimum_required(VERSION 3.22)
project(DrumsStudioTorres VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(DrumsStudioTorres
    COMPANY_NAME "Studio Torres"
    BUNDLE_ID "com.studiotorres.drumsstudiotorres"
    PLUGIN_MANUFACTURER_CODE StTr
    PLUGIN_CODE DST1
    FORMATS VST3 Standalone
    PRODUCT_NAME "Drums Studio Torres"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(DrumsStudioTorres)
target_sources(DrumsStudioTorres PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)
target_compile_features(DrumsStudioTorres PRIVATE cxx_std_17)
target_compile_definitions(DrumsStudioTorres PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)
target_link_libraries(DrumsStudioTorres PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags)
