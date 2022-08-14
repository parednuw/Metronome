/*
  ==============================================================================

    Metronome.cpp
    Created: 28 Feb 2022 5:45:28pm
    Author:  Paul Wunder

  ==============================================================================
*/

#include "Metronome.h"

Metronome::Metronome()
{
	mFormatManager.registerBasicFormats();
	
	setFileToPlay("HIHAT.wav");
}

void Metronome::setFileToPlay(const juce::String& nameOfFileToPlay)
{
	
	juce::File fileToPlay { juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory()};
	auto mSamples = fileToPlay.findChildFiles(juce::File::TypesOfFileToFind::findFiles, true, nameOfFileToPlay);
	jassert(mSamples[0].exists());
	
	setmSamples(mSamples);
}

void Metronome::setmSamples(juce::Array<juce::File> mSamplesFromChooser)
{
	mSamples = mSamplesFromChooser;
	auto formatReader = mFormatManager.createReaderFor(mSamples[0]);
	pMetronomeSample.reset(new juce::AudioFormatReaderSource(formatReader, true));
}

//==============================================================================
void Metronome::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	mSampleRate = sampleRate;
	mBPMInSamples = 60.0 / mBPM * mSampleRate;
	juce::HighResolutionTimer::startTimer(100);
	
	if (pMetronomeSample != nullptr)
	{
		pMetronomeSample->prepareToPlay(samplesPerBlockExpected, sampleRate);
		DBG("File loaded.");
	}
}

void Metronome::calcWhenPlayFile(const juce::AudioSourceChannelInfo& bufferToFill)
{
	auto bufferSize = bufferToFill.numSamples;

	if (firstTrigger == true)
	{
		pMetronomeSample->setNextReadPosition(0);
		pMetronomeSample->getNextAudioBlock(bufferToFill);
	}
	
	firstTrigger = false;
	
	mTotalSamples += bufferSize;
	mSamplesRemainder = mTotalSamples % mBPMInSamples;
	//DBG("Samples remainder: " << mSamplesRemainder);
	//DBG("Beat Interval: " << mInterval);
	
	if ((mSamplesRemainder + bufferSize) >= mBPMInSamples)
	{
		const auto timeToStartPlaying = mBPMInSamples - mSamplesRemainder;
		pMetronomeSample->setNextReadPosition(0);
		
		for (int sample = 0; sample < bufferSize; sample++)
		{
			if (sample == timeToStartPlaying)
			{
				//DBG("#########\n" << "CLICK\n" << "Total Samples: " << mTotalSamples << "\n#########");
				pMetronomeSample->getNextAudioBlock(bufferToFill);
			}
		}
		
		mTotalSamples = 0;
	}
	
	if (pMetronomeSample->getNextReadPosition() != 0)
	{
		pMetronomeSample->getNextAudioBlock(bufferToFill);
	}
	
}

//==============================================================================
void Metronome::resetCountSamples()
{
	mTotalSamples = 0;

}

void Metronome::setTempo(double newTempo)
{
	mBPM = newTempo;
}

void Metronome::hiResTimerCallback()
{
	mBPMInSamples = 60.0 / mBPM * mSampleRate;
}

//==============================================================================
void Metronome::sliderValueChanged(juce::Slider *slider)
{
	mAudioLevel = slider->getValue();
}

float Metronome::getSliderLevel()
{
	return mAudioLevel;
}

void Metronome::setFirstTrigger(bool trigger)
{
	firstTrigger = trigger;
}

