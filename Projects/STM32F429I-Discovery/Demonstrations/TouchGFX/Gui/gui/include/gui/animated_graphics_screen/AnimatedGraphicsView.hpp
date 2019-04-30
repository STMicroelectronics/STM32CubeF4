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
  


#ifndef ANIMATED_GRAPHICS_VIEW_HPP
#define ANIMATED_GRAPHICS_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/animated_graphics_screen/AnimatedGraphicsPresenter.hpp>
#include <gui/animated_graphics_screen/BumpMapImage.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Box.hpp>

class AnimatedGraphicsView : public View<AnimatedGraphicsPresenter>
{
public:
    AnimatedGraphicsView();
    virtual ~AnimatedGraphicsView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void handleDragEvent(const DragEvent& evt);

private:

    enum States
    {
        ANIMATE_SHADE_UP,
        NO_ANIMATION
    } currentState;

    struct
    {
        BitmapId image;
        const unsigned int* bump_map;
    } bumpMapInfo;

    uint32_t animationCounter;

    Box shade;
    BumpMapImage bumpMapImage;
    Button exitButton;

    Callback<AnimatedGraphicsView, const AbstractButton&> onButtonPressed;

    void buttonPressedhandler(const AbstractButton& button);
    void updateBumpMapImage();

    void animateShadeUp();
};

#endif // ANIMATED_GRAPHICS_VIEW_HPP
