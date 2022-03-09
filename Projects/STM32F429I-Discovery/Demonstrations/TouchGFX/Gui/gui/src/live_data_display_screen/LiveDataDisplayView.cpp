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
  


#include <gui/live_data_display_screen/LiveDataDisplayView.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/EasingEquations.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <stdlib.h>


LiveDataDisplayView::LiveDataDisplayView() :
    currentState(NO_ANIMATION),
    animationCounter(0),
    tickCounter(0),
    dragX(0),
    infoScreens(EAST),
    onButtonPressed(this, &LiveDataDisplayView::buttonPressedhandler)
{
}

LiveDataDisplayView::~LiveDataDisplayView()
{
}

void LiveDataDisplayView::setupScreen()
{
    exitButton.setBitmaps(Bitmap(BITMAP_ANIMATED_GRAPHICS_BACK_BUTTON_ID), Bitmap(BITMAP_ANIMATED_GRAPHICS_BACK_BUTTON_PRESSED_ID));
    exitButton.setXY(0, HAL::DISPLAY_HEIGHT - exitButton.getHeight());
    exitButton.setAction(onButtonPressed);

    infoScreen[0].setBitmap(BITMAP_WEATHER_BACKGROUND_1_ID);
    infoScreen[0].setCity(CityInfo::COPENHAGEN);
    infoScreen[0].setTextColor(Color::getColorFrom24BitRGB(0xFA, 0xF0, 0xB4));
    infoScreen[1].setBitmap(BITMAP_WEATHER_BACKGROUND_2_ID);
    infoScreen[1].setCity(CityInfo::HONG_KONG);
    infoScreen[1].setTextColor(Color::getColorFrom24BitRGB(0x0A, 0x45, 0x064));
    infoScreen[2].setBitmap(BITMAP_WEATHER_BACKGROUND_3_ID);
    infoScreen[2].setCity(CityInfo::MUMBAI);
    infoScreen[2].setTextColor(Color::getColorFrom24BitRGB(0x27, 0x5F, 0x7D));
    infoScreen[3].setBitmap(BITMAP_WEATHER_BACKGROUND_1_ID);
    infoScreen[3].setCity(CityInfo::NEW_YORK);
    infoScreen[3].setTextColor(Color::getColorFrom24BitRGB(0xBA, 0xD2, 0xDA));

    infoScreens.setXY(0, 0);
    infoScreens.add(infoScreen[0]);
    infoScreens.add(infoScreen[1]);
    infoScreens.add(infoScreen[2]);
    infoScreens.add(infoScreen[3]);

    infoScreensViewPort.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT - exitButton.getHeight());
    infoScreensViewPort.add(infoScreens);

    currentScreen = 0;
    weekInfoBar.setPosition(0, exitButton.getY() - 87, HAL::DISPLAY_WIDTH, 60);
    weekInfoBar.setTextColor(infoScreen[currentScreen].getTextColor());
    weekInfoBar.setInfo(infoScreen[currentScreen].getCity());

    dotIndicator.setNumberOfDots(NUMBER_OF_SCREENS);
    dotIndicator.setBitmaps(Bitmap(BITMAP_WEATHER_DOT_NORMAL_ID), Bitmap(BITMAP_WEATHER_DOT_SELECTED_ID));
    dotIndicator.setXY(((HAL::DISPLAY_WIDTH - dotIndicator.getWidth()) / 2), exitButton.getY() - 20);

    backgroundSwipeAreaLeft.setBitmap(Bitmap(BITMAP_WEATHER_BACKGROUND_ENDING_1_ID));
    backgroundSwipeAreaLeft.setXY(0, infoScreensViewPort.getY());

    backgroundSwipeAreaRight.setBitmap(Bitmap(BITMAP_WEATHER_BACKGROUND_ENDING_1_ID));
    backgroundSwipeAreaRight.setXY(HAL::DISPLAY_WIDTH - backgroundSwipeAreaRight.getWidth(), infoScreensViewPort.getY());

    add(backgroundSwipeAreaLeft);
    add(backgroundSwipeAreaRight);
    add(infoScreensViewPort);
    add(weekInfoBar);
    add(dotIndicator);
    add(exitButton);
}

void LiveDataDisplayView::tearDownScreen()
{

}

void LiveDataDisplayView::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &exitButton)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMenuScreen();
    }
}

