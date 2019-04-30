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
  


#ifndef TEMPERATURE_SCHEDULE_MENU_ITEM_HPP_
#define TEMPERATURE_SCHEDULE_MENU_ITEM_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <gui/model/RoomTemperatureInfo.hpp>
#include <gui/home_automation_screen/TemperatureSlider.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class TemperatureScheduleMenuItem : public Container
{
public:
    TemperatureScheduleMenuItem();
    virtual ~TemperatureScheduleMenuItem();

    void setMenuItemSelectedCallback(GenericCallback< const TemperatureScheduleMenuItem& >& callback)
    {
        menuItemSelectedCallback = &callback;
    }
    void setMenuItemValueUpdatedCallback(GenericCallback< const TemperatureScheduleMenuItem& >& callback)
    {
        valueUpdatedCallback = &callback;
    }

    void setWeekday(RoomTemperatureInfo::weekday day);
    RoomTemperatureInfo::weekday getWeekday()
    {
        return weekday;
    }

    void setRoomTemperatureInfo(RoomTemperatureInfo& info);

    void setSelected(bool sel);

    void setAlpha(int16_t alpha);

    int16_t getDayTemperature()
    {
        return dayTemperatureValue;
    }
    int16_t getNightTemperature()
    {
        return nightTemperatureValue;
    }

    void setSlider(TemperatureSlider* temperatureSlider)
    {
        slider = temperatureSlider;
    }

private:
    RoomTemperatureInfo::weekday weekday;

    Image background;
    TextArea weekdayText;
    TextAreaWithOneWildcard dayTemperature;
    Unicode::UnicodeChar dayTemperatureTextBuffer[5];
    TextAreaWithOneWildcard nightTemperature;
    Unicode::UnicodeChar nightTemperatureTextBuffer[5];

    TouchArea itemSelectArea;
    Image daySelected;
    TouchArea daySelectArea;
    Image nightSelected;
    TouchArea nightSelectArea;
    bool currentSelectedIsDay;

    int16_t dayTemperatureValue;
    int16_t nightTemperatureValue;

    TemperatureSlider* slider;

    bool selected;

    GenericCallback< const TemperatureScheduleMenuItem& >* menuItemSelectedCallback;
    GenericCallback< const TemperatureScheduleMenuItem& >* valueUpdatedCallback;

    Callback<TemperatureScheduleMenuItem, const AbstractButton&> onSelectionAreaPressed;
    Callback<TemperatureScheduleMenuItem, int16_t> onSliderUpdated;

    void selectionAreaPressedhandler(const AbstractButton& button);
    void sliderUpdatedHandler(int16_t value);

    void updateTemperatureText();

};


#endif /* TEMPERATURE_SCHEDULE_MENU_ITEM_HPP_ */
