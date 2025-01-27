cmake_minimum_required(VERSION 3.15)
project(Metronome VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

juce_add_gui_app(${PROJECT_NAME}
    COMPANY_NAME paulwunder
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE PaWu                                                           
    PRODUCT_NAME "Metronome")


juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/Main.cpp
        src/MainComponent.cpp
        src/Metronome.cpp
        src/PracticeTimer.cpp)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_SILENCE_XCODE_15_LINKER_WARNING=1)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
