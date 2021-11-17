#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Viewer.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

//==============================================================================
/**
*/
class NonlinearBiquadAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    NonlinearBiquadAudioProcessorEditor (NonlinearBiquadAudioProcessor&);
    ~NonlinearBiquadAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    NonlinearBiquadAudioProcessor& processor;
    AudioProcessorValueTreeState& vts;
    enum {
        editorWidth = 500,
        editorMargin = 10,
        editorPadding = 10,

        sliderTextEntryBoxWidth = 100,
        sliderTextEntryBoxHeight = 25,
        sliderHeight = 25,
        buttonHeight = 25,
        comboBoxHeight = 25,
        labelWidth = 100,
    };

    //======================================

    OwnedArray<Slider> sliders;
    OwnedArray<ToggleButton> toggles;
    OwnedArray<ComboBox> comboBoxes;

    OwnedArray<Label> labels;
    Array<Component*> components;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

    OwnedArray<SliderAttachment> sliderAttachments;
    OwnedArray<ButtonAttachment> buttonAttachments;
    OwnedArray<ComboBoxAttachment> comboBoxAttachments;

    //==============================================================================


    Viewer viewer;

    ComboBox shapeBox;
    std::unique_ptr<ComboBoxAttachment> shapeBoxAttach;
    const StringArray shapeChoices = StringArray ({ "Bell", "Notch", "HShelf", "LShelf", "HPF", "LPF" });

    Slider freqSlide;
    std::unique_ptr<SliderAttachment> freqAttach;

    Slider qSlide;
    std::unique_ptr<SliderAttachment> qAttach;

    Slider gainSlide;
    std::unique_ptr<SliderAttachment> gainAttach;

    Slider driveSlide;
    std::unique_ptr<SliderAttachment> driveAttach;

    ComboBox satBox;
    std::unique_ptr<ComboBoxAttachment> satBoxAttach;
    const StringArray satChoices = StringArray ({"None", "Hard", "Soft", "Tanh" });

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonlinearBiquadAudioProcessorEditor)
};
