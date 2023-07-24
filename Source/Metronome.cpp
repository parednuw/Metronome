/*
 ==============================================================================
 
 Metronome.cpp
 Created: 28 Feb 2022 5:45:28pm
 Author:  Paul Wunder
 
 ==============================================================================
 */

#include "Metronome.h"

Metronome::Metronome() : mTotalSamples(0), mSampleRate(0), mBPMInSamples(0), mFirstTrigger(false), mIsAudioTriggered(false), mOmitCount(1), mMetronomeSample(nullptr), mMetronomeState(MetronomeState::Stopped)
{
	mFormatManager.registerBasicFormats();
	
	setFileToPlay("HIHAT.wav");
}

void Metronome::setFileToPlay(const juce::String& nameOfFileToPlay)
{
	juce::File fileToPlayDir { "/Users/paulwunder/Dev/Projects/Metronome/Audio"};
	auto mSamples = fileToPlayDir.findChildFiles(juce::File::TypesOfFileToFind::findFiles, true, nameOfFileToPlay);
	jassert(mSamples[0].exists());
	
	setmSamples(mSamples);
}

void Metronome::setmSamples(const juce::Array<juce::File>& mSamplesFromChooser)
{
	auto formatReader = mFormatManager.createReaderFor(mSamplesFromChooser[0]);
	mMetronomeSample.reset(new juce::AudioFormatReaderSource(formatReader, true));
}

void Metronome::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	mSampleRate = sampleRate;
	mBPMInSamples = 60.0 / mBPM * mSampleRate;
	juce::HighResolutionTimer::startTimer(100);
	
	if (mMetronomeSample != nullptr)
	{
		mMetronomeSample->prepareToPlay(samplesPerBlockExpected, sampleRate);
		//DBG("File loaded.");
	}
}

void Metronome::calcWhenPlayFile(const juce::AudioSourceChannelInfo& bufferToFill)
{
	auto bufferSize = bufferToFill.numSamples;
	
	if (mFirstTrigger == true)
	{
		mMetronomeSample->setNextReadPosition(0);
		mMetronomeSample->getNextAudioBlock(bufferToFill);
	}
	
	mFirstTrigger = false;
	
	mTotalSamples += bufferSize;
	mSamplesRemainder = mTotalSamples % mBPMInSamples;
	
	if ((mSamplesRemainder + bufferSize) >= mBPMInSamples)
	{
		const int timeToStartPlaying = mBPMInSamples - mSamplesRemainder;
		mMetronomeSample->setNextReadPosition(0);
		for (int sample = 0; sample <= bufferSize; sample++)
		{
			if (sample == timeToStartPlaying)
			{
				//DBG("#########\n" << "CLICK\n" << "Total Samples: " << mTotalSamples << "\n#########");
				mOmitCount += 1;
				DBG(mOmitCount);
				mMetronomeSample->getNextAudioBlock(bufferToFill);
			}
		}
		
		mTotalSamples = 0;
	}
	
	if (mMetronomeSample->getNextReadPosition() != 0)
	{
		mMetronomeSample->getNextAudioBlock(bufferToFill);
	}
}

void Metronome::resetCountSamples()
{
	mTotalSamples = 0;
}

void Metronome::setMetronomeState(MetronomeState stateToSet)
{
	mMetronomeState = stateToSet;
}

Metronome::MetronomeState Metronome::getMetronomeState()
{
	return mMetronomeState;
}

void Metronome::setTempo(double newTempo)
{
	mBPM = newTempo;
}

void Metronome::setOmitCount(int value)
{
	mOmitCount = value;
}

int Metronome::getOmitCount()
{
	return mOmitCount;
}

void Metronome::hiResTimerCallback()
{
	mBPMInSamples = 60.0 / mBPM * mSampleRate;
}

void Metronome::sliderValueChanged(juce::Slider *slider)
{
	mAudioLeveldB = slider->getValue();
}

float Metronome::getSliderLevel()
{
	return mAudioLeveldB;
}

void Metronome::setFirstTrigger(bool trigger)
{
	mFirstTrigger = trigger;
}

