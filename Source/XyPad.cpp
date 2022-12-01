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
        if (moveCallback)
        {
            moveCallback(getPosition().toDouble());
        }
    }

    // XY Pad Section
    XyPad::XyPad()
    {
        addAndMakeVisible(thumb);
        thumb.moveCallback = [&](juce::Point<double> position)
        {
            const std::lock_guard<std::mutex> lock(vectorMutex);
            const auto bounds = getLocalBounds().toDouble();
            const auto w = static_cast<double>(thumbSize);
            for (auto* slider : xSliders)
            {
                slider->setValue(juce::jmap(position.getX(), 0.0, bounds.getWidth() - w, slider->getMinimum(), slider->getMaximum()));
            }
            for (auto* slider: ySliders)
            {
                slider->setValue(juce::jmap(position.getY(), bounds.getWidth() - w,  0.0, slider->getMinimum(), slider->getMinimum()));
            }
            repaint();
            
        };
    }

    void XyPad::resized()
    {
        thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize, thumbSize));
        if (!xSliders.empty())
            sliderValueChanged(xSliders[0]);
        if (!ySliders.empty())
            sliderValueChanged(ySliders[0]);
    }
    
    void XyPad::paint(juce::Graphics &g)
    {
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
    }

    void XyPad::registerSlider(juce::Slider* slider, Axis axis)
    {
        slider->addListener(this);
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
        slider->removeListener(this);
        // Lock
        const std::lock_guard<std::mutex> lock(vectorMutex);
        // remove/erease idion
        xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
        ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
        
    }

    void XyPad::sliderValueChanged(juce::Slider* slider)
    {
        // Avoid loop back
        if (thumb.isMouseOverOrDragging(false))
        {
            return;
        }
        // Figure out if the slider belongs to xSliders or ySliders
        const auto isXAxisSlider = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
        const auto bounds = getLocalBounds().toDouble();
        const auto w = static_cast<double>(thumbSize);
        if (isXAxisSlider)
        {
            thumb.setTopLeftPosition(juce::jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getWidth() - w), thumb.getY());
        }
        else
        {
            thumb.setTopLeftPosition(thumb.getX(), juce::jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), bounds.getHeight() - w, 0.0));
        }
        repaint();
        
        
    }



};