void LiveDataDisplayView::handleTickEvent()
{
    tickCounter++;

    if (currentState == ANIMATE_SWIPE_CANCELLED_LEFT)
    {
        animateSwipeCancelledLeft();
    }
    else if (currentState == ANIMATE_SWIPE_CANCELLED_RIGHT)
    {
        animateSwipeCancelledRight();
    }
    else if (currentState == ANIMATE_LEFT)
    {
        animateLeft();
    }
    else if (currentState == ANIMATE_RIGHT)
    {
        animateRight();
    }

    // Sometimes change current temperature to make
    // demo screen look alive
    if (tickCounter % 100 == 0)
    {
        for (int16_t i = 0; i < NUMBER_OF_SCREENS; i++)
        {
            if ((rand() % 4) == 0)
            {
                infoScreen[i].adjustTemperature();
            }
        }
    }

}

void LiveDataDisplayView::handleClickEvent(const ClickEvent& evt)
{
    // If an animation is already in progress do not
    // react to clicks
    if (currentState != NO_ANIMATION)
    {
        // Make sure that click events outside the scroll area are propagated
        if (evt.getY() > infoScreensViewPort.getY())
        {
            View<LiveDataDisplayPresenter>::handleClickEvent(evt);
        }
        return;
    }

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Save current position for use during animation
        animateDistance = dragX;
        startX = infoScreens.getX();

        if (dragX < 0)
        {
            if (currentScreen == NUMBER_OF_SCREENS - 1 || dragX > -120)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_LEFT;
            }
            else
            {
                currentState = ANIMATE_LEFT;
            }
        }
        else if (dragX > 0)
        {
            if (currentScreen == 0 || dragX < 120)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_RIGHT;
            }
            else
            {
                currentState = ANIMATE_RIGHT;
            }
        }

        //adjustInfoScreens();
    }

    // Make sure that click events outside the scroll area are propagated
    if (evt.getY() > infoScreensViewPort.getY())
    {
        View<LiveDataDisplayPresenter>::handleClickEvent(evt);
        return;
    }
}

void LiveDataDisplayView::handleDragEvent(const DragEvent& evt)
{
    // If an animation is already in progress do not
    // react to drags
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    dragX += evt.getDeltaX();

    // Do not show too much background next to end screens
    if (currentScreen == 0 && dragX > backgroundSwipeAreaLeft.getWidth())
    {
        dragX = backgroundSwipeAreaLeft.getWidth();
    }
    else if (currentScreen == NUMBER_OF_SCREENS - 1 && dragX < -backgroundSwipeAreaRight.getWidth())
    {
        dragX = -backgroundSwipeAreaRight.getWidth();
    }

    adjustInfoScreens();
}

void LiveDataDisplayView::handleGestureEvent(const GestureEvent& evt)
{
    // Do not accept gestures while animating
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (evt.getType() == evt.SWIPE_HORIZONTAL)
    {
        // Save current position for use during animation
        animateDistance = dragX;
        startX = infoScreens.getX();

        if (evt.getVelocity() < 0  && currentScreen < NUMBER_OF_SCREENS - 1)
        {
            currentState = ANIMATE_LEFT;
        }
        else if (evt.getVelocity() > 0  && currentScreen > 0)
        {
            currentState = ANIMATE_RIGHT;
        }
    }
}

void LiveDataDisplayView::adjustInfoScreens()
{
    infoScreens.moveTo(- (currentScreen * HAL::DISPLAY_WIDTH) + dragX, infoScreens.getY());

    int alphaAdjustment = (dragX < 0) ? -dragX : dragX;
    weekInfoBar.setAlpha(255 - alphaAdjustment);
}

void LiveDataDisplayView::animateSwipeCancelledLeft()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, -animateDistance, duration);
        dragX = animateDistance + delta;

        adjustInfoScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragX = 0;
        adjustInfoScreens();
    }
    animationCounter++;
}

void LiveDataDisplayView::animateSwipeCancelledRight()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, animateDistance, duration);
        dragX = animateDistance - delta;

        adjustInfoScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragX = 0;
        adjustInfoScreens();
    }
    animationCounter++;
}

void LiveDataDisplayView::animateLeft()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, infoScreensViewPort.getWidth() + animateDistance, duration);
        dragX = animateDistance - delta;

        adjustInfoScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentScreen++;
        dragX = 0;
        weekInfoBar.setTextColor(infoScreen[currentScreen].getTextColor());
        weekInfoBar.setInfo(infoScreen[currentScreen].getCity());
        adjustInfoScreens();
        dotIndicator.goRight();
    }
    animationCounter++;
}

void LiveDataDisplayView::animateRight()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, infoScreensViewPort.getWidth() - animateDistance, duration);
        dragX = animateDistance + delta;

        adjustInfoScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentScreen--;
        dragX = 0;
        weekInfoBar.setTextColor(infoScreen[currentScreen].getTextColor());
        weekInfoBar.setInfo(infoScreen[currentScreen].getCity());
        adjustInfoScreens();
        dotIndicator.goLeft();
    }
    animationCounter++;
}
