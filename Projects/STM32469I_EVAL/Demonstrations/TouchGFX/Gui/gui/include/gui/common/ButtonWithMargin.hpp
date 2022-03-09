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
  

#ifndef BUTTON_WITH_MARGIN_HPP
#define BUTTON_WITH_MARGIN_HPP

#include <touchgfx/widgets/Button.hpp>

class Button : public touchgfx::Button
{
public:
    virtual void draw(const touchgfx::Rect& invalidatedArea) const;
    virtual touchgfx::Rect getSolidRect() const;
private:
    int16_t buttonX() const;
    int16_t buttonY() const;
};

#endif
