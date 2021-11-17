/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NonlinearBiquadAudioProcessorEditor::NonlinearBiquadAudioProcessorEditor (NonlinearBiquadAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    vts (p.getVTS()),
    viewer (vts)
{
    const Array<AudioProcessorParameter*> parameters = processor.getParameters();
    int comboBoxCounter = 0;

    int editorHeight = 2 * editorMargin;
    for (int i = 0; i < parameters.size(); ++i) {
        if (const AudioProcessorParameterWithID* parameter =
                dynamic_cast<AudioProcessorParameterWithID*> (parameters[i])) {

            if (processor.parameters.parameterTypes[i] == "Slider") {
                Slider* aSlider;
                sliders.add (aSlider = new Slider());
                aSlider->setTextValueSuffix (parameter->label);
                aSlider->setTextBoxStyle (Slider::TextBoxLeft,
                                          false,
                                          sliderTextEntryBoxWidth,
                                          sliderTextEntryBoxHeight);

                SliderAttachment* aSliderAttachment;
                sliderAttachments.add (aSliderAttachment =
                    new SliderAttachment (processor.parameters.apvts, parameter->paramID, *aSlider));

                components.add (aSlider);
                editorHeight += sliderHeight;
            }

            //======================================

            else if (processor.parameters.parameterTypes[i] == "ToggleButton") {
                ToggleButton* aButton;
                toggles.add (aButton = new ToggleButton());
                aButton->setToggleState (parameter->getDefaultValue(), dontSendNotification);

                ButtonAttachment* aButtonAttachment;
                buttonAttachments.add (aButtonAttachment =
                    new ButtonAttachment (processor.parameters.apvts, parameter->paramID, *aButton));

                components.add (aButton);
                editorHeight += buttonHeight;
            }

            //======================================

            else if (processor.parameters.parameterTypes[i] == "ComboBox") {
                ComboBox* aComboBox;
                comboBoxes.add (aComboBox = new ComboBox());
                aComboBox->setEditableText (false);
                aComboBox->setJustificationType (Justification::left);
                aComboBox->addItemList (processor.parameters.comboBoxItemLists[comboBoxCounter++], 1);

                ComboBoxAttachment* aComboBoxAttachment;
                comboBoxAttachments.add (aComboBoxAttachment =
                    new ComboBoxAttachment (processor.parameters.apvts, parameter->paramID, *aComboBox));

                components.add (aComboBox);
                editorHeight += comboBoxHeight;
            }

            //======================================

            Label* aLabel;
            labels.add (aLabel = new Label (parameter->name, parameter->name));
            aLabel->attachToComponent (components.getLast(), true);
            addAndMakeVisible (aLabel);

            components.getLast()->setName (parameter->name);
            components.getLast()->setComponentID (parameter->paramID);
            addAndMakeVisible (components.getLast());
        }
    }

    //======================================

    editorHeight += components.size() * editorPadding + 500;
    editorWidth = (editorWidth + 400 + editorPadding*2)/2
    setSize (editorWidth, editorHeight);
    addAndMakeVisible (viewer);

    auto setupBox = [this] (ComboBox& box, AudioProcessorValueTreeState& vts, String paramID,
        std::unique_ptr<ComboBoxAttachment>& attachment, StringArray choices,
        std::function<void()> onChange = {})
    {
        addAndMakeVisible (box);
        box.addItemList (choices, 1);
        box.setSelectedItemIndex (0);
        box.onChange = onChange;

        attachment.reset (new ComboBoxAttachment (vts, paramID, box));
    };

    setupBox (shapeBox, vts, "shape", shapeBoxAttach, shapeChoices, [this] { viewer.setNeedsCurveUpdate (true); });
    setupBox (satBox,   vts, "sat",   satBoxAttach,   satChoices,   [this] { viewer.setNeedsCurveUpdate (true); });

    auto setupSlider = [this] (Slider& slider, AudioProcessorValueTreeState& vts, String paramID,
        std::unique_ptr<SliderAttachment>& attachment, String name = {},
        std::function<void()> onValueChange = {}, std::function<String (double)> textFromValue = {},
        std::function<double (String)> valueFromText = {})
    {
        addAndMakeVisible (slider);
        attachment.reset (new SliderAttachment (vts, paramID, slider));

        slider.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
        slider.setName (name);
        slider.textFromValueFunction = textFromValue;
        slider.valueFromTextFunction = valueFromText;
        slider.setNumDecimalPlacesToDisplay (2);
        slider.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
        slider.setColour (Slider::textBoxOutlineColourId, Colours::transparentBlack);
        slider.onValueChange = onValueChange;
    };

    setupSlider (freqSlide,  vts, "freq",      freqAttach,  "Freq",  [this] { viewer.setNeedsCurveUpdate (true); });
    setupSlider (qSlide,     vts, "q",         qAttach,     "Q",     [this] { viewer.setNeedsCurveUpdate (true); });
    setupSlider (gainSlide,  vts, "gain",      gainAttach,  "Gain",  [this] { viewer.setNeedsCurveUpdate (true); });
    setupSlider (driveSlide, vts, "drivegain", driveAttach, "Drive", [this] { viewer.setNeedsCurveUpdate (true); });

    freqSlide.setTextValueSuffix (" Hz");
    freqSlide.setNumDecimalPlacesToDisplay (0);

    qSlide.setSkewFactorFromMidPoint (0.707f);

    gainSlide.setTextValueSuffix (" dB");
    driveSlide.setTextValueSuffix (" dB");
}

NonlinearBiquadAudioProcessorEditor::~NonlinearBiquadAudioProcessorEditor()
{
}

//==============================================================================
void NonlinearBiquadAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    auto makeName = [this, &g] (Component& comp, String name)
    {
        const int height = 20;
        Rectangle<int> nameBox (comp.getX(), 402, comp.getWidth(), height);
        g.drawFittedText (name, nameBox, Justification::centred, 1);
    };

    makeName (shapeBox,   "Shape");
    makeName (freqSlide,  "Frequency");
    makeName (qSlide,     "Q");
    makeName (gainSlide,  "Gain");
    makeName (driveSlide, "Drive");
    makeName (satBox,     "Saturator");
}

void NonlinearBiquadAudioProcessorEditor::resized()
{
    viewer.setBounds (0, 0, getWidth(), getWidth());

    shapeBox.setBounds     (5,                          440, 70, 20);
    freqSlide.setBounds    (shapeBox.getRight()   - 05, 415, 80, 80);
    qSlide.setBounds       (freqSlide.getRight()  - 20, 415, 80, 80);
    gainSlide.setBounds    (qSlide.getRight()     - 20, 415, 80, 80);
    driveSlide.setBounds   (gainSlide.getRight()  - 20, 415, 80, 80);
    satBox.setBounds       (driveSlide.getRight() - 05, 440, 70, 20);
  Rectangle<int> r = getLocalBounds().reduced (editorMargin);
    r = r.removeFromRight (r.getWidth() - labelWidth);

    for (int i = 0; i < components.size(); ++i) {
        if (Slider* aSlider = dynamic_cast<Slider*> (components[i]))
            components[i]->setBounds (r.removeFromTop (sliderHeight));

        if (ToggleButton* aButton = dynamic_cast<ToggleButton*> (components[i]))
            components[i]->setBounds (r.removeFromTop (buttonHeight));

        if (ComboBox* aComboBox = dynamic_cast<ComboBox*> (components[i]))
            components[i]->setBounds (r.removeFromTop (comboBoxHeight));

        r = r.removeFromBottom (r.getHeight() - editorPadding);
    }
}
