#pragma once

#include <JuceHeader.h>
#include "PracticeTimer.h"
#include "Metronome.h"

class MainComponent : public juce::AudioAppComponent,
					  public juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
	void repaint();
	void updatePracticeTimerDisplay();
	
	
	void play();
	void stop();
	void chooseSound();
	void omitClick();
	void changeMetronomeState();
	
	void choosePracticeTime();
	void startPracticeTimer();
	void stopPracticeTimer();
	void changePracticeTimerState();
	
	void timerCallback() override;
	
	enum class MetronomeState
	{
		Playing,
		Stopped
	};
	
	enum class PracticeTimerState
	{
		Counting,
		Stopped
	};
	
private:
	Metronome mMetronome;
	PracticeTimer mPracticeTimer;
	
	MetronomeState mMetronomeState { MetronomeState::Stopped };
	PracticeTimerState mPracticeTimerState { PracticeTimerState::Stopped };
	
	juce::Label mMetronomeLabel { {}, "METRONOME" };
	juce::TextButton mPlayButton { "Play" };
	juce::ComboBox mChooseBox { "Choose\n Sound"};
	juce::Slider mTempoSlider;
	juce::Slider mVolumeSlider;
	juce::Label mTempoLabel { {}, "Tempo" };
	juce::Label mVolumeLabel { {}, "Level" };
	juce::TextButton mOmitButton { "Omit" };
	
	juce::Label mPracticeTimerLabel { {}, "TIMER" };
	juce::ComboBox mPracticeTimerChooser { "Choose\n Time" };
	juce::Label mPracticeTimerDisplay { {}, "0:00" };
	juce::TextButton mPracticeTimerStart { "Start" };
	
	int mMetronomeHorizontalItemCount { 3 };
	int mTimerHorizontalItemCount { 1 };
	int mTotalHorizontalItemCount { mMetronomeHorizontalItemCount + mTimerHorizontalItemCount };
	
	bool omitStatus = false;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
