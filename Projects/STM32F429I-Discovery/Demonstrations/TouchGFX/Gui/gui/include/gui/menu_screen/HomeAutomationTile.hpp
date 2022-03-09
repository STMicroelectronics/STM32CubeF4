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
  


#ifndef HOMEAUTOMATIONTILE_HPP_
#define HOMEAUTOMATIONTILE_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/model/RoomTemperatureInfo.hpp>
#include <gui/common/FrontendApplication.hpp>

using namespace touchgfx;

class HomeAutomationTile : public Container
{
public:
    HomeAutomationTile();
    virtual ~HomeAutomationTile();

    virtual void handleTickEvent();

    int16_t getWidth()
    {
        return Container::getWidth();
    }
    int16_t getHeight()
    {
        return Container::getHeight();
    }

    void gotoAssociatedScreen()
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoHomeAutomationScreen();
    }

    void addRoom(RoomTemperatureInfo& room);
    void initialize();

private:
    enum States
    {
        ANIMATE_TEXT_OUT,
        ANIMATE_TEXT_IN,
        NO_ANIMATION
    } currentState;

    uint8_t animationCounter;

    Box background;
    Image hiddenBackground;

    int currentRoomIndex;
    TextArea roomName;
    TextArea roomNameDropShadow;
    TextAreaWithOneWildcard temperature;
    TextAreaWithOneWildcard temperatureDropShadow;
    Unicode::UnicodeChar temperatureBuffer[7];
    int tickCounter;

    int dropShadowOffsetX;
    int dropShadowOffsetY;

    TextArea tileId;

    int numberOfRooms;
    RoomTemperatureInfo rooms[10];

    int roomNameY;
    int temperatureY;

    void animateTextOut();
    void animateTextIn();
    void setCurrentState(States newState);

    void setRoom(TEXTS newRoom);
    void setTemperature(int16_t newTemperature);
};

#endif /* HOMEAUTOMATIONTILE_HPP_ */
