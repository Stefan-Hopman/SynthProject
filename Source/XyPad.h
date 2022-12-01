/*
  ==============================================================================

    XyPad.h
    Created: 30 Nov 2022 8:00:42am
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
namespace GUI
{
    class XyPad : public juce::Component
    {
    public:
        enum class Axis { X, Y };
        
        class Thumb : public juce::Component
        {
        public:
            Thumb();
            void paint(juce::Graphics& g) override;
            void mouseDown(const juce::MouseEvent& event) override;
            void mouseDrag(const juce::MouseEvent& event) override;
        private:
            juce::ComponentDragger dragger;
            juce::ComponentBoundsConstrainer constrainer;
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb);
        };
        XyPad();
        void resized() override;
        void paint(juce::Graphics& g) override;
        void registerSlider(juce::Slider* slider, Axis axis);
        void deregisterSlider(juce::Slider* slider);
    private:
        std::vector<juce::Slider*> xSliders, ySliders;
        Thumb thumb;
        std::mutex vectorMutex;
        static constexpr int thumbSize = 30;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XyPad);
    
    };
}
