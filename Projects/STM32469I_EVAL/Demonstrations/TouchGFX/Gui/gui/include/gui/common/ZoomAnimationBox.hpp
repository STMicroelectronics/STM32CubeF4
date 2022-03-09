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
  

#ifndef ZOOM_ANIMATION_BOX_HPP_
#define ZOOM_ANIMATION_BOX_HPP_

#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/EasingEquations.hpp>

using namespace touchgfx;

class ZoomAnimationBox : public Box
{
public:
    ZoomAnimationBox();
    virtual ~ZoomAnimationBox();

    void startZoomAnimation(int16_t endWidth, int16_t endHeight, uint16_t duration, touchgfx::EasingEquation widthProgressionEquation = &touchgfx::EasingEquations::linearEaseNone, EasingEquation heightProgressionEquation = &touchgfx::EasingEquations::linearEaseNone);

    virtual void handleTickEvent();

    /**
     * Associates an action to be performed when the animation ends.
     *
     * @param callback The callback to be executed. The callback will be given a
     * reference to the ZoomAnimationBox.
     *
     * @see GenericCallback
     */
    void setAnimationEndedCallback(touchgfx::GenericCallback<const ZoomAnimationBox&>& callback) { animationEndedAction = &callback; }


protected:

    /**
     * Animation states.
     *
     * ANIMATE_ZOOM: Zoom animation state
     */
    enum States
    {
        ANIMATE_ZOOM,
        NO_ANIMATION
    };

    /**
     * The current animation state
     */
    States currentState;

    /**
     * The progress counter for the animation 
     */
    uint32_t animationCounter;
    
    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationStartWidth;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationStartHeight;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationEndWidth;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationEndHeight;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationStartX;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationStartY;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationDeltaX;

    /**
     * Member variable for the current animation 
     */
    int16_t zoomAnimationDeltaY;


    /**
     * Member variable for the current animation 
     */
    uint16_t animationDuration;

    /**
     * Member variable for the current animation 
     */
    EasingEquation zoomAnimationWidthEquation;

    /**
     * Member variable for the current animation 
     */
    EasingEquation zoomAnimationHeightEquation;

    /**
     * Animation ended Callback  
     */
    touchgfx::GenericCallback<const ZoomAnimationBox&>* animationEndedAction;

    /**
     * Sets the current animation state.
     *
     * @param state The new state.
     */
    virtual void setCurrentState(States state);
};

#endif /* ZOOM_ANIMATION_BOX_HPP_ */
