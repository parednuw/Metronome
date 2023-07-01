/*
 ==============================================================================
 
 Metronome.h
 Created: 28 Feb 2022 5:45:28pm
 Author:  Paul Wunder
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

class Metronome : public juce::HighResolutionTimer, public juce::Slider::Listener
{
public:
	Metronome();
	
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void calcWhenPlayFile(const juce::AudioSourceChannelInfo&);
	void resetCountSamples();
	
	void hiResTimerCallback() override;
	
	void setTempo(double newTempo);
	void setOmitCount(int value);
	int getOmitCount();
	
	void setFileToPlay(const juce::String& nameOfFileToPlay);
	void setmSamples(juce::Array<juce::File> mSamplesFromChooser);
	
	void sliderValueChanged(juce::Slider *slider) override;
	float getSliderLevel();
	
	void setFirstTrigger(bool trigger);
	bool getIsAudioTriggered();
	
private:
	int mTotalSamples;
	double mSampleRate;
	int mBPMInSamples;
	double mBPM;
	int mSamplesRemainder;
	float mAudioLeveldB;
	bool firstTrigger;
	bool mIsAudioTriggered;
	
	int mOmitCount;
	
	juce::AudioFormatManager mFormatManager;
	std::unique_ptr <juce::AudioFormatReaderSource> pMetronomeSample;
	
	juce::Array<juce::File> mSamples;
};
