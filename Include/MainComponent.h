#pragma once

#include <JuceHeader.h>
#include "PracticeTimer.h"
#include "Metronome.h"

class MainComponent : public juce::AudioAppComponent, public juce::Timer
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
	
private:
	void timerCallback() override;
	
	Metronome mMetronome;
	PracticeTimer mPracticeTimer;
	
	juce::Label mMetronomeLabel;
	juce::TextButton mPlayButton;
	juce::ComboBox mChooseBox;
	juce::Slider mTempoSlider;
	juce::Slider mVolumeSlider;
	juce::Label mTempoLabel;
	juce::Label mVolumeLabel;
	juce::TextButton mOmitButton;
	
	juce::Label mPracticeTimerLabel;
	juce::ComboBox mPracticeTimerChooser;
	juce::Label mPracticeTimerDisplay;
	juce::TextButton mPracticeTimerStart;
	
	int mMetronomeHorizontalItemCount;
	int mTimerHorizontalItemCount;
	int mTotalHorizontalItemCount;
	
	bool mOmitStatus;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
