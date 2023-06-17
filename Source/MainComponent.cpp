#include "MainComponent.h"

MainComponent::MainComponent()
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
	
	mCountdownLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mCountdownLabel);
	
	mMetronomeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mMetronomeLabel);
	
	mOmitButton.setToggleState(false, juce::NotificationType::dontSendNotification);
	mOmitButton.setRadioGroupId(1);
	mOmitButton.onClick = [this]() { omitClick(); };
	mOmitButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF573068));
	addAndMakeVisible(&mOmitButton);
	
	mCountdownChooser.addItem("5", 1);
	mCountdownChooser.addItem("10", 2);
	mCountdownChooser.addItem("15", 3);
	mCountdownChooser.addItem("20", 4);
	mCountdownChooser.addItem("25", 5);
	mCountdownChooser.addItem("30", 6);
	mCountdownChooser.addItem("1", 7);
	mCountdownChooser.setJustificationType(juce::Justification::centred);
	mCountdownChooser.setSelectedId(1);
	mCountdownChooser.onChange = [this]() { chooseCountdown(); };
	addAndMakeVisible(&mCountdownChooser);
	
	mCountdownDisplay.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mCountdownDisplay);
	
	mCountdownStart.onClick = [this]() { changeCountdownState(); };
	mCountdownStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFABC0A8));
	addAndMakeVisible(&mCountdownStart);
	
	Timer::startTimerHz(60);
	
    setSize (400, 300);

    // Some platforms require permissions to open input channels so request that here
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
	if (mMetronomeState == MetronomeState::Stopped)
	{
		mMetronomeState = MetronomeState::Playing;
		mPlayButton.setButtonText("Stop");
		mPlayButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFC52121));
		play();
	}
	else if (mMetronomeState == MetronomeState::Playing)
	{
		mMetronomeState = MetronomeState::Stopped;
		mPlayButton.setButtonText("Play");
		mPlayButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF35C521));
		stop();
	}
}

void MainComponent::chooseSound()
{
	int id = mChooseBox.getSelectedId();
	
	if (id == 1)
		mMetronome.setFileToPlay("HIHAT.wav");
	else if (id == 2)
		mMetronome.setFileToPlay("COWBELL.wav");
	
}

void MainComponent::omitClick()
{
	if (omitStatus == false)
	{
		omitStatus = true;
		mMetronome.setOmitCount(0);
		mOmitButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF003068));
	}
	else if (omitStatus == true)
	{
		omitStatus = false;
		mOmitButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF573068));
		
	}
}

void MainComponent::chooseCountdown()
{
	mCountdown.resetCountdown();
	
	mCountdown.countdownEnabled = false;	//these two lines of code are not very elegant. This related to stopCountdown --> chooseCountdown
	mCountdownStart.setButtonText("Start");		//
	
	int id = mCountdownChooser.getSelectedId();
	if (id == 1)
		mCountdown.setCountdown(5);
	if (id == 2)
		mCountdown.setCountdown(10);
	if (id == 3)
		mCountdown.setCountdown(15);
	if (id == 4)
		mCountdown.setCountdown(20);
	if (id == 5)
		mCountdown.setCountdown(25);
	if (id == 6)
		mCountdown.setCountdown(30);
	if (id == 7)
		mCountdown.setCountdown(1);
}

void MainComponent::startCountdown()
{
	mCountdown.startTimer(1000);
	mCountdown.countdownEnabled = true;
}

void MainComponent::stopCountdown()
{
	mCountdown.countdownEnabled = false;
	chooseCountdown();
}

void MainComponent::changeCountdownState()
{
	if (mCountdown.countdownEnabled == false)
	{
		mCountdown.countdownEnabled = true;
		mCountdownStart.setButtonText("Stop");
		mCountdownStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFC52121));
		startCountdown();
	}
	else if (mCountdown.countdownEnabled == true)
	{
		mCountdown.countdownEnabled = false;
		mCountdownStart.setButtonText("Start");
		mCountdownStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFFC52121));
		stopCountdown();
	}
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	mMetronome.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

	if (mMetronomeState == MetronomeState::Playing)
		mMetronome.calcWhenPlayFile(bufferToFill);

	float level = juce::Decibels::decibelsToGain(mMetronome.getSliderLevel());
	
	if (omitStatus && mMetronome.getOmitCount() > 4)
	{
		level = 0;
		if (mMetronome.getOmitCount() == 9)
			mMetronome.setOmitCount(1);
	}
	bufferToFill.buffer->applyGain(bufferToFill.startSample, bufferToFill.numSamples, level);
	mMetronome.setTempo(MainComponent::mTempoSlider.getValue());
}

void MainComponent::releaseResources()
{

}

//==============================================================================

void MainComponent::timerCallback()
{
	updateCountdownDisplay();
	
	if(!mCountdown.countdownEnabled)
	{
		mCountdownStart.setButtonText("Start");
		mCountdownStart.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xFF35C521));
	}
}

void MainComponent::updateCountdownDisplay()
{
	juce::String string = mCountdown.getCountdownAsFormattedString();
	mCountdownDisplay.setText(string, juce::NotificationType::dontSendNotification);
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
	timerFlexBox.items.add(juce::FlexItem(mCountdownLabel).withFlex(2));
	timerFlexBox.items.add(juce::FlexItem(mCountdownChooser).withFlex(2));
	timerFlexBox.items.add(juce::FlexItem(mCountdownDisplay).withFlex(3));
	timerFlexBox.items.add(juce::FlexItem(mCountdownStart).withFlex(3));
	
	juce::FlexBox flexBox;
	flexBox.items.add(juce::FlexItem(metronomeFlexBox3).withFlex(3));
	flexBox.items.add(juce::FlexItem(timerFlexBox).withFlex(1));
	flexBox.performLayout(bounds);
	
}

