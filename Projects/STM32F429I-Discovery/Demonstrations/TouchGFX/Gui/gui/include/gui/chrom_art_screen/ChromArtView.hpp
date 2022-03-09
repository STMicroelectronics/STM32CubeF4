/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  


#ifndef CHROM_ART_VIEW_HPP
#define CHROM_ART_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/chrom_art_screen/ChromArtPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/mixins/Draggable.hpp>

class ChromArtView : public View<ChromArtPresenter>
{
public:
    ChromArtView();
    virtual ~ChromArtView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void updateMCULoad(uint8_t mcuLoad);
private:
    static const uint8_t CLOUD_SPACING = 100;

    enum States
    {
        ANIMATE_STARTUP,
        ANIMATE_TO_NEXT_SETUP,
        NO_ANIMATION
    } currentState;

    uint32_t animationCounter;
    uint32_t tickCounter;

    Container viewPort;

    Image background;
    Button exitButton;

    TextArea mcuLoadHeadline;
    TextAreaWithOneWildcard mcuLoadTxt;
    Unicode::UnicodeChar mcuLoadTxtbuf[5];

    Image tree;
    Draggable<Image> stLogo;
    Draggable<Image> touchGFXLogo;
    ToggleButton chromArtOnOffButton;

    Image clouds0;
    Image clouds1;

    Button nextButton;

    // The element that the next step animation should
    // use as previous element when inserting elements
    Drawable* nextInsertElement;

    Callback<ChromArtView, const AbstractButton&> onButtonPressed;
    void buttonPressedhandler(const AbstractButton& button);

    void animateStartUp();
    void animateToNextSetup();
    void animateClouds();
};

#endif // CHROM_ART_VIEW_HPP
