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
  

#if 0
#include <gui/common/Button.hpp>

void Button::draw(const touchgfx::Rect& invalidatedArea) const
{
    touchgfx::Bitmap bmp(AbstractButton::pressed ? down : up);
    touchgfx::Rect dirty(buttonX(), buttonY(), bmp.getWidth(), bmp.getHeight());
    dirty &= invalidatedArea;
    if((bmp.getId() != touchgfx::BITMAP_INVALID) && !dirty.isEmpty())
    {
        dirty.x -= buttonX();
        dirty.y -= buttonY();
        touchgfx::Rect r(buttonX(), buttonY(), 0, 0) ;
        translateRectToAbsolute(r);
        touchgfx::HAL::lcd().drawPartialBitmap(bmp, r.x, r.y, dirty, alpha);
    }
}

touchgfx::Rect Button::getSolidRect() const
{
    if (alpha < 255)
    {
        return touchgfx::Rect();
    }

    touchgfx::Rect result = AbstractButton::pressed ? down.getSolidRect() : up.getSolidRect();
    result.x += buttonX();
    result.y += buttonY();

    return result;
}

int16_t Button::buttonX() const
{
    return getWidth()/2-down.getWidth()/2;
}

int16_t Button::buttonY() const
{
    return getHeight()/2-down.getHeight()/2;
}
#endif
