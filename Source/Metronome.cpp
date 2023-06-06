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
	juce::File fileToPlayDir { "/Users/paulwunder/Dev/Projects/Metronome/Audio"};	//Replace path with your path to "/Metronome/Audio"
	auto mSamples = fileToPlayDir.findChildFiles(juce::File::TypesOfFileToFind::findFiles, true, nameOfFileToPlay);
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
		//DBG("File loaded.");
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
	
	if ((mSamplesRemainder + bufferSize) >= mBPMInSamples)
	{
		const int timeToStartPlaying = mBPMInSamples - mSamplesRemainder;
		pMetronomeSample->setNextReadPosition(0);
		for (int sample = 0; sample <= bufferSize; sample++)
		{
			if (sample == timeToStartPlaying)
			{
				//DBG("#########\n" << "CLICK\n" << "Total Samples: " << mTotalSamples << "\n#########");
				mOmitCount += 1;
				DBG(mOmitCount);
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

void Metronome::setOmitCount(int value)
{
	mOmitCount = value;
}

int Metronome::getOmitCount()
{
	return mOmitCount;
}

/*void Metronome::omitCounterAdd1(int maxOmit)
{
	mOmitCount += 1;
	if (mOmitCount == maxOmit)
		mOmitCount = 0;
}*/

void Metronome::hiResTimerCallback()
{
	mBPMInSamples = 60.0 / mBPM * mSampleRate;
}

//==============================================================================
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
	firstTrigger = trigger;
}

