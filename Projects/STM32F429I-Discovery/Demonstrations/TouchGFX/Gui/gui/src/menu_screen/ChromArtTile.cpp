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
  


#include <gui/menu_screen/ChromArtTile.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Color.hpp>

ChromArtTile::ChromArtTile()
{
    setWidth(90);
    setHeight(70);

    tileId.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    tileId.setTypedText(TypedText(T_CHROM_ART_TILE));
    tileId.setPosition(0, getHeight() - 22, getWidth(), 20);

    add(tileId);
}

ChromArtTile::~ChromArtTile()
{
}
