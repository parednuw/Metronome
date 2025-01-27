#include "PracticeTimer.h"

PracticeTimer::PracticeTimer() : mPracticeTimerState(false)
{
	
}

PracticeTimer::~PracticeTimer()
{
	stopTimer();
}

void PracticeTimer::resetPracticeTimer()
{
	if ( isTimerRunning() )
	{
		mPracticeTimerInSec = mRequestedPracticeTimerInSec;
		stopTimer();
	}
}

void PracticeTimer::timerCallback()
{
	counter();
}

void PracticeTimer::counter()
{
	if (getTimerInterval() == 1000)
	{
		mPracticeTimerInSec -= 1;
	}
	
	if (mPracticeTimerInSec == 0)
	{
		mPracticeTimerState = false;
		resetPracticeTimer();
	}
}

void PracticeTimer::setPracticeTimer(int requestedCountdownInMin)
{
	mRequestedPracticeTimerInSec = practiceTimerMinToSec(requestedCountdownInMin);
	mPracticeTimerInSec = mRequestedPracticeTimerInSec;
}

int PracticeTimer::practiceTimerMinToSec(int mPracticeTimerInMin)
{
	return mPracticeTimerInMin * 60;
}

void PracticeTimer::setPracticeTimerState(bool stateToSet)
{
	mPracticeTimerState = stateToSet;
}

bool PracticeTimer::getPracticeTimerState()
{
	return mPracticeTimerState;
}

int PracticeTimer::getPracticeTimerInSec()
{
	return mPracticeTimerInSec;
}

juce::String PracticeTimer::getPracticeTimerInSecAsString()
{
	auto string = juce::String(mPracticeTimerInSec);
	return string;
}

juce::String PracticeTimer::getPracticeTimerAsFormattedString()
{
	int minutes = std::floor(mPracticeTimerInSec / 60);
	int seconds = mPracticeTimerInSec - minutes * 60;
	juce::String finalString = juce::String(minutes);
	finalString += ":";
	
	if (seconds < 10)
	{
		finalString += ("0" + juce::String(seconds));
	}
	else
	{
		finalString += juce::String(seconds);
	}
	
	return finalString;
}
