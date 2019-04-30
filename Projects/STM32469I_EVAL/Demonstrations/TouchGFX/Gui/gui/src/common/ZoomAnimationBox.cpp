/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

