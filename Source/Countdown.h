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
	
	void setCountdown(int requestedTimeInMin);
	void resetCountdown();
	void countdownMinToSec(int timeInMin);
	
	void timerCallback() override;
	
	int getCountdownInSec();
	juce::String getCountdownInSecAsString();
	juce::String getCountdownAsFormattedString();
	
private:
	void counter();
	
	int mCountdownInMin;
	int mCountdownInSec;
	
};
