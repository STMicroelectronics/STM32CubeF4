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
  


#include <gui/menu_screen/AnimatedGraphicsTile.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Color.hpp>

AnimatedGraphicsTile::AnimatedGraphicsTile()
{
    setWidth(90);
    setHeight(70);

    tileId.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    tileId.setTypedText(TypedText(T_ANIMATED_GRAPHICS_TILE));
    tileId.setPosition(0, getHeight() - 22, getWidth(), 20);

    add(tileId);
}

AnimatedGraphicsTile::~AnimatedGraphicsTile()
{
}
