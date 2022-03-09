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
  

#ifndef COLLECTIBLE_ITEM_HPP
#define COLLECTIBLE_ITEM_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>

using namespace touchgfx;

class CollectibleItem : public Container
{
public:
    CollectibleItem();
    virtual ~CollectibleItem();

    void startAnimation(int16_t newSpeed);
    void stopAnimation();

    void setBitmaps(BitmapId start, BitmapId end);

    bool isRunning() { return running; }

    virtual void handleTickEvent();
private:

    int tickCounter;
    int speed;
    bool running;

    AnimatedImage image;
};

#endif /* COLLECTIBLE_ITEM_HPP */
