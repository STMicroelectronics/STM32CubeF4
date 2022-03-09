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
  


#include <gui/custom_controls_screen/DatePicker.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

DatePicker::DatePicker() :
    onSelectedElementChanged(this, &DatePicker::selectedElementChangedHandler)
{
    background.setBitmap(Bitmap(BITMAP_DATEPICKER_MAIN_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    colortype normalTextColor = Color::getColorFrom24BitRGB(0x31, 0x31, 0x31);
    colortype selectedTextColor = Color::getColorFrom24BitRGB(0x00, 0x8A, 0xFF);
    colortype selectedBackgroundColor = Color::getColorFrom24BitRGB(0xF5, 0xF5, 0xF5);

    days.setXY(13, 11);
    days.setupDatePickerWheelDay(49, 156, 10, 72, T_DATEPICKER_DAY_28, T_DATEPICKER_DAY_29, T_DATEPICKER_DAY_30, T_DATEPICKER_DAY_31);
    days.setTextColor(normalTextColor, selectedTextColor, selectedBackgroundColor, 65, 32);
    add(days);

    months.setXY(70, 11);
    months.setup(129, 156, 10, 72, T_DATEPICKER_MONTHS);
    months.setTextColor(normalTextColor, selectedTextColor, selectedBackgroundColor, 65, 32);
    months.setElementSelectedCallback(onSelectedElementChanged);
    add(months);

    years.setXY(207, 11);
    years.setup(79, 156, 10, 72, T_DATEPICKER_YEARS);
    years.setTextColor(normalTextColor, selectedTextColor, selectedBackgroundColor, 65, 32);
    years.setElementSelectedCallback(onSelectedElementChanged);
    add(years);

    shadowTop.setBitmap(Bitmap(BITMAP_DATEPICKER_TOP_SHADOW_OVERLAY_ID));
    shadowTop.setXY(9, 11);
    add(shadowTop);

    shadowBottom.setBitmap(Bitmap(BITMAP_DATEPICKER_BOTTOM_SHADOW_OVERLAY_ID));
    shadowBottom.setXY(9, background.getHeight() - shadowBottom.getHeight() - 11);
    add(shadowBottom);

    glassOverlay.setBitmap(Bitmap(BITMAP_DATEPICKER_GLASS_OVERLAY_ID));
    glassOverlay.setXY(9, (background.getHeight() - glassOverlay.getHeight()) / 2 + 3);
    add(glassOverlay);

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

DatePicker::~DatePicker()
{

}

void DatePicker::setday(int index, int duration, EasingEquation equation)
{
    days.setSelectedIndex(index, duration, equation);
}

void DatePicker::setMonth(int index, int duration, EasingEquation equation)
{
    months.setSelectedIndex(index, duration, equation);
}

void DatePicker::setYear(int index, int duration, EasingEquation equation)
{
    years.setSelectedIndex(index, duration, equation);
}

void DatePicker::reset()
{
    days.reset();
    months.reset();
    years.reset();
}

void DatePicker::selectedElementChangedHandler(const WheelSelector& wheel, const int& index)
{
    // Adjust the number of days in the selected month/year
    int numberOfDaysInMonth = getNumberOfDays(months.getSelectedIndex() + 1, START_YEAR + years.getSelectedIndex());

    if (numberOfDaysInMonth == 28)
    {
        days.fadeExtraText(0, 0, 16);
        days.fadeExtraText(1, 0, 16);
        days.fadeExtraText(2, 0, 16);
        if (days.getSelectedIndex() > 27)
        {
            days.setSelectedIndex(27, 20);
        }
    }
    else if (numberOfDaysInMonth == 29)
    {
        days.fadeExtraText(0, 255, 16);
        days.fadeExtraText(1, 0, 16);
        days.fadeExtraText(2, 0, 16);
        if (days.getSelectedIndex() > 28)
        {
            days.setSelectedIndex(28, 20);
        }
    }
    else if (numberOfDaysInMonth == 30)
    {
        days.fadeExtraText(0, 255, 16);
        days.fadeExtraText(1, 255, 16);
        days.fadeExtraText(2, 0, 16);
        if (days.getSelectedIndex() > 29)
        {
            days.setSelectedIndex(29, 20);
        }
    }
    else
    {
        days.fadeExtraText(0, 255, 16);
        days.fadeExtraText(1, 255, 16);
        days.fadeExtraText(2, 255, 16);
    }
}

uint16_t DatePicker::getNumberOfDays(uint16_t month, uint16_t year)
{
    uint16_t result = 0;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        result = 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        result = 30;
    }
    else if (month == 2)
    {
        result = 28;
        if (year % 400 == 0)
        {
            result++;
        }
        else if (year % 100 == 0)
        {
        }
        else if (year % 4 == 0)
        {
            result++;
        }
    }
    return result;
}

