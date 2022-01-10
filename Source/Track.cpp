/*
  ==============================================================================

    Track.cpp
    Created: 3 Mar 2021 4:23:55pm
    Author:  User

  ==============================================================================
*/

#include "Track.h"
#include <filesystem>

Track::Track(juce::File _file) : file(_file),
trackTitle(_file.getFileNameWithoutExtension()),
URL(juce::URL{ _file })
{
    DBG("Created new track with title: " << trackTitle);
}

bool Track::operator==(const juce::String& other) const
{
    return trackTitle == other;
}
