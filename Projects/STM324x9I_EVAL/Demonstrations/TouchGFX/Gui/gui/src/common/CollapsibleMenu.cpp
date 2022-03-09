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
  


#include <gui/common/CollapsibleMenu.hpp>
#include <BitmapDatabase.hpp>
#include <assert.h>


CollapsibleMenu::CollapsibleMenu() :
    currentAnimationState(NO_ANIMATION),
    size(0),
    isExpanded(false),
    elementSpace(0),
    expandDirection(RIGHT),
    fadedOut(false),
    timeout(0),
    timeoutCounter(0),
    selectedElementIndex(0),
    onButtonPressed(this, &CollapsibleMenu::buttonPressedHandler),
    menuElementMoveAnimationEndedCallback(this, &CollapsibleMenu::menuElementMoveAnimationEndedHandler),
    menuElementFadeAnimationEndedCallback(this, &CollapsibleMenu::menuElementFadeAnimationEndedHandler),
    stateChangedAction(0)
{
    for (int i = MAX_NUMBER_OF_ELEMENTS - 1; i >= 0; i--)
    {
        menuElements[i].setVisible(false);
        menuElements[i].setAction(onButtonPressed);
        menuElements[i].setFadeAnimationEndedAction(menuElementFadeAnimationEndedCallback);
        menuElements[i].setMoveAnimationEndedAction(menuElementMoveAnimationEndedCallback);
        add(menuElements[i]);
    }
}

CollapsibleMenu::~CollapsibleMenu()
{
}

void CollapsibleMenu::addMenuElement(const Bitmap& elementBitmap, const Bitmap& elementPressedBitmap)
{
    menuElements[size].setBitmaps(elementBitmap, elementPressedBitmap);
    menuElements[size].setVisible(true);
    size++;
}


void CollapsibleMenu::setElementSpace(uint16_t space)
{
    elementSpace = space;
}

void CollapsibleMenu::setExpandDirection(ExpandDirection direction)
{
    expandDirection = direction;
}

void CollapsibleMenu::finilizeInitialization()
{
    int newWidth = 0;
    int newHeight = 0;

    for (int i = 0; i < size; i++)
    {
        newWidth += menuElements[i].getWidth();

        if (i < size - 1)
        {
            newWidth += elementSpace;
        }

        newHeight = (menuElements[i].getHeight() > newHeight) ? menuElements[i].getHeight() : newHeight;
    }

    setWidth(newWidth);
    setHeight(newHeight);

    for (int i = 0; i < size; i++)
    {
        if (expandDirection == LEFT)
        {
            menuElements[i].setXY(getWidth() - menuElements[i].getWidth(), 0);
        }
        else
        {
            menuElements[i].setXY(0, 0);
        }
    }
}

void CollapsibleMenu::buttonPressedHandler(const AbstractButton& button)
{
    if (currentAnimationState != NO_ANIMATION)
    {
        return;
    }

    if (isExpanded)
    {
        // Place the old selected element in it correct z order
        if (selectedElementIndex > 0)
        {
            remove(menuElements[selectedElementIndex]);
            insert(&menuElements[selectedElementIndex - 1], menuElements[selectedElementIndex]);
        }

        uint8_t oldSelectedElementIndex = selectedElementIndex;

        // Find the new selected index
        for (int i = 0; i < size; i++)
        {
            if (&button == &menuElements[i])
            {
                selectedElementIndex = i;
                break;
            }
        }

        // Place the newly selected element in front (z order)
        remove(menuElements[selectedElementIndex]);
        add(menuElements[selectedElementIndex]);

        int duration = 20;
        int delayCounter = 0;

        for (int i = 0; i < size; i++)
        {
            // Move all elements except the old selected that is already in place
            if (i != oldSelectedElementIndex)
            {
                int endPosition = (expandDirection == LEFT) ? getWidth() - menuElements[i].getWidth() : 0;

                menuElements[i].setMoveAnimationDelay(2 * delayCounter++);
                menuElements[i].startMoveAnimation(endPosition, menuElements[i].getY(), duration, EasingEquations::cubicEaseOut);
            }
        }

        currentAnimationState = ANIMATE_TO_COLLAPSED;

        if (timeout > 0)
        {
            // Remove timeout
            Application::getInstance()->unregisterTimerWidget(this);
        }
    }
    else
    {
        int duration = 20;
        int delayCounter = 0;

        int posX = (expandDirection == LEFT) ? getWidth() - menuElements[selectedElementIndex].getWidth() : menuElements[selectedElementIndex].getWidth() + elementSpace;

        for (int i = 0; i < size; i++)
        {
            if (i != selectedElementIndex)
            {
                if (expandDirection == LEFT)
                {
                    posX -= menuElements[i].getWidth() + elementSpace;
                }

                menuElements[i].setMoveAnimationDelay(2 * delayCounter++);
                menuElements[i].startMoveAnimation(posX, menuElements[i].getY(), duration, EasingEquations::cubicEaseOut);

                if (expandDirection == RIGHT)
                {
                    posX += menuElements[i].getWidth() + elementSpace;
                }
            }
        }

        currentAnimationState = ANIMATE_TO_EXPANDED;

        // Set timeout
        if (timeout > 0)
        {
            Application::getInstance()->registerTimerWidget(this);
            timeoutCounter = 0;
        }
    }
}

