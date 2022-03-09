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
