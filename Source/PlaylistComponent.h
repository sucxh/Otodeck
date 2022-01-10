/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Feb 2021 10:40:23pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "Track.h"
#include "DJAudioPlayer.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2,
                      DJAudioPlayer* _playerForParsingMetaData);
    ~PlaylistComponent() ;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    void paintCell(juce::Graphics &,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) override;

    juce::Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        juce::Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

    
    

private:
    std::vector<Track> tracks;
    juce::TextButton loadToDeck1Btn{ "Load in Deck 1" };
    juce::TextButton loadToDeck2Btn{ "Load in Deck 2" };
    juce::TextButton importBtn{ "Import Tracks" };
    juce::TextButton deleteButton{ "Delete" };
    juce::TableListBox playlist;
    juce::TextEditor searchBox;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* djAudioPlayer;


    juce::String getLength(juce::URL audioURL);
    juce::String secToMin(int seconds);
    

    void importToPlaylist();
    void savePlaylist();
    void loadPlaylist();
    void searchPlaylist(juce::String searchText);
    int findTrackTitle(juce::String searchText);
    void deleteFromTracks(int id);
    bool isInTracks(juce::String fileNameWithoutExtension);
    void loadInDeck(DeckGUI* deckGUI);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
