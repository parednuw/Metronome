#pragma once

#include <JuceHeader.h>
#include "Countdown.h"
#include "Metronome.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
					  public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
	void repaint();
	void updateCountdownDisplay();
	
	
	void play();
	void stop();
	void chooseSound();
	void changeMetronomeState();
	
	void chooseCountdown();
	void startCountdown();
	void stopCountdown();
	void changeCountdownState();
	
	void timerCallback() override;
	
	enum class MetronomeState
	{
		Playing,
		Stopped
	};
	
	enum class CountdownState
	{
		Counting,
		Stopped
	};
	
private:
	Metronome mMetronome;
	Countdown mCountdown;
	
	juce::TextButton mPlayButton { "Play" };
	//juce::TextButton mStopButton { "Stop" };
	juce::Label mChooseLabel { {}, "Choose Sound" };
	juce::ComboBox mChooseBox { "Choose\n Sound"};
	juce::Slider mTempoSlider;
	juce::Slider mVolumeSlider;
	juce::Label mCountdownLabel { {}, "TIMER" };
	juce::ComboBox mCountdownChooser { "Choose\n Time" };
	juce::Label mCountdownDisplay { {}, "0:0" };
	juce::TextButton mCountdownStart { "Start" };
	int mHorizontalItemCount { 5 };
	
	MetronomeState mMetronomeState { MetronomeState::Stopped };
	CountdownState mCountdownState { CountdownState::Stopped };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
