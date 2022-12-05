/*
  ==============================================================================

    PresetPannel.h
    Created: 20 Nov 2022 7:20:34pm
    Author:  Stefan Hopman

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PresetManager.h"

namespace GUI
{
    class PresetPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
    {
    public:
        PresetPanel(Service::PresetManager& pm) : presetManager(pm)
        {
            configureButton(saveButton, "Save");
            configureButton(deletePresetButton, "Delete");
            configureButton(previousPresetButton, "<");
            configureButton(nextPresetButton, ">");
            
            presetList.setTextWhenNothingSelected("No Preset Selected");
            presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(presetList);
            presetList.addListener(this);
            
            loadPresetList();
            
        }
        ~PresetPanel()
        {
            saveButton.removeListener(this);
            deletePresetButton.removeListener(this);
            previousPresetButton.removeListener(this);
            nextPresetButton.removeListener(this);
            presetList.removeListener(this);
        }
        
        void resized() override
        {
            const auto container = getLocalBounds().reduced(4);
            auto bounds = container;
            
            saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
            previousPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
            presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
            nextPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
            deletePresetButton.setBounds(bounds.reduced(4));
        }
        void paint(juce::Graphics& g) override
        {
            
            
        }
        
    private:
        void buttonClicked(juce::Button* button) override
        {
            if (button == &saveButton)
            {
                fileChooser = std::make_unique<juce::FileChooser>(
                    "Please enter the name of the preset to save",
                     Service::PresetManager::defaultDirectory,
                     "*." + Service::PresetManager::extension
                );
                fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
                {
                    const auto resultFile = chooser.getResult();
                    presetManager.savePreset(resultFile.getFileNameWithoutExtension());
                    loadPresetList();
                });
            }
            if (button == &previousPresetButton)
            {
                const auto index = presetManager.loadPreviousPreset();
                presetList.setSelectedItemIndex(index);
            }
            if (button == &nextPresetButton)
            {
                const auto index = presetManager.loadNextPreset();
                presetList.setSelectedItemIndex(index, juce::dontSendNotification);
            }
            if (button == &deletePresetButton)
            {
                presetManager.deletePreset(presetManager.getCurrentPreset());
            }
            
            
        }
        void loadPresetList()
        {
            presetList.clear(juce::dontSendNotification);
            const auto allPresets = presetManager.getAllPresets();
            const auto currentPreset = presetManager.getCurrentPreset();
            presetList.addItemList(allPresets, 1);
            presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
        }
        
        void configureButton(juce::Button& button, const juce::String& buttonText)
        {
            button.setButtonText(buttonText);
            button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(button);
            button.addListener(this);
        }
        
        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
        {
            if (comboBoxThatHasChanged == &presetList)
            {
                presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
            }
        }
        Service::PresetManager& presetManager;
        juce::TextButton saveButton, deletePresetButton, previousPresetButton, nextPresetButton;
        juce::ComboBox presetList;
        std::unique_ptr<juce::FileChooser> fileChooser;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
        
        
    };
};

