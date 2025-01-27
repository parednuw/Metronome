#include "MainComponent.h"

MainComponent::MainComponent() : mPlayButton({"Play"}), mTempoLabel( {}, "Tempo"), mVolumeLabel({}, "Level"), mOmitButton({"Omit"}), mMetronomeLabel({}, "METRONOME"), mPracticeTimerLabel({}, "TIMER"), mPracticeTimerDisplay({}, "0:00"), mPracticeTimerStart({"Start"}), mMetronomeHorizontalItemCount(3), mTimerHorizontalItemCount(1), mOmitStatus(false)
{
	
	mChooseBox.addItem("SWNB", 1);
	mChooseBox.addItem("Cowbell", 2);
	mChooseBox.setJustificationType(juce::Justification::centred);
	mChooseBox.setSelectedId(1);
	mChooseBox.onChange = [this]() { chooseSound(); };
	addAndMakeVisible(&mChooseBox);
	
	mPlayButton.setToggleState(false, juce::NotificationType::dontSendNotification);
	mPlayButton.setRadioGroupId(1);
	mPlayButton.onClick = [this]() { changeMetronomeState(); };
	mPlayButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF35C521));
	addAndMakeVisible(&mPlayButton);
	
	mTempoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	mTempoSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 70, 30);
	mTempoSlider.setRange(20, 300, 1);
	mTempoSlider.setValue(60);
	mTempoSlider.setNumDecimalPlacesToDisplay(0);
	mTempoSlider.setTextValueSuffix(" bpm");
	mTempoSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
	addAndMakeVisible(&mTempoSlider);
	
	mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	mVolumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 90, 30);
	mVolumeSlider.setSkewFactorFromMidPoint(7);
	mVolumeSlider.setRange(-98, 0, 0.1);
	mVolumeSlider.setValue(-12);
	mVolumeSlider.setNumDecimalPlacesToDisplay(1);
	mVolumeSlider.setTextValueSuffix(" dBFS");
	mVolumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
	mVolumeSlider.addListener(&mMetronome);
	addAndMakeVisible(&mVolumeSlider);
	
	mTempoLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mTempoLabel);
	
	mVolumeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mVolumeLabel);
	
	mPracticeTimerLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mPracticeTimerLabel);
	
	mMetronomeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mMetronomeLabel);
	
	mOmitButton.setToggleState(false, juce::NotificationType::dontSendNotification);
	mOmitButton.setRadioGroupId(1);
	mOmitButton.onClick = [this]() { omitClick(); };
	mOmitButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF573068));
	addAndMakeVisible(&mOmitButton);
	
	int itemCount = 6;
	for (int i = 1; i <= itemCount; i++)
	{
		mPracticeTimerChooser.addItem(std::to_string(5 * i), i);
	}
	mPracticeTimerChooser.setSelectedId(1);
	mPracticeTimerChooser.setJustificationType(juce::Justification::centred);
	mPracticeTimerChooser.onChange = [this]() { choosePracticeTime(); };
	addAndMakeVisible(&mPracticeTimerChooser);
	
	mPracticeTimerDisplay.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mPracticeTimerDisplay);
	
	mPracticeTimerStart.onClick = [this]() { changePracticeTimerState(); };
	mPracticeTimerStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFABC0A8));
	addAndMakeVisible(&mPracticeTimerStart);
	
	Timer::startTimerHz(60);
	
	setSize (400, 300);
	
	if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
		&& ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
	{
		juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
										   [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
	}
	else
	{
		setAudioChannels (0, 2);
	}
	
	mTotalHorizontalItemCount = mMetronomeHorizontalItemCount + mTimerHorizontalItemCount;
}

MainComponent::~MainComponent()
{
	Timer::stopTimer();
	shutdownAudio();
}

void MainComponent::play()
{
	mMetronome.setFirstTrigger(true);
}

void MainComponent::stop()
{
	mMetronome.resetCountSamples();
	mMetronome.setOmitCount(1);
}

void MainComponent::changeMetronomeState()
{
	if (mMetronome.getMetronomeState() == Metronome::MetronomeState::Stopped)
	{
		mMetronome.setMetronomeState(Metronome::MetronomeState::Playing);
		mPlayButton.setButtonText("Stop");
		mPlayButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFC52121));
		play();
	}
	else if (mMetronome.getMetronomeState() == Metronome::MetronomeState::Playing)
	{
		mMetronome.setMetronomeState(Metronome::MetronomeState::Stopped);
		mPlayButton.setButtonText("Play");
		mPlayButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF35C521));
		stop();
	}
}

void MainComponent::chooseSound()
{
	int id = mChooseBox.getSelectedId();
	
	if (id == 1)
	{
		mMetronome.setFileToPlay("HIHAT.wav");
	}
	else if (id == 2)
	{
		mMetronome.setFileToPlay("COWBELL.wav");
	}
	
}

void MainComponent::omitClick()
{
	mOmitStatus = !mOmitStatus;
	mOmitButton.setColour(juce::TextButton::ColourIds::buttonColourId, mOmitStatus ? juce::Colour(0xFF003068) : juce::Colour(0xFF573068));
	mOmitButton.setButtonText(mOmitStatus ? "Omitting" : "Omit");
	if (!mOmitStatus)
	{
		mMetronome.setOmitCount(0);
	}
}

