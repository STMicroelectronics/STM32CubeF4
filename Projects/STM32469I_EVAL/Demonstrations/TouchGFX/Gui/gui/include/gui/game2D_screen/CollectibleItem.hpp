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
