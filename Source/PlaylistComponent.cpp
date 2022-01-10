/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 20 Feb 2021 10:40:23pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>
#include "DeckGUI.h"
#include <algorithm>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _djAudioPlayer
                                    ):deckGUI1(_deckGUI1),
                                      deckGUI2(_deckGUI2),
                                      djAudioPlayer(_djAudioPlayer)
                                      
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    

    playlist.getHeader().addColumn("Track title",1,400);
    playlist.getHeader().addColumn("Length", 2, 200);
    playlist.getHeader().addColumn("", 3, 200);
    playlist.setModel(this);

    loadPlaylist();

    addAndMakeVisible(playlist);
    addAndMakeVisible(importBtn);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(loadToDeck1Btn);
    addAndMakeVisible(loadToDeck2Btn);



    importBtn.addListener(this);
    loadToDeck2Btn.addListener(this);
    loadToDeck1Btn.addListener(this);

    searchBox.addListener(this);
    searchBox.setTextToShowWhenEmpty("Type to Search tracks", juce::Colours::orange);
    //textbox onTextChange
    searchBox.onTextChange = [this] { searchPlaylist(searchBox.getText()); };
}

PlaylistComponent::~PlaylistComponent()
{
    savePlaylist();
}

void PlaylistComponent::paint (juce::Graphics& g)
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
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    playlist.setBounds(0, 0, getWidth(), 3*getHeight()/4);
    loadToDeck1Btn.setBounds(0,14*getHeight() / 16, getWidth() / 2, getHeight() / 10);
    loadToDeck2Btn.setBounds(getWidth()/2,14*getHeight() / 16, getWidth() / 2, getHeight() / 10);
    searchBox.setBounds(3*getWidth()/4, 0, getWidth()/4, getHeight() / 11);
    importBtn.setBounds(0, 18* getHeight() / 23, getWidth() / 4, getHeight() / 15);

}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }

}
void PlaylistComponent::paintCell(juce::Graphics& g,
                                int rowNumber,
                                int columnId,
                                int width,
                                int height,
                                bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].trackTitle,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }

}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Delete" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);


            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

//Function to import tracks to playlist
void PlaylistComponent::importToPlaylist()
{
    DBG("PlaylistComponent::importToPlaylist called");

    //initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if not already loaded
            {
                Track importedTrack{ file };
                juce::URL audioURL{ file };
                importedTrack.length = getLength(audioURL);
                tracks.push_back(importedTrack);
                DBG("loaded file: " << importedTrack.trackTitle);
            }
            else // display info message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "Load details:",
                    fileNameWithoutExtension + " is already in the playlist",
                    "OK",
                    nullptr
                );
            }
        }
    }
}


juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    djAudioPlayer->loadURL(audioURL);
    double seconds{ djAudioPlayer->getLengthInSeconds() };
    juce::String minutes{ secToMin(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secToMin(int seconds)
{
    //Minutes in string
    juce::String minStr{ std::to_string(seconds / 60) };
    //Seconds in string
    juce::String secStr{ std::to_string(seconds % 60) };
    //return minutes and seconds in a single string
    return juce::String{ minStr + ":" + secStr };
}
bool PlaylistComponent::isInTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}



void PlaylistComponent::buttonClicked(juce::Button* button)
{
    //int id = std::stoi(button->getComponentID().toStdString());
    //DBG("PlaylistComponent::buttonClicked" +trackTitles[id]);
    if (button == &importBtn)
    {
        DBG("Load button was clicked");
        importToPlaylist();
        playlist.updateContent();
    }
    else if (button == &loadToDeck1Btn)
    {
        DBG("Load to deck 1 button was clicked");
        loadInDeck(deckGUI1);
    }
    else if (button == &loadToDeck2Btn)
    {

        DBG("Load to deck 2 button was clicked");
        loadInDeck(deckGUI2);

    }

    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(tracks[id].trackTitle + " removed from Library");
        //delete
        deleteFromTracks(id);
        playlist.updateContent();
    }
}
void PlaylistComponent::savePlaylist()
{
    // create .csv to save library
    std::ofstream playlist("playlist.txt");

    // save library to file
    for (Track& t : tracks)
    {
        playlist << t.file.getFullPathName() << "," << t.length << "\n";
    }
}
void PlaylistComponent::loadPlaylist()
{
    //reading from saved playlist.txt
    std::ifstream playlist("playlist.txt");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (playlist.is_open())
    {
        while (getline(playlist, filePath, ',')) {
            juce::File file{ filePath };
            Track newTrack{ file };

            getline(playlist, length);
            newTrack.length = length;
            tracks.push_back(newTrack);
        }
    }
    playlist.close();
}
void PlaylistComponent::searchPlaylist(juce::String keyword)
{
    DBG("Searching library for: " << keyword);
    if (keyword != "")
    {
        int rowNumber = findTrackTitle(keyword);
        //highlight track matching keyword
        playlist.selectRow(rowNumber);
    }
    else
    {
        playlist.deselectAllRows();
    }
}
int PlaylistComponent::findTrackTitle(juce::String keyword)
{
    // finds index where track title contains the keyword search
    auto it = find_if(tracks.begin(), tracks.end(),
        [&keyword](const Track& obj) {return obj.trackTitle.contains(keyword); });
    int i = -1;

    if (it != tracks.end())
    {
        DBG("Found Track!");
        i = std::distance(tracks.begin(), it);
        DBG(i);
    }

    return i;
}
void PlaylistComponent::deleteFromTracks(int id)
{
    tracks.erase(tracks.begin() + id);
}

//Function to load tracks to deck
void PlaylistComponent::loadInDeck(DeckGUI* deckGUI)
{
    int selectedRow{ playlist.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << tracks[selectedRow].trackTitle << " to Deck");
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Error loading to deck:",
            "No Track Selected",
            "OK",
            nullptr
        );
    }
}