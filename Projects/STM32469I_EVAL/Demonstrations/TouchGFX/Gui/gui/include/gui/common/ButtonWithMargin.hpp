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
