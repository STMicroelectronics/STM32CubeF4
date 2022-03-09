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
  


#include <gui/live_data_display_screen/WeekInfo.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

WeekInfo::WeekInfo()
{
    int smallIconDiff = 74;
    smallIcons[0].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_CLOUD_ID));
    smallIcons[0].setXY(12, 10);
    smallIcons[1].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_CLOUD_ID));
    smallIcons[1].setXY(smallIcons[0].getX() + 1 * smallIconDiff, 10);

    smallIcons[2].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_CLOUD_ID));
    smallIcons[2].setXY(smallIcons[0].getX() + 2 * smallIconDiff, 10);

    days[0].setTypedText(TypedText(T_WEATHER_DAY_0));
    days[0].setPosition(48, 11, 30, 16);
    days[1].setTypedText(TypedText(T_WEATHER_DAY_1));
    days[1].setPosition(days[0].getX() + 1 * smallIconDiff, 10, 30, 16);

    days[2].setTypedText(TypedText(T_WEATHER_DAY_2));
    days[2].setPosition(days[0].getX() + 2 * smallIconDiff, 10, 30, 16);

    smallTemperature->setX(48);
    for (int i = 0; i < NUMBER_OF_DAYS; i++)
    {
        smallTemperature[i].setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        smallTemperature[i].setTypedText(TypedText(T_WEATHER_SMALL_TEMPERATURE));
        smallTemperature[i].setPosition(smallTemperature[0].getX() + i * smallIconDiff, 25, 30, 16);
        smallTemperature[i].setWildcard(smallTemperatureBuffer[i]);

        smallTemperatureDropShadow[i].setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));
        smallTemperatureDropShadow[i].setTypedText(TypedText(T_WEATHER_SMALL_TEMPERATURE));
        smallTemperatureDropShadow[i].setPosition(smallTemperature[i].getX() + 1, smallTemperature[i].getY() + 1, 30, 16);
        smallTemperatureDropShadow[i].setWildcard(smallTemperatureBuffer[i]);
        smallTemperatureDropShadow[i].setAlpha(128);
    }

    for (int i = 0; i < NUMBER_OF_DAYS; i++)
    {
        add(smallIcons[i]);
        add(days[i]);
        add(smallTemperatureDropShadow[i]);
        add(smallTemperature[i]);
    }
}

/* Setup the WeekInfo with some static city information
 * In a real world example this data would be live updated
 * and come from the model, but in this demo it is just hard coded.
 */
void WeekInfo::setInfo(CityInfo::Cities city)
{
    switch (city)
    {
    case CityInfo::COPENHAGEN:
        smallIcons[0].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_ID));
        smallIcons[1].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_BIGCLOUD_ID));
        smallIcons[2].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_ID));
        Unicode::snprintf(smallTemperatureBuffer[0], 4, "%d", 23);
        Unicode::snprintf(smallTemperatureBuffer[1], 4, "%d", 16);
        Unicode::snprintf(smallTemperatureBuffer[2], 4, "%d", 24);
        break;

    case CityInfo::HONG_KONG:
        smallIcons[0].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_ID));
        smallIcons[1].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_BIGCLOUD_ID));
        smallIcons[2].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SHOWER_ID));
        Unicode::snprintf(smallTemperatureBuffer[0], 4, "%d", 23);
        Unicode::snprintf(smallTemperatureBuffer[1], 4, "%d", 19);
        Unicode::snprintf(smallTemperatureBuffer[2], 4, "%d", 21);
        break;

    case CityInfo::MUMBAI:
        smallIcons[0].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_ID));
        smallIcons[1].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_BIGCLOUD_ID));
        smallIcons[2].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_ID));
        Unicode::snprintf(smallTemperatureBuffer[0], 4, "%d", 32);
        Unicode::snprintf(smallTemperatureBuffer[1], 4, "%d", 28);
        Unicode::snprintf(smallTemperatureBuffer[2], 4, "%d", 27);
        break;

    case CityInfo::NEW_YORK:
        smallIcons[0].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SUN_BIGCLOUD_ID));
        smallIcons[1].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SHOWER_ID));
        smallIcons[2].setBitmap(Bitmap(BITMAP_WEATHER_SMALL_ICON_SHOWER_ID));
        Unicode::snprintf(smallTemperatureBuffer[0], 4, "%d", 20);
        Unicode::snprintf(smallTemperatureBuffer[1], 4, "%d", 19);
        Unicode::snprintf(smallTemperatureBuffer[2], 4, "%d", 19);
        break;

    default:
        break;
    }

    for (int i = 0; i < NUMBER_OF_DAYS; i++)
    {
        smallIcons[i].invalidate();
        smallTemperature[i].invalidate();
        smallTemperatureDropShadow[i].invalidate();
    }
}


void WeekInfo::setAlpha(int alpha)
{
    if (alpha < 0)
    {
        alpha = 0;
    }
    if (alpha > 255)
    {
        alpha = 255;
    }

    for (int i = 0; i < NUMBER_OF_DAYS; i++)
    {
        smallIcons[i].setAlpha(alpha);
        smallIcons[i].invalidate();

        smallTemperature[i].setAlpha(alpha);
        smallTemperature[i].invalidate();

        smallTemperatureDropShadow[i].setAlpha(alpha);
        smallTemperatureDropShadow[i].invalidate();

        days[i].setAlpha(alpha);
        days[i].invalidate();
    }
}

void WeekInfo::setTextColor(colortype color)
{
    for (int i = 0; i < NUMBER_OF_DAYS; i++)
    {
        days[i].setColor(color);
        days[i].invalidate();
    }
}
