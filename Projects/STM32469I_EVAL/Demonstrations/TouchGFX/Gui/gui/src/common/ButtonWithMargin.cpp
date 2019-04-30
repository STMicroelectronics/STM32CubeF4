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
