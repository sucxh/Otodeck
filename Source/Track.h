/*
  ==============================================================================

    Track.h
    Created: 3 Mar 2021 4:23:55pm
    Author:  User

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Track
{
public:
    Track(juce::File _file);
    juce::File file;
    juce::URL URL;
    juce::String trackTitle;
    juce::String length;
    bool operator==(const juce::String& other) const;
};