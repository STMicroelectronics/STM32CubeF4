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
  


#include <gui/chrom_art_screen/ChromArtView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/FrontendApplication.hpp>

ChromArtView::ChromArtView() :
    currentState(ANIMATE_STARTUP),
    animationCounter(0),
    tickCounter(0),
    onButtonPressed(this, &ChromArtView::buttonPressedhandler)
{
}

ChromArtView::~ChromArtView()
{
}

void ChromArtView::setupScreen()
{
    viewPort.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);

    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_CHROM_ART_SUN_ID));

    exitButton.setBitmaps(Bitmap(BITMAP_ANIMATED_GRAPHICS_BACK_BUTTON_ID), Bitmap(BITMAP_ANIMATED_GRAPHICS_BACK_BUTTON_PRESSED_ID));
    exitButton.setXY(0, background.getHeight());
    exitButton.setAction(onButtonPressed);

    clouds0.setBitmap(Bitmap(BITMAP_CHROM_ART_CLOUDS_ID));
    clouds0.setXY(0, 20);
    clouds0.setAlpha(0);

    clouds1.setBitmap(Bitmap(BITMAP_CHROM_ART_CLOUDS_ID));
    clouds1.setXY(clouds0.getWidth() + CLOUD_SPACING, 20);
    clouds1.setAlpha(0);

    tree.setBitmap(Bitmap(BITMAP_CHROM_ART_BACKGROUND_ID));
    tree.setXY(0, background.getHeight() - tree.getHeight());
    tree.setAlpha(0);

    chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_CHROM_ART_ON_BUTTON_ID), Bitmap(BITMAP_CHROM_ART_OFF_BUTTON_ID));
    chromArtOnOffButton.setXY(16, background.getHeight() - chromArtOnOffButton.getHeight() - 10);
    chromArtOnOffButton.setAction(onButtonPressed);

    stLogo.setBitmap(Bitmap(BITMAP_ST_LOGO_ID));
    stLogo.setXY(120, 80);
    stLogo.setAlpha(0);

    touchGFXLogo.setBitmap(Bitmap(BITMAP_TOUCHGFX_LOGO_ID));
    touchGFXLogo.setXY(20, 80);
    touchGFXLogo.setAlpha(0);

    nextButton.setBitmaps(Bitmap(BITMAP_LAYER_BUTTON_ID), Bitmap(BITMAP_LAYER_BUTTON_PRESSED_ID));
    nextButton.setXY(viewPort.getWidth() - nextButton.getWidth() - 16, background.getHeight() - nextButton.getHeight() - 10);
    nextButton.setAction(onButtonPressed);

    mcuLoadHeadline.setTypedText(TypedText(T_MCU_LOAD_TEXT));
    mcuLoadHeadline.setPosition(HAL::DISPLAY_WIDTH - 56, HAL::DISPLAY_HEIGHT - 36, 50, 15);
    mcuLoadHeadline.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    Unicode::snprintf(mcuLoadTxtbuf, 5, "%d", 0);
    mcuLoadTxt.setWildcard(mcuLoadTxtbuf);
    mcuLoadTxt.setTypedText(TypedText(T_MCU_LOAD_VALUE));
    mcuLoadTxt.setPosition(HAL::DISPLAY_WIDTH - 56, HAL::DISPLAY_HEIGHT - 20, 50, 15);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    nextInsertElement = &tree;

    viewPort.add(background);
    viewPort.add(clouds0);
    viewPort.add(clouds1);
    viewPort.add(stLogo);
    viewPort.add(touchGFXLogo);
    viewPort.add(tree);
    viewPort.add(chromArtOnOffButton);
    viewPort.add(nextButton);

    add(viewPort);
    add(exitButton);
    add(mcuLoadHeadline);
    add(mcuLoadTxt);
}

#if defined(ST) && !defined(SIMULATOR)
extern volatile bool disableChromArt;
#endif

void ChromArtView::tearDownScreen()
{
#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = false;
#endif
}

void ChromArtView::handleTickEvent()
{
    tickCounter++;

    if (currentState == ANIMATE_TO_NEXT_SETUP)
    {
        animateToNextSetup();
    }
    else if (currentState == ANIMATE_STARTUP)
    {
        animateStartUp();
    }

    //   if (tickCounter % 2 == 0)
    {
        animateClouds();
    }
}

