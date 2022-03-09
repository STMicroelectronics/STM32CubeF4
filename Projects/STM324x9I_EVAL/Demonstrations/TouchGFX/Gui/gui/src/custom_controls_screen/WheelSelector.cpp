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
  


#include <gui/custom_controls_screen/WheelSelector.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

WheelSelector::WheelSelector() :
    currentAnimationState(NO_ANIMATION),
    elementSelectedAction(0),
    textMoveEndedCallback(this, &WheelSelector::textMoveEndedHandler)
{
}

WheelSelector::~WheelSelector()
{
}

void WheelSelector::setup(int width, int height, int textXPosition, int textYPosition, TEXTS wheelText)
{
    setWidth(width);
    setHeight(height);
    setTouchable(true);

    selectedTextYPosition = textYPosition;

    textContainer.setPosition(0, 0, getWidth(), getHeight());
    add(textContainer);

    text.setTypedText(TypedText(wheelText));
    text.setXY(textXPosition, selectedTextYPosition);
    text.setMoveAnimationEndedAction(textMoveEndedCallback);
    textContainer.add(text);

    spaceBetweenTextRows = text.getTypedText().getFont()->getMinimumTextHeight() * 2; // For blank lines between texts;

    // Prepare textSelected (if colors are different)
    // Position of textSelectedContainer (y coordinate) updated later in setTextColor()
    textSelectedContainer.setPosition(textContainer.getX(), textContainer.getY(), textContainer.getWidth(), 0);
    add(textSelectedContainer);

    textSelectedBackground.setPosition(0, 0, textSelectedContainer.getWidth(), 0);
    textSelectedContainer.add(textSelectedBackground);

    textSelected.setTypedText(text.getTypedText());
    // Position of textSelected updated later in setTextColor()
    textSelected.setXY(text.getX(), text.getY());
    textSelected.setMoveAnimationEndedAction(textMoveEndedCallback);
    textSelectedContainer.add(textSelected);
    // Hide selected text, shown when normal/selected colors differ, see setTextColor()
    textSelectedContainer.setVisible(false);

    topCutOff = selectedTextYPosition + (spaceBetweenTextRows / 2);
    bottomCutOff = topCutOff - text.getHeight() - (spaceBetweenTextRows / 3);

    reset();
}

void WheelSelector::setTextColor(colortype normalColor, colortype selectedColor, colortype selectedBackgroundColor, int selectedYOffset, int selectedHeight)
{
    text.setColor(normalColor);
    if (normalColor != selectedColor)
    {
        selectedAreaYOffset = selectedYOffset;
        textSelectedBackground.setHeight(selectedHeight);
        textSelectedBackground.setColor(selectedBackgroundColor);
        textSelectedContainer.setY(textContainer.getY() + selectedAreaYOffset);
        textSelectedContainer.setHeight(selectedHeight);
        textSelected.setColor(selectedColor);
        textSelected.setY(text.getY() - selectedAreaYOffset);
        textSelectedContainer.setVisible(true);
    }
    else
    {
        textSelectedContainer.setVisible(false);
    }
    text.invalidate(); // This also overlaps with textSelected which is therefore also redrawn
}

void WheelSelector::handleDragEvent(const DragEvent& evt)
{
    if (currentAnimationState == ANIMATING_DRAG)
    {
        moveText(text.getY() + evt.getDeltaY());
    }
}

void WheelSelector::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        if (currentAnimationState != NO_ANIMATION)
        {
            text.cancelMoveAnimation();
            textSelected.cancelMoveAnimation();
        }
        currentAnimationState = ANIMATING_DRAG;
    }
    else if (evt.getType() == ClickEvent::RELEASED && currentAnimationState == ANIMATING_DRAG)
    {
        snap();
    }
}

void WheelSelector::handleGestureEvent(const GestureEvent& evt)
{
    if (evt.getType() == GestureEvent::SWIPE_VERTICAL)
    {
        currentAnimationState = ANIMATING_GESTURE;

        int delta = evt.getVelocity() * 5;
        int newYPosition = text.getY() + delta;
        adjustForBoundries(newYPosition);

        int distanceToMove = newYPosition - text.getY();
        int distanceToMoveABS = (distanceToMove > 0) ? distanceToMove : -distanceToMove;
        int duration = distanceToMoveABS / 10;
        duration = (duration < 2) ? 2 : duration;

        animateMoveText(newYPosition, duration, EasingEquations::cubicEaseOut);
    }
}

void WheelSelector::snap()
{
    int deltaUp = (spaceBetweenTextRows - (text.getY() - selectedTextYPosition)) % spaceBetweenTextRows;

    // Invert % value for negative values
    deltaUp = (deltaUp > 0) ? deltaUp : spaceBetweenTextRows + deltaUp;

    int deltaDown = spaceBetweenTextRows - deltaUp;

    if (deltaUp < deltaDown)
    {
        animateMoveText(text.getY() + deltaUp, 10, EasingEquations::backEaseInOut);
    }
    else
    {
        animateMoveText(text.getY() - deltaDown, 10, EasingEquations::backEaseInOut);
    }
    currentAnimationState = ANIMATING_SNAP;
}

int WheelSelector::getSelectedIndex()
{
    return (selectedTextYPosition - text.getY()) / spaceBetweenTextRows;
}

void WheelSelector::setSelectedIndex(int index, int duration, EasingEquation equation)
{
    int newYPosition = selectedTextYPosition - (spaceBetweenTextRows * index);
    adjustForBoundries(newYPosition);

    if (duration == 0)
    {
        moveText(newYPosition);
    }
    else
    {
        currentAnimationState = ANIMATING_GESTURE;
        animateMoveText(newYPosition, duration, equation);
    }

}

void WheelSelector::textMoveEndedHandler(const MoveAnimator<TextArea>& textArea)
{
    if (currentAnimationState == ANIMATING_GESTURE)
    {
        if (!textArea.isRunning() && !textSelected.isRunning())
        {
            snap();
        }
    }
    else if (currentAnimationState == ANIMATING_SNAP)
    {
        currentAnimationState = NO_ANIMATION;
        if (elementSelectedAction && elementSelectedAction->isValid())
        {
            elementSelectedAction->execute(*this, getSelectedIndex());
        }
    }
    else
    {
        currentAnimationState = NO_ANIMATION;
    }
}

void WheelSelector::adjustForBoundries(int& newYValue)
{
    if (newYValue > topCutOff)
    {
        newYValue = topCutOff;
    }
    else if (newYValue < bottomCutOff)
    {
        newYValue = bottomCutOff;
    }
}

void WheelSelector::moveText(int newYPosition)
{
    adjustForBoundries(newYPosition);

    text.moveTo(text.getX(), newYPosition);
    if (textSelectedContainer.isVisible())
    {
        textSelected.moveTo(text.getX(), newYPosition - selectedAreaYOffset);
    }
}

void WheelSelector::animateMoveText(int newYPosition, int duration, EasingEquation equation)
{
    text.startMoveAnimation(text.getX(), newYPosition, duration, EasingEquations::linearEaseNone, equation);
    if (textSelectedContainer.isVisible())
    {
        textSelected.startMoveAnimation(text.getX(), newYPosition - selectedAreaYOffset, duration, EasingEquations::linearEaseNone, equation);
    }
}

void WheelSelector::reset()
{
    moveText(selectedTextYPosition);
}
