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
  


#include <gui/custom_controls_screen/ThreeWayProgressBarCircle.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <assert.h>


ThreeWayProgressBarCircle::ThreeWayProgressBarCircle()
{
    background.setBitmap(Bitmap(BITMAP_CONTROLS_THREE_WAY_SMALL_CIRCLE_ID));
    background.setXY(0, 0);
    add(background);

    add(icon);

    text.setTypedText(TypedText(T_THREE_WAY_PROGRESS_BAR_PERCENTAGE));
    text.setWildcard(textBuffer);
    text.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    text.setPosition(18, 37, 30, 14);
    add(text);
    setPercentage(0);

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

ThreeWayProgressBarCircle::~ThreeWayProgressBarCircle()
{
}

void ThreeWayProgressBarCircle::setIcon(BitmapId newIconBmp, BitmapId newIconActiveBmp)
{
    iconBmp = newIconBmp;
    iconActiveBmp = newIconActiveBmp;

    setActive(false);
    icon.setXY((background.getWidth() - icon.getWidth()) / 2, 4);
}

void ThreeWayProgressBarCircle::setActive(bool active)
{
    active ? icon.setBitmap(Bitmap(iconActiveBmp)) : icon.setBitmap(Bitmap(iconBmp));
    icon.invalidate();
}

void ThreeWayProgressBarCircle::setPercentage(int percentage)
{
    Unicode::snprintf(textBuffer, 5, "%d", percentage);
    text.invalidate();
}


