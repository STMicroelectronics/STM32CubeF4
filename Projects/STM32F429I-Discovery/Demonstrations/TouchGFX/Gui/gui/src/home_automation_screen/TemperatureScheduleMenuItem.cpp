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
  


#include <gui/home_automation_screen/TemperatureScheduleMenuItem.hpp>
#include <touchgfx/Color.hpp>

TemperatureScheduleMenuItem::TemperatureScheduleMenuItem() :
    onSelectionAreaPressed(this, &TemperatureScheduleMenuItem::selectionAreaPressedhandler),
    onSliderUpdated(this, &TemperatureScheduleMenuItem::sliderUpdatedHandler)
{
    background.setXY(0, 0);
    background.setBitmap(Bitmap(BITMAP_TEMPERATURE_SCHEDULE_MENU_ITEM_ID));

    weekdayText.setPosition(10, 7, 160, 30);
    weekdayText.setTypedText(TypedText(T_TEMPERATURE_ROOM7));

    daySelected.setXY(119, 0);
    daySelected.setBitmap(Bitmap(BITMAP_TEMPERATURE_SCHEDULE_BUTTON_SELECTED_ID));
    daySelected.setVisible(false);
    daySelectArea.setPosition(daySelected.getX(), daySelected.getY(), daySelected.getWidth(), daySelected.getHeight());
    daySelectArea.setAction(onSelectionAreaPressed);

    nightSelected.setXY(daySelected.getX() + daySelected.getWidth() + 1, 0);
    nightSelected.setBitmap(Bitmap(BITMAP_TEMPERATURE_SCHEDULE_BUTTON_SELECTED_ID));
    nightSelected.setVisible(false);
    nightSelectArea.setPosition(nightSelected.getX(), nightSelected.getY(), nightSelected.getWidth(), nightSelected.getHeight());
    nightSelectArea.setAction(onSelectionAreaPressed);

    dayTemperature.setPosition(daySelected.getX() + 3, 6, 46, 34);
    dayTemperature.setTypedText(TypedText(T_TEMPERATURE_MENU));
    dayTemperature.setWildcard(dayTemperatureTextBuffer);

    nightTemperature.setPosition(dayTemperature.getX() + daySelected.getWidth(), 6, 46, 34);
    nightTemperature.setTypedText(TypedText(T_TEMPERATURE_MENU));
    nightTemperature.setWildcard(nightTemperatureTextBuffer);

    Unicode::snprintf(dayTemperatureTextBuffer, 5, "%d", 1);
    Unicode::snprintf(nightTemperatureTextBuffer, 5, "%d", 2);

    itemSelectArea.setPosition(0, 0, daySelectArea.getX(), background.getHeight());
    itemSelectArea.setAction(onSelectionAreaPressed);

    currentSelectedIsDay = true;
    setSelected(false);

    add(background);
    add(daySelected);
    add(nightSelected);
    add(weekdayText);
    add(dayTemperature);
    add(nightTemperature);

    add(itemSelectArea);
    add(daySelectArea);
    add(nightSelectArea);

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

TemperatureScheduleMenuItem::~TemperatureScheduleMenuItem()
{


}

void TemperatureScheduleMenuItem::selectionAreaPressedhandler(const AbstractButton& button)
{
    if (&button == &daySelectArea)
    {
        currentSelectedIsDay = true;
    }
    else if (&button == &nightSelectArea)
    {
        currentSelectedIsDay = false;
    }

    if (&button == &itemSelectArea && !selected)
    {
        currentSelectedIsDay = true;
    }

    if (menuItemSelectedCallback)
    {
        menuItemSelectedCallback->execute(*this);
    }
}

void TemperatureScheduleMenuItem::sliderUpdatedHandler(int16_t value)
{
    if (currentSelectedIsDay)
    {
        if (value != dayTemperatureValue)
        {
            dayTemperatureValue = value;
            updateTemperatureText();

            if (valueUpdatedCallback)
            {
                valueUpdatedCallback->execute(*this);
            }
        }
    }
    else
    {
        if (value != nightTemperatureValue)
        {
            nightTemperatureValue = value;
            updateTemperatureText();

            if (valueUpdatedCallback)
            {
                valueUpdatedCallback->execute(*this);
            }
        }
    }
}

void TemperatureScheduleMenuItem::setSelected(bool sel)
{
    // Toggle selection from unselected to selected
    if (sel && !selected)
    {
        slider->setValueChangedCallback(onSliderUpdated);

        if (currentSelectedIsDay)
        {
            slider->setValue(dayTemperatureValue);
        }
        else
        {
            slider->setValue(nightTemperatureValue);
        }
        slider->invalidate();
    }


    selected = sel;

    if (selected)
    {
        background.setBitmap(Bitmap(BITMAP_TEMPERATURE_SCHEDULE_MENU_ITEM_SELECTED_ID));
        weekdayText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        dayTemperature.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        nightTemperature.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        daySelected.setVisible(currentSelectedIsDay);
        nightSelected.setVisible(!currentSelectedIsDay);

        slider->setValue(currentSelectedIsDay ? dayTemperatureValue : nightTemperatureValue);
    }
    else
    {
        background.setBitmap(Bitmap(BITMAP_TEMPERATURE_SCHEDULE_MENU_ITEM_ID));
        weekdayText.setColor(Color::getColorFrom24BitRGB(0xA9, 0xAD, 0xB6));
        dayTemperature.setColor(Color::getColorFrom24BitRGB(0xA9, 0xAD, 0xB6));
        nightTemperature.setColor(Color::getColorFrom24BitRGB(0xA9, 0xAD, 0xB6));
        daySelected.setVisible(false);
        nightSelected.setVisible(false);

    }
    background.invalidate();
    weekdayText.invalidate();
    dayTemperature.invalidate();
    nightTemperature.invalidate();
}

void TemperatureScheduleMenuItem::setWeekday(RoomTemperatureInfo::weekday day)
{
    weekday = day;
    weekdayText.setTypedText(TypedText(T_TEMPERATURE_MONDAY + (uint8_t)day));
    weekdayText.invalidate();
}

void TemperatureScheduleMenuItem::setRoomTemperatureInfo(RoomTemperatureInfo& info)
{
    dayTemperatureValue = info.getScheduleDayTemp(weekday);
    nightTemperatureValue = info.getScheduleNightTemp(weekday);
    updateTemperatureText();
}

void TemperatureScheduleMenuItem::updateTemperatureText()
{
    Unicode::snprintf(dayTemperatureTextBuffer, 5, "%d", dayTemperatureValue);
    Unicode::snprintf(nightTemperatureTextBuffer, 5, "%d", nightTemperatureValue);
    dayTemperature.invalidate();
    nightTemperature.invalidate();
}

void TemperatureScheduleMenuItem::setAlpha(int16_t alpha)
{
    alpha = (alpha < 0) ? 0 : alpha;
    alpha = (alpha > 255) ? 255 : alpha;

    weekdayText.setAlpha((uint8_t) alpha);
    dayTemperature.setAlpha((uint8_t) alpha);
    nightTemperature.setAlpha((uint8_t) alpha);
    slider->setAlpha((uint8_t) alpha);
    invalidate();
}