void CollapsibleMenu::fadeIn(int duration)
{
    for (int i = 0; i < size; i++)
    {
        menuElements[i].setVisible(true);
    }

    if (isExpanded)
    {
        for (int i = 0; i < size; i++)
        {
            menuElements[i].setFadeAnimationDelay(0);
            menuElements[i].startFadeAnimation(255, duration);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            if (i == selectedElementIndex)
            {
                menuElements[i].setFadeAnimationDelay(0);
                menuElements[i].startFadeAnimation(255, duration);
            }
            else
            {
                menuElements[i].setFadeAnimationDelay(duration);
                menuElements[i].startFadeAnimation(255, 1);
            }
        }
    }

    currentAnimationState = FADE_IN;
}

void CollapsibleMenu::fadeOut(int duration)
{
    for (int i = 0; i < size; i++)
    {
        menuElements[i].setVisible(false);
        menuElements[i].invalidate();
    }
    fadedOut = true;
    /*
    if (isExpanded)
    {
        for (int i = 0; i < size; i++)
        {
            menuElements[i].setFadeAnimationDelay(10);
            menuElements[i].startFadeAnimation(0, duration);
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            if (i == selectedElementIndex)
            {
                menuElements[i].setFadeAnimationDelay(10);
                menuElements[i].startFadeAnimation(0, duration);
            }
            else
            {
                menuElements[i].setAlpha(0);
                menuElements[i].invalidate();
            }
        }
    }

    currentAnimationState = FADE_OUT;
    */
}

bool CollapsibleMenu::isFadedOut()
{
    return fadedOut;
}

void CollapsibleMenu::menuElementMoveAnimationEndedHandler(const MoveAnimator<Button>& element)
{
    if (currentAnimationState == ANIMATE_TO_COLLAPSED)
    {
        currentAnimationState = NO_ANIMATION;
        isExpanded = false;
    }
    else if (currentAnimationState == ANIMATE_TO_EXPANDED)
    {
        currentAnimationState = NO_ANIMATION;
        isExpanded = true;
    }
    else
    {
        return;
    }

    if (stateChangedAction && stateChangedAction->isValid())
    {
        stateChangedAction->execute(*this, isExpanded);
    }

}

void CollapsibleMenu::menuElementFadeAnimationEndedHandler(const FadeAnimator<MoveAnimator<Button> >& element)
{
    if (anyFadeAnimationRunning())
    {
        // Wait for the last FadeAnimation to react.
        return;
    }

    if (currentAnimationState == FADE_IN)
    {
        currentAnimationState = NO_ANIMATION;
        fadedOut = false;
    }
    else if (currentAnimationState == FADE_OUT)
    {
        currentAnimationState = NO_ANIMATION;
        fadedOut = true;
        for (int i = 0; i < size; i++)
        {
            menuElements[i].setVisible(false);
            menuElements[i].invalidate();
        }
    }
    else if (currentAnimationState == FADE_IN_AND_EXPAND)
    {
        currentAnimationState = NO_ANIMATION;
        fadedOut = false;
        expandMenu();
    }
}

void CollapsibleMenu::collapseMenu()
{
    if (isExpanded)
    {
        buttonPressedHandler(menuElements[selectedElementIndex]);
    }
}

void CollapsibleMenu::expandMenu()
{
    if (!isExpanded)
    {
        buttonPressedHandler(menuElements[selectedElementIndex]);
    }
}


void CollapsibleMenu::setTimeout(int newTimeout)
{
    timeout = newTimeout;
}

void CollapsibleMenu::fadeInAndExpand(int duration)
{
    fadeIn(duration);
    currentAnimationState = FADE_IN_AND_EXPAND;
}

bool CollapsibleMenu::anyFadeAnimationRunning()
{
    for (int i = 0; i < size; i++)
    {
        if (menuElements[i].isRunning())
        {
            return true;
        }
    }
    return false;
}

uint8_t CollapsibleMenu::getSelectedElementIndex()
{
    return selectedElementIndex;
}

void CollapsibleMenu::handleTickEvent()
{
    timeoutCounter++;
    if (timeoutCounter >= timeout)
    {
        collapseMenu(); // Will also unregister time
    }
}