void ChromArtView::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &nextButton)
    {
        nextButton.setTouchable(false);

        currentState = ANIMATE_TO_NEXT_SETUP;
        animationCounter = 0;
    }
    else if (&button == &chromArtOnOffButton)
    {
#if defined(ST) && !defined(SIMULATOR)
        disableChromArt = !disableChromArt;
#endif
    }
    else if (&button == &exitButton)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMenuScreen();
    }
}


void ChromArtView::animateToNextSetup()
{
    uint8_t fadeDuration = 18;

    if (animationCounter < fadeDuration)
    {
        int16_t alpha = EasingEquations::quadEaseIn(animationCounter, 0, 255, fadeDuration - 1);
        stLogo.setAlpha((uint8_t)(255 - alpha));
        touchGFXLogo.setAlpha((uint8_t)(255 - alpha));
        stLogo.invalidate();
        touchGFXLogo.invalidate();
    }
    else if (animationCounter == fadeDuration)
    {
        viewPort.remove(stLogo);
        viewPort.insert(nextInsertElement, stLogo);

        viewPort.remove(touchGFXLogo);
        viewPort.insert(&stLogo, touchGFXLogo);
        viewPort.invalidate();

        // Prepare the next insertion
        if (nextInsertElement == &tree)
        {
            nextInsertElement = &background;
        }
        else if (nextInsertElement == &background)
        {
            nextInsertElement = &clouds1;
        }
        else
        {
            nextInsertElement = &tree;
        }
    }
    else if (animationCounter < (uint8_t)(fadeDuration * 2))
    {
        int16_t alpha = EasingEquations::quadEaseIn(animationCounter - fadeDuration, 0, 255, fadeDuration - 1);
        stLogo.setAlpha((uint8_t) alpha);
        touchGFXLogo.setAlpha((uint8_t) alpha);
        stLogo.invalidate();
        touchGFXLogo.invalidate();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        nextButton.setTouchable(true);
    }

    animationCounter++;
}

void ChromArtView::animateStartUp()
{
    uint8_t fadeDuration = 34;

    if (animationCounter < fadeDuration)
    {
        int16_t alpha = EasingEquations::quadEaseIn(animationCounter, 0, 255, fadeDuration - 1);
        clouds0.setAlpha((uint8_t) alpha);
        clouds1.setAlpha((uint8_t) alpha);
        clouds0.invalidate();
        clouds1.invalidate();
    }
    else if (animationCounter < (uint8_t)(fadeDuration * 2))
    {
        int16_t alpha = EasingEquations::quadEaseIn(animationCounter - fadeDuration, 0, 255, fadeDuration - 1);
        stLogo.setAlpha((uint8_t) alpha);
        stLogo.invalidate();
    }
    else if (animationCounter < (uint8_t)(fadeDuration * 3))
    {
        int16_t alpha = EasingEquations::quadEaseIn(animationCounter - (fadeDuration * 2), 0, 255, fadeDuration - 1);
        touchGFXLogo.setAlpha((uint8_t) alpha);
        touchGFXLogo.invalidate();
    }
    else if (animationCounter < (uint8_t)(fadeDuration * 4))
    {
        int16_t alpha = EasingEquations::quadEaseIn(animationCounter - (fadeDuration * 3), 0, 255, fadeDuration - 1);
        tree.setAlpha((uint8_t) alpha);
        tree.invalidate();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        nextButton.setTouchable(true);
    }

    animationCounter++;
}

void ChromArtView::animateClouds()
{
    clouds0.moveTo(clouds0.getX() - 1, clouds0.getY());
    clouds1.moveTo(clouds1.getX() - 1, clouds1.getY());

    if (clouds0.getX() + clouds0.getWidth() < 0)
    {
        clouds0.moveTo(clouds1.getX() + clouds1.getWidth() + CLOUD_SPACING, clouds0.getY());
    }
    else if (clouds1.getX() + clouds1.getWidth() < 0)
    {
        clouds1.moveTo(clouds0.getX() + clouds0.getWidth() + CLOUD_SPACING, clouds1.getY());
    }
}

void ChromArtView::updateMCULoad(uint8_t mcuLoad)
{
    Unicode::snprintf(mcuLoadTxtbuf, 5, "%d", mcuLoad);
    mcuLoadTxt.invalidate();
}
