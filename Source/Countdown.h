/*
  ==============================================================================

    cTimer.h
    Created: 19 Jun 2022 10:00:39pm
    Author:  Paul Wunder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Countdown : public juce::Timer
{
public:
	Countdown();
	~Countdown();
	
	void setCountdown(int requestedCountdownInMin);
	void resetCountdown();
	int countdownMinToSec(int timeInMin);
	
	void timerCallback() override;
	
	int getCountdownInSec();
	juce::String getCountdownInSecAsString();
	juce::String getCountdownAsFormattedString();
	
	bool countdownEnabled = false;
	
private:
	void counter();
	
	int mCountdownInMin;
	int mCountdownInSec;
	int mRequestedCountdownInMin;
	int mRequestedCountdownInSec;
};
