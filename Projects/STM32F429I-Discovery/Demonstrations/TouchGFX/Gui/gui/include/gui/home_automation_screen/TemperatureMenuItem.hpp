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
  


#ifndef TEMPERATURE_MENU_ITEM_HPP_
#define TEMPERATURE_MENU_ITEM_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/model/RoomTemperatureInfo.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class TemperatureMenuItem : public Container
{
public:
    TemperatureMenuItem();
    virtual ~TemperatureMenuItem();

    void setMenuItemSelectedCallback(GenericCallback< const TemperatureMenuItem& >& callback)
    {
        menuItemSelectedCallback = &callback;
    }

    void setRoomTemperatureInfo(RoomTemperatureInfo& info);

    void setSelected(bool sel);

    void setRoomId(uint8_t id)
    {
        roomId = id;
    }
    uint8_t getRoomId() const
    {
        return roomId;
    }

    void setTextsAlpha(int16_t alpha);

private:
    uint8_t roomId;

    Image background;
    TextArea primaryText;
    TextAreaWithOneWildcard currentTemperature;
    Unicode::UnicodeChar currentTemperatureTextBuffer[5];

    TouchArea itemSelectArea;

    bool selected;

    GenericCallback< const TemperatureMenuItem& >* menuItemSelectedCallback;

    Callback<TemperatureMenuItem, const AbstractButton&> onButtonPressed;

    void buttonPressedhandler(const AbstractButton& button);
};


#endif /* TEMPERATURE_MENU_ITEM_HPP_ */
