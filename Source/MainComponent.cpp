#include "MainComponent.h"

MainComponent::MainComponent()
{
	
	mChooseLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mChooseLabel);
	
	mChooseBox.addItem("Short White Noise Burst", 1);
	mChooseBox.addItem("Cowbell", 2);
	mChooseBox.setSelectedId(1);
	mChooseBox.onChange = [this]() { chooseSound(); };
	addAndMakeVisible(&mChooseBox);
	
	mPlayButton.setToggleState(false, juce::NotificationType::dontSendNotification);
	mPlayButton.setRadioGroupId(1);
	mPlayButton.onClick = [this]() { changeMetronomeState(); };
	addAndMakeVisible(&mPlayButton);
	
	mTempoSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	mTempoSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 60, 30);
	mTempoSlider.setRange(20, 300, 1);
	mTempoSlider.setValue(60);
	mTempoSlider.setNumDecimalPlacesToDisplay(0);
	mTempoSlider.setTextValueSuffix(" bpm");
	addAndMakeVisible(&mTempoSlider);
	
	mVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	mVolumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, false, 60, 30);
	mVolumeSlider.setSkewFactorFromMidPoint(0.5);
	mVolumeSlider.setRange(0, 1, 0.00001);
	mVolumeSlider.setValue(0.3);
	mVolumeSlider.setNumDecimalPlacesToDisplay(2);
	mVolumeSlider.addListener(&mMetronome);
	addAndMakeVisible(&mVolumeSlider);
	
	mCountdownLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mCountdownLabel);
	
	mCountdownChooser.addItem("5", 1);
	mCountdownChooser.addItem("10", 2);
	mCountdownChooser.addItem("15", 3);
	mCountdownChooser.addItem("20", 4);
	mCountdownChooser.addItem("25", 5);
	mCountdownChooser.addItem("30", 6);
	mCountdownChooser.setSelectedId(1);
	mCountdownChooser.onChange = [this]() { chooseCountdown(); };
	addAndMakeVisible(&mCountdownChooser);
	
	mCountdownDisplay.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(&mCountdownDisplay);
	
	mCountdownStart.onClick = [this]() { changeCountdownState(); };
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
}

void MainComponent::changeMetronomeState()
{
	if (mMetronomeState == MetronomeState::Stopped)
	{
		mMetronomeState = MetronomeState::Playing;
		mPlayButton.setButtonText("Stop");
		play();
	}
	else if (mMetronomeState == MetronomeState::Playing)
	{
		mMetronomeState = MetronomeState::Stopped;
		mPlayButton.setButtonText("Play");
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
		startCountdown();
	}
	else if (mCountdown.countdownEnabled == true)
	{
		mCountdown.countdownEnabled = false;
		mCountdownStart.setButtonText("Start");
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

	float level = mMetronome.getSliderLevel();
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
		mCountdownStart.setButtonText("Start");
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
	
	juce::FlexBox flexBox;
	juce::FlexBox chooseFlexBox;
	juce::FlexBox timerFlexBox;

	chooseFlexBox.flexDirection = juce::FlexBox::Direction::column;
	chooseFlexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight() * 0.2, mChooseLabel));
	chooseFlexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight() * 0.8, mChooseBox));
	
	timerFlexBox.flexDirection = juce::FlexBox::Direction::column;
	timerFlexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight() * 0.2, mCountdownLabel));
	timerFlexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight() * 0.2, mCountdownChooser));
	timerFlexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight() * 0.3, mCountdownDisplay));
	timerFlexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight() * 0.3, mCountdownStart));
	
	flexBox.items.add(juce::FlexItem(chooseFlexBox).withFlex(2.5));
	flexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight(), mPlayButton));
	//flexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight(), mStopButton));
	flexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight(), mTempoSlider));
	flexBox.items.add(juce::FlexItem(getWidth() / mHorizontalItemCount, getHeight(), mVolumeSlider));
	flexBox.items.add(juce::FlexItem(timerFlexBox).withFlex(2.5));
	
	flexBox.performLayout(bounds);
	
}

