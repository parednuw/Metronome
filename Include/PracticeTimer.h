#pragma once

#include <JuceHeader.h>

class PracticeTimer : public juce::Timer
{
public:
	PracticeTimer();
	~PracticeTimer();
	
	void setPracticeTimerState(bool stateToSet);
	bool getPracticeTimerState();
	
	void setPracticeTimer(int requestedPracticeTimerInMin);
	void resetPracticeTimer();
	int practiceTimerMinToSec(int timeInMin);
	int getPracticeTimerInSec();
	juce::String getPracticeTimerInSecAsString();
	juce::String getPracticeTimerAsFormattedString();
	
private:
	void counter();
	void timerCallback() override;
	
	bool mPracticeTimerState;
	int mPracticeTimerInMin;
	int mPracticeTimerInSec;
	int mRequestedPracticeTimerInMin;
	int mRequestedPracticeTimerInSec;
};
