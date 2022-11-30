/*
  ==============================================================================

    XyPad.cpp
    Created: 30 Nov 2022 8:00:47am
    Author:  Stefan Hopman

  ==============================================================================
*/

#include "XyPad.h"

namespace GUI
{

    // XY Pad Thumb Section
    XyPad::Thumb::Thumb()
    {
        
    }

    void XyPad::Thumb::paint(juce::Graphics &g)
    {
        g.setColour(juce::Colours::white);
        g.fillEllipse(getLocalBounds().toFloat());
    
    }

    // XY Pad Section
    XyPad::XyPad()
    {
        addAndMakeVisible(thumb);
    }

    void XyPad::resized()
    {
        thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize, thumbSize));
    }
    
    void XyPad::paint(juce::Graphics &g)
    {
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
    }



}
