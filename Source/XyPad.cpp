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
        constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
        
    }

    void XyPad::Thumb::paint(juce::Graphics &g)
    {
        g.setColour(juce::Colours::white);
        g.fillEllipse(getLocalBounds().toFloat());
    }
    
    void XyPad::Thumb::mouseDown(const juce::MouseEvent& event)
    {
        dragger.startDraggingComponent(this, event);
    }
    
    void XyPad::Thumb::mouseDrag(const juce::MouseEvent& event)
    {
        dragger.dragComponent(this, event, &constrainer);
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

    void XyPad::registerSlider(juce::Slider* slider, Axis axis)
    {
        const std::lock_guard<std::mutex> lock(vectorMutex);
        if (axis == Axis::X)
        {
            xSliders.push_back(slider);
        }
        if (axis == Axis::Y)
        {
            ySliders.push_back(slider);
        }
    }

    void XyPad::deregisterSlider(juce::Slider* slider)
    {
        // Lock
        const std::lock_guard<std::mutex> lock(vectorMutex);
        // remove/erease idion
        xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
        ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
        
    }


};
