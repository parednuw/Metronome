#pragma once

#include <JuceHeader.h>

class Metronome : public juce::HighResolutionTimer, public juce::Slider::Listener
{
public:
	Metronome();
	
	void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	void calcWhenPlayFile(const juce::AudioSourceChannelInfo&);
	void resetCountSamples();
	
	enum class MetronomeState
	{
		Playing,
		Stopped
	};
	
	void setMetronomeState(MetronomeState stateToSet);
	MetronomeState getMetronomeState();
	
	void setTempo(double newTempo);
	void setOmitCount(int value);
	int getOmitCount();
	
	void setFileToPlay(const juce::String& nameOfFileToPlay);
	void setmSamples(const juce::Array<juce::File>& mSamplesFromChooser);
	
	void sliderValueChanged(juce::Slider *slider) override;
	float getSliderLevel();
	
	void setFirstTrigger(bool trigger);
	bool getIsAudioTriggered();
	
private:
	void hiResTimerCallback() override;
	
	MetronomeState mMetronomeState;
	
	int mTotalSamples;
	double mSampleRate;
	int mBPMInSamples;
	double mBPM;
	int mSamplesRemainder;
	float mAudioLeveldB;
	bool mFirstTrigger;
	bool mIsAudioTriggered;
	
	int mOmitCount;
	
	juce::AudioFormatManager mFormatManager;
	std::unique_ptr <juce::AudioFormatReaderSource> mMetronomeSample;
};
