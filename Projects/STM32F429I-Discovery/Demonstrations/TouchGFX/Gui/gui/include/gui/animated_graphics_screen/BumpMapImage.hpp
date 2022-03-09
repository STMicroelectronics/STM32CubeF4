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
  


#ifndef BUMP_MAP_IMAGE_HPP
#define BUMP_MAP_IMAGE_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class BumpMapImage : public Widget
{
public:

    BumpMapImage(BitmapId image_, const unsigned int* bump_map_, const unsigned char* light_source_);
    virtual ~BumpMapImage();

    virtual void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);

    virtual void draw(const touchgfx::Rect&) const;
    virtual void drawOptimized(const touchgfx::Rect&) const;
    virtual void drawNormal(const touchgfx::Rect&) const;

    virtual Rect getSolidRect() const;
    void setLightPosition(uint16_t x, uint16_t y);

    void setImage(Bitmap newImage)
    {
        image = newImage;
    }
    void setBumpMap(const unsigned int* bump_map_)
    {
        bump_map = bump_map_;
    }
    void setDragFinish(bool state);

private:
    static const uint16_t lightSourceWidth = 128;
    static const uint16_t lightSourceHeight = 128;
    static const uint8_t lightMax = 48;

    enum States
    {
        DRAGGING,
        ANIMATE_CATCH,
        ANIMATE_AUTO
    } currentState;

    Bitmap image;
    Bitmap bump;
    const unsigned int* bump_map;
    const unsigned char* light_source;

    int currentTime;
    uint8_t catchTime;
    uint16_t lightPositionX;
    uint16_t lightPositionY;
    uint16_t tempPositionX;
    uint16_t tempPositionY;

    int height(uint16_t value) const;
    void invalidateAccordingToCurrentLightSetting();

#ifdef SIMULATOR
    uint8_t light(int16_t x, int16_t y) const;
    void printBumpMap();
    void printLightTable();
#endif // SIMULATOR
};

#endif // BUMP_MAP_IMAGE_HPP
