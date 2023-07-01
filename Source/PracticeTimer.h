/*
 ==============================================================================
 
 cTimer.h
 Created: 19 Jun 2022 10:00:39pm
 Author:  Paul Wunder
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

class PracticeTimer : public juce::Timer
{
public:
	PracticeTimer();
	~PracticeTimer();
	
	void setPracticeTimer(int requestedPracticeTimerInMin);
	void resetPracticeTimer();
	int practiceTimerMinToSec(int timeInMin);
	
	void timerCallback() override;
	
	int getPracticeTimerInSec();
	juce::String getPracticeTimerInSecAsString();
	juce::String getPracticeTimerAsFormattedString();
	
	bool practiceTimerEnabled = false;
	
private:
	void counter();
	
	int mPracticeTimerInMin;
	int mPracticeTimerInSec;
	int mRequestedPracticeTimerInMin;
	int mRequestedPracticeTimerInSec;
};
