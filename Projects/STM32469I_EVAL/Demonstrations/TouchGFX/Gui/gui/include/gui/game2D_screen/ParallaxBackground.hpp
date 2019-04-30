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
  

#ifndef PARALLAX_BACKGROUND_HPP
#define PARALLAX_BACKGROUND_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <gui/game2D_screen/CollectibleItem.hpp>

using namespace touchgfx;

class ParallaxBackground : public Container
{
public:
    ParallaxBackground(int16_t width, int16_t height);
    virtual ~ParallaxBackground();

    void startAnimation();
    void stopAnimation();

    void addPlayerCharacter(AnimatedImage& player);
    void addCollectibleItem(CollectibleItem& item);

    virtual void handleTickEvent();
private:

    struct Layer 
    {
        Image image0;
        Image image1;
        Image image2;
        int animationUpdateSpeed;
        int animationWidth;
    };

    enum AnimationState
    {
        ANIMATION_RUNNING,
        NO_ANIMATION
    };

    AnimationState currentAnimationState;

    int tickCounter;

    Layer background;
    Layer ground;
    Layer leaves;
    Layer treeTops;


    void initializeLayer(Layer& layer, BitmapId bmp, int animationUpdateSpeed, int animationWidth, int y);
    void moveLayer(Layer& layer, int tickCount);
};

#endif /* PARALLAX_BACKGROUND_HPP */
