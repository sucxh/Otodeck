/*
  ==============================================================================

    DeckGUI.h
    Created: 27 Jan 2021 10:38:55pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveFormDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
public:
    DeckGUI( DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* ) override;

    void sliderValueChanged(juce::Slider* slider)override;

    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;
    void loadFile(juce::URL audioURL);
private:


    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton loadButton{ "Load" };
    juce::TextButton forwardButton{"Fast Forward>>"};
    juce::TextButton rewindButton{ "Rewind" };
    juce::TextButton restartButton{ "Restart" };
    
    

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Label speedLabel;
    juce::Label posLabel;
    juce::Label volLabel;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

   


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
