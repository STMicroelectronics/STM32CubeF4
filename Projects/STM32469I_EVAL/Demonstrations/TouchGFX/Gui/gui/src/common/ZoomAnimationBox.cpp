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
  

#include <gui/common/ZoomAnimationBox.hpp>

ZoomAnimationBox::ZoomAnimationBox() :
    Box(),
    currentState(NO_ANIMATION),
    animationCounter(0),
    animationEndedAction(0)
{

}

ZoomAnimationBox::~ZoomAnimationBox()
{
}

void ZoomAnimationBox::startZoomAnimation(int16_t endWidth, int16_t endHeight, uint16_t duration, touchgfx::EasingEquation widthProgressionEquation /*= &touchgfx::EasingEquations::linearEaseNone*/, EasingEquation heightProgressionEquation /*= &touchgfx::EasingEquations::linearEaseNone*/)
{
    touchgfx::Application::getInstance()->registerTimerWidget(this);

    zoomAnimationStartX = getX();
    zoomAnimationStartY = getY();
    zoomAnimationStartWidth = getWidth();
    zoomAnimationStartHeight = getHeight();
    zoomAnimationEndWidth = endWidth;
    zoomAnimationEndHeight = endHeight;
    animationDuration = duration;

    zoomAnimationWidthEquation = widthProgressionEquation;
    zoomAnimationHeightEquation = heightProgressionEquation;

    zoomAnimationDeltaX = (zoomAnimationStartWidth - zoomAnimationEndWidth) / 2;
    zoomAnimationDeltaY = (zoomAnimationStartHeight - zoomAnimationEndHeight) / 2;

    setCurrentState(ANIMATE_ZOOM);
}

void ZoomAnimationBox::handleTickEvent()
{
    if (currentState == ANIMATE_ZOOM)
    {
        if (animationCounter <= animationDuration)
        {

            int16_t deltaWidth = zoomAnimationWidthEquation(animationCounter, 0, zoomAnimationEndWidth - zoomAnimationStartWidth, animationDuration);
            int16_t deltaHeight = zoomAnimationHeightEquation(animationCounter, 0, zoomAnimationEndHeight - zoomAnimationStartHeight, animationDuration);

            invalidate();
            int16_t newWidth = zoomAnimationStartWidth + deltaWidth;
            if (newWidth < 0)
            {
              newWidth = 0;
            }
            int16_t newHeight = zoomAnimationStartHeight + deltaHeight;
            if (newHeight < 0)
            {
              newHeight = 0;
            }
            setWidth(newWidth);
            setHeight(newHeight);

            moveTo(zoomAnimationStartX - (deltaWidth/2), zoomAnimationStartY - (deltaHeight/2));

            invalidate();
            animationCounter++; 
        }
        else
        {
            touchgfx::Application::getInstance()->unregisterTimerWidget(this);
            setCurrentState(NO_ANIMATION);

            if (animationEndedAction && animationEndedAction->isValid())
            {
                animationEndedAction->execute(*this);
            }
        } 
    }
}

void ZoomAnimationBox::setCurrentState(States state)
{
    currentState = state;
    animationCounter = 0;
}

