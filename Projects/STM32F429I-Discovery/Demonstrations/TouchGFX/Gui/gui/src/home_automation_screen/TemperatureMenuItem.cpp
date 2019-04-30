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
  


#include <gui/home_automation_screen/TemperatureMenuItem.hpp>
#include <touchgfx/Color.hpp>

TemperatureMenuItem::TemperatureMenuItem() :
    onButtonPressed(this, &TemperatureMenuItem::buttonPressedhandler)
{
    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_TEMPERATURE_MENU_ITEM_ID));

    primaryText.setPosition(10, 7, 160, 20);

    currentTemperature.setPosition(180, 5, 50, 24);
    currentTemperature.setTypedText(TypedText(T_TEMPERATURE_MENU));
    currentTemperature.setWildcard(currentTemperatureTextBuffer);

    itemSelectArea.setPosition(0, 0, background.getWidth(), background.getHeight());
    itemSelectArea.setAction(onButtonPressed);

    setSelected(false);

    add(background);
    add(primaryText);
    add(currentTemperature);
    add(itemSelectArea);

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

TemperatureMenuItem::~TemperatureMenuItem()
{

}

void TemperatureMenuItem::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &itemSelectArea)
    {
        if (menuItemSelectedCallback)
        {
            menuItemSelectedCallback->execute(*this);
        }
    }
}

void TemperatureMenuItem::setRoomTemperatureInfo(RoomTemperatureInfo& info)
{
    primaryText.setTypedText(TypedText(info.getRoomName()));
    primaryText.invalidate();

    Unicode::snprintf(currentTemperatureTextBuffer, 5, "%d", info.getCurrentTemperature());
    currentTemperature.invalidate();
}

void TemperatureMenuItem::setSelected(bool sel)
{
    selected = sel;

    if (selected)
    {
        background.setBitmap(Bitmap(BITMAP_TEMPERATURE_MENU_ITEM_SELECTED_ID));
        primaryText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        currentTemperature.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    }
    else
    {
        background.setBitmap(Bitmap(BITMAP_TEMPERATURE_MENU_ITEM_ID));
        primaryText.setColor(Color::getColorFrom24BitRGB(0xA9, 0xAD, 0xB6));
        currentTemperature.setColor(Color::getColorFrom24BitRGB(0xA9, 0xAD, 0xB6));
    }
    background.invalidate();
    primaryText.invalidate();
    currentTemperature.invalidate();
}

void TemperatureMenuItem::setTextsAlpha(int16_t alpha)
{
    alpha = (alpha < 0) ? 0 : alpha;
    alpha = (alpha > 255) ? 255 : alpha;

    primaryText.setAlpha((uint8_t) alpha);
    currentTemperature.setAlpha((uint8_t) alpha);

    primaryText.invalidate();
    currentTemperature.invalidate();
}
