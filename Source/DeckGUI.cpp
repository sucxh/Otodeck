/*
  ==============================================================================

    DeckGUI.cpp
    Created: 27 Jan 2021 10:38:55pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI( DJAudioPlayer* _player,
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse
   
            ): player(_player),
               waveformDisplay(formatManagerToUse,cacheToUse)
               
               
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(forwardButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(restartButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(volLabel);

    addAndMakeVisible(waveformDisplay);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    playButton.addListener(this);
    pauseButton.addListener(this);
    loadButton.addListener(this);
    forwardButton.addListener(this);
    rewindButton.addListener(this);
    restartButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    
    
   

    startTimer(500);
    //vol slider settings
    volSlider.setRange(0.0, 1.0);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        40,
        volSlider.getTextBoxHeight());
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setValue(0.5);
    volSlider.setSkewFactorFromMidPoint(0.5);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);


    //pos slider settings
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        40,
        posSlider.getTextBoxHeight());
    posSlider.setNumDecimalPlacesToDisplay(2);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);


    //speed slider settings
    speedSlider.setRange(0.5, 5.0);
    speedSlider.setValue(1);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                                false,
                                40,
                                speedSlider.getTextBoxHeight()
    );
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedSlider.setSkewFactorFromMidPoint(1);
    speedLabel.attachToComponent(&speedSlider, true);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{


    double rowH = getHeight() / 8;
    double rowW = getWidth() / 9;
    double rowWAlign = getWidth() - rowW;
    playButton.setBounds(0, 0, getWidth()/3, rowH);
    pauseButton.setBounds(getWidth()/3, 0, getWidth()/3, rowH);
    restartButton.setBounds(2*getWidth() / 3, 0, getWidth() / 3, rowH);
    waveformDisplay.setBounds(0, rowH, getWidth(), rowH * 2);
    volSlider.setBounds(rowW, rowH * 3, rowWAlign, rowH);
    speedSlider.setBounds(rowW, rowH * 4, rowWAlign, rowH);
    posSlider.setBounds(rowW, rowH * 5, rowWAlign, rowH);
    forwardButton.setBounds(0, rowH * 6, rowWAlign/2, rowH);
    rewindButton.setBounds(getWidth()/2, rowH * 6, rowWAlign/2, rowH);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
    

}
void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton) {
        DBG("Play Button was clicked");

        player->start();
        auto buttonArea= playButton.getLocalBounds();
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
        pauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
    }   
    if (button == &pauseButton) {
        DBG("Stop Button was clicked");
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
        pauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
        player->stop();
    }
    if (button == &restartButton) {
        DBG("Restart Button was Clicked");
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
        pauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
        player->restart();

    }
    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
        }
        

    }
    if (button == &forwardButton)
    {

            posSlider.setValue(player->getPositionRelative() + 0.005);
       
    }
    if (button == &rewindButton)
    {
        posSlider.setValue(player->getPositionRelative() - 0.005);
    }


}
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider==&volSlider) 
    {
        player->setGain(slider->getValue());


    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());

    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());

    }
}
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) 
{
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{juce::File{files[0]}});
        waveformDisplay.loadURL(juce::URL{ juce::File{files[0]} });
        
    }
    
}
//load File audioURL
void DeckGUI::loadFile(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);

}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
             player->getPositionRelative());
}