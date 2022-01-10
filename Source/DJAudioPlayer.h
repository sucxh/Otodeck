/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 25 Jan 2021 1:39:31pm
    Author:  User

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer:public juce::AudioSource {
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
     
    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioUrl);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();
    void restart();

    /**get relative position of the playhead */
    double getPositionRelative();
    double getLengthInSeconds();

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource,false,2};

};