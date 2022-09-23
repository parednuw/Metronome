/*
  ==============================================================================

    cTimer.cpp
    Created: 19 Jun 2022 10:00:39pm
    Author:  Paul Wunder

  ==============================================================================
*/

#include "Countdown.h"

Countdown::Countdown()
{
	
}

Countdown::~Countdown()
{
	stopTimer();
}

void Countdown::resetCountdown()
{
	if ( isTimerRunning() )
	{
		mCountdownInSec = mRequestedCountdownInSec;
		stopTimer();
	}
}

void Countdown::timerCallback()
{
	counter();
}

void Countdown::counter()
{
	if (getTimerInterval() == 1000)
		mCountdownInSec -= 1;
	
	if (mCountdownInSec == 0)
	{
		countdownEnabled = false;
		resetCountdown();
	}
}

void Countdown::setCountdown(int requestedCountdownInMin)
{
	mRequestedCountdownInSec = countdownMinToSec(requestedCountdownInMin);
	mCountdownInSec = mRequestedCountdownInSec;
}

int Countdown::countdownMinToSec(int mCountdownInMin)
{
	return mCountdownInMin * 60;
}

int Countdown::getCountdownInSec()
{
	return mCountdownInSec;
}

juce::String Countdown::getCountdownInSecAsString()
{
	auto string = juce::String(mCountdownInSec);
	return string;
}

juce::String Countdown::getCountdownAsFormattedString()
{
	int minutes = std::floor(mCountdownInSec / 60);
	int seconds = mCountdownInSec - minutes * 60;
	juce::String finalString = juce::String(minutes);
	finalString += ":";
	finalString += juce::String(seconds);
	
	return finalString;
}