void MainComponent::choosePracticeTime()
{
	mPracticeTimer.resetPracticeTimer();
	
	mPracticeTimer.setPracticeTimerState(false);
	mPracticeTimerStart.setButtonText("Start");
	
	int id = mPracticeTimerChooser.getSelectedId();
	mPracticeTimer.setPracticeTimer(5 * id);
}

void MainComponent::startPracticeTimer()
{
	mPracticeTimer.startTimer(1000);
	mPracticeTimer.setPracticeTimerState(true);
}

void MainComponent::stopPracticeTimer()
{
	mPracticeTimer.setPracticeTimerState(false);
	choosePracticeTime();
}

void MainComponent::changePracticeTimerState()
{
	if (mPracticeTimer.getPracticeTimerState() == false)
	{
		mPracticeTimer.setPracticeTimerState(true);
		mPracticeTimerStart.setButtonText("Stop");
		mPracticeTimerStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFC52121));
		startPracticeTimer();
	}
	else if (mPracticeTimer.getPracticeTimerState() == true)
	{
		mPracticeTimer.setPracticeTimerState(false);
		mPracticeTimerStart.setButtonText("Start");
		mPracticeTimerStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFC52121));
		stopPracticeTimer();
	}
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	mMetronome.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();
	
	if (mMetronome.getMetronomeState() == Metronome::MetronomeState::Playing)
	{
		mMetronome.calcWhenPlayFile(bufferToFill);
	}
	
	float level = juce::Decibels::decibelsToGain(mMetronome.getSliderLevel());
	
	int numberOfClicksToOmit = 5;
	
	if (mOmitStatus && mMetronome.getOmitCount() > numberOfClicksToOmit)
	{
		level = 0;
		if (mMetronome.getOmitCount() == (numberOfClicksToOmit * 2 + 1))
		{
			mMetronome.setOmitCount(1);
		}
	}
	
	bufferToFill.buffer->applyGain(bufferToFill.startSample, bufferToFill.numSamples, level);
	mMetronome.setTempo(MainComponent::mTempoSlider.getValue());
}

void MainComponent::releaseResources()
{
	
}

void MainComponent::timerCallback()
{
	updatePracticeTimerDisplay();
	
	if(!mPracticeTimer.getPracticeTimerState())
	{
		mPracticeTimerStart.setButtonText("Start");
		mPracticeTimerStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF35C521));
	}
}

void MainComponent::updatePracticeTimerDisplay()
{
	juce::String string = mPracticeTimer.getPracticeTimerAsFormattedString();
	mPracticeTimerDisplay.setText(string, juce::NotificationType::dontSendNotification);
}


void MainComponent::paint (juce::Graphics& g)
{
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	juce::Rectangle<int> bounds = getLocalBounds();
	
	juce::FlexBox metronomeFlexBox1;
	metronomeFlexBox1.flexDirection = juce::FlexBox::Direction::column;
	metronomeFlexBox1.items.add(juce::FlexItem(mChooseBox).withFlex(2));
	metronomeFlexBox1.items.add(juce::FlexItem(mOmitButton).withFlex(2));
	metronomeFlexBox1.items.add(juce::FlexItem(mPlayButton).withFlex(4));
	
	juce::FlexBox tempoSliderFlexBox;
	tempoSliderFlexBox.flexDirection = juce::FlexBox::Direction::column;
	tempoSliderFlexBox.items.add(juce::FlexItem(mTempoLabel).withFlex(1));
	tempoSliderFlexBox.items.add(juce::FlexItem(mTempoSlider).withFlex(7));
	
	juce::FlexBox volumeSliderFlexBox;
	volumeSliderFlexBox.flexDirection = juce::FlexBox::Direction::column;
	volumeSliderFlexBox.items.add(juce::FlexItem(mVolumeLabel).withFlex(1));
	volumeSliderFlexBox.items.add(juce::FlexItem(mVolumeSlider).withFlex(7));
	
	juce::FlexBox metronomeFlexBox2;
	metronomeFlexBox2.flexDirection = juce::FlexBox::Direction::row;
	metronomeFlexBox2.items.add(juce::FlexItem(metronomeFlexBox1).withFlex(1));
	metronomeFlexBox2.items.add(juce::FlexItem(tempoSliderFlexBox).withFlex(1));
	metronomeFlexBox2.items.add(juce::FlexItem(volumeSliderFlexBox).withFlex(1));
	
	juce::FlexBox metronomeFlexBox3;
	metronomeFlexBox3.flexDirection = juce::FlexBox::Direction::column;
	metronomeFlexBox3.items.add(juce::FlexItem(mMetronomeLabel).withFlex(2));
	metronomeFlexBox3.items.add(juce::FlexItem(metronomeFlexBox2).withFlex(8));
	
	juce::FlexBox timerFlexBox;
	timerFlexBox.flexDirection = juce::FlexBox::Direction::column;
	timerFlexBox.items.add(juce::FlexItem(mPracticeTimerLabel).withFlex(2));
	timerFlexBox.items.add(juce::FlexItem(mPracticeTimerChooser).withFlex(2));
	timerFlexBox.items.add(juce::FlexItem(mPracticeTimerDisplay).withFlex(3));
	timerFlexBox.items.add(juce::FlexItem(mPracticeTimerStart).withFlex(3));
	
	juce::FlexBox flexBox;
	flexBox.items.add(juce::FlexItem(metronomeFlexBox3).withFlex(3));
	flexBox.items.add(juce::FlexItem(timerFlexBox).withFlex(1));
	flexBox.performLayout(bounds);
	
}

