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
  

#include <gui/game2D_screen/CollectibleItem.hpp>
#include <BitmapDatabase.hpp>


CollectibleItem::CollectibleItem() :
    tickCounter(0),
    speed(0)
{
    add(image);

    image.setBitmaps(BITMAP_COLLECTIBLE_ITEM_00_ID, BITMAP_COLLECTIBLE_ITEM_09_ID);
    image.setUpdateTicksInterval(4);

    setWidth(image.getWidth());
    setHeight(image.getHeight());
}

CollectibleItem::~CollectibleItem()
{
}


void CollectibleItem::setBitmaps(BitmapId start, BitmapId end)
{
    image.setBitmaps(start, end);
}

void CollectibleItem::handleTickEvent()
{
    tickCounter++;

    if (speed != 0)
    {
        moveTo(getX() - speed, getY());
    }
}

void CollectibleItem::startAnimation(int16_t newSpeed)
{
    speed = newSpeed;
    running = true;
    Application::getInstance()->registerTimerWidget(this);
    image.startAnimation(false, true, true);
}

void CollectibleItem::stopAnimation()
{
    running = false;
    Application::getInstance()->unregisterTimerWidget(this);
    image.stopAnimation();
}
