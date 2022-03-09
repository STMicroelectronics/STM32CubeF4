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
  


#include <gui/common/SwipeContainer.hpp>
#include <touchgfx/EasingEquations.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

using namespace touchgfx;

SwipeContainer::SwipeContainer() :
    currentState(NO_ANIMATION),
    numberOfScreens(0),
    animationCounter(0),
    swipeCutoff(80),
    dragX(0),
    currentScreen(0),
    endElasticWidth(30),
    screens(EAST),
    menuChangeInitiatedAction(0),
    menuChangeEndedAction(0)
{
    touchgfx::Application::getInstance()->registerTimerWidget(this);

    setTouchable(true);

    Container::add(screens);
    Container::add(dotIndicator);
}

SwipeContainer::~SwipeContainer()
{
    touchgfx::Application::getInstance()->unregisterTimerWidget(this);
}


void SwipeContainer::add(Drawable& screen)
{
    screens.add(screen);
    numberOfScreens++;

    dotIndicator.setNumberOfDots(numberOfScreens);

    setWidth(screen.getWidth());
    setHeight(screen.getHeight());
}

void SwipeContainer::setEndSwipeElasticWidth(uint16_t width)
{
    endElasticWidth = width;
}

void SwipeContainer::setSwipeCutoff(uint16_t cutoff)
{
    swipeCutoff = cutoff;
}

void SwipeContainer::setDotIndicatorBitmaps(const touchgfx::Bitmap& normalDot, const touchgfx::Bitmap& highlightedDot)
{
    dotIndicator.setBitmaps(normalDot, highlightedDot);
}

void SwipeContainer::setDotIndicatorXY(uint16_t x, uint16_t y)
{
    dotIndicator.setXY(x, y);
}

void SwipeContainer::setDotIndicatorXYWithCenteredX(uint16_t x, uint16_t y)
{
    dotIndicator.setXY(x - dotIndicator.getWidth() / 2, y);
}

void SwipeContainer::setSelectedScreen(uint8_t screenIndex)
{
    currentScreen = screenIndex;
    dotIndicator.setHighlightPosition(currentScreen);
    adjustScreens();
}

void SwipeContainer::handleTickEvent()
{
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
}

void SwipeContainer::handleClickEvent(const ClickEvent& evt)
{
    // If an animation is already in progress do not
    // react to clicks
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Save current position for use during animation
        animateDistance = dragX;
        startX = screens.getX();

        if (dragX < 0)
        {
            if (currentScreen == getNumberOfScreens() - 1 || dragX > -swipeCutoff)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_LEFT;
            }
            else
            {
                currentState = ANIMATE_LEFT;

                if (menuChangeInitiatedAction && menuChangeInitiatedAction->isValid())
                {
                    menuChangeInitiatedAction->execute(*this);
                }
            }
        }
        else if (dragX > 0)
        {
            if (currentScreen == 0 || dragX < swipeCutoff)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_RIGHT;
            }
            else
            {
                currentState = ANIMATE_RIGHT;

                if (menuChangeInitiatedAction && menuChangeInitiatedAction->isValid())
                {
                    menuChangeInitiatedAction->execute(*this);
                }
            }
        }
    }
}

void SwipeContainer::handleDragEvent(const DragEvent& evt)
{
    // If an animation is already in progress do not
    // react to drags
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    dragX += evt.getDeltaX();

    // Do not show too much background next to end screens
    if (currentScreen == 0 && dragX > endElasticWidth)
    {
        dragX = endElasticWidth;
    }
    else if (currentScreen == getNumberOfScreens() - 1 && dragX < -endElasticWidth)
    {
        dragX = -endElasticWidth;
    }

    adjustScreens();
}

void SwipeContainer::handleGestureEvent(const GestureEvent& evt)
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
        startX = screens.getX();

        if (evt.getVelocity() < 0  && currentScreen < getNumberOfScreens() - 1)
        {
            currentState = ANIMATE_LEFT;

            if (menuChangeInitiatedAction && menuChangeInitiatedAction->isValid())
            {
                menuChangeInitiatedAction->execute(*this);
            }
        }
        else if (evt.getVelocity() > 0  && currentScreen > 0)
        {
            currentState = ANIMATE_RIGHT;

            if (menuChangeInitiatedAction && menuChangeInitiatedAction->isValid())
            {
                menuChangeInitiatedAction->execute(*this);
            }
        }
    }
}

void SwipeContainer::adjustScreens()
{
    screens.moveTo(-(currentScreen * getWidth()) + dragX, 0);
}

void SwipeContainer::animateSwipeCancelledLeft()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, -animateDistance, duration);
        dragX = animateDistance + delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragX = 0;
        adjustScreens();
    }
    animationCounter++;
}

void SwipeContainer::animateSwipeCancelledRight()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, animateDistance, duration);
        dragX = animateDistance - delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragX = 0;
        adjustScreens();
    }
    animationCounter++;
}

void SwipeContainer::animateLeft()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() + animateDistance, duration);
        dragX = animateDistance - delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;

        animationCounter = 0;
        currentScreen++;
        dragX = 0;
        adjustScreens();
        dotIndicator.goRight();

        if (menuChangeEndedAction && menuChangeEndedAction->isValid())
        {
            menuChangeEndedAction->execute(*this);
        }
    }
    animationCounter++;
}

void SwipeContainer::animateRight()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() - animateDistance, duration);
        dragX = animateDistance + delta;

        adjustScreens();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentScreen--;
        dragX = 0;
        adjustScreens();
        dotIndicator.goLeft();

        if (menuChangeEndedAction && menuChangeEndedAction->isValid())
        {
            menuChangeEndedAction->execute(*this);
        }
    }
    animationCounter++;
}



