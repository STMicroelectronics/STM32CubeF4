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
  


#include <gui/menu_screen/HomeAutomationTile.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Color.hpp>

HomeAutomationTile::HomeAutomationTile() :
    currentState(NO_ANIMATION),
    animationCounter(0),
    currentRoomIndex(0),
    tickCounter(0),
    numberOfRooms(0)
{
    Application::getInstance()->registerTimerWidget(this);

    setWidth(HAL::DISPLAY_WIDTH - 2 * 20);
    setHeight(60);

    dropShadowOffsetX = 2;
    dropShadowOffsetY = 2;

    roomNameY = 6;
    roomName.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    roomName.setPosition(10, 6, 160, 40);
    roomNameDropShadow.setColor(Color::getColorFrom24BitRGB(0x0A, 0x90, 0x0B));
    roomNameDropShadow.setPosition(roomName.getX() + dropShadowOffsetX, roomName.getY() + dropShadowOffsetY, roomName.getWidth(), roomName.getHeight());

    temperatureY = 6;
    temperature.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    temperature.setTypedText(TypedText(T_TEMPERATUREINDICATORTILE));
    temperature.setPosition(122, 6, 70, 40);
    Unicode::snprintf(temperatureBuffer, 4, "%d", 0);
    temperature.setWildcard(temperatureBuffer);

    temperatureDropShadow.setColor(Color::getColorFrom24BitRGB(0x0A, 0x90, 0x0B));
    temperatureDropShadow.setTypedText(TypedText(T_TEMPERATUREINDICATORTILE));
    temperatureDropShadow.setPosition(temperature.getX() + dropShadowOffsetX, temperature.getY() + dropShadowOffsetY, temperature.getWidth(), temperature.getHeight());
    Unicode::snprintf(temperatureBuffer, 4, "%d", 0);
    temperatureDropShadow.setWildcard(temperatureBuffer);

    tileId.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    tileId.setTypedText(TypedText(T_HOME_AUTOMATION_TILE));
    tileId.setPosition(10, getHeight() - 20, 100, 20);

    add(roomNameDropShadow);
    add(roomName);
    add(temperatureDropShadow);
    add(temperature);
    add(tileId);
}

HomeAutomationTile::~HomeAutomationTile()
{
    Application::getInstance()->unregisterTimerWidget(this);
}

void HomeAutomationTile::addRoom(RoomTemperatureInfo& room)
{
    rooms[numberOfRooms++] = room;
}

void HomeAutomationTile::setTemperature(int16_t newTemperature)
{
    Unicode::snprintf(temperatureBuffer, 7, "%d", newTemperature);
    temperature.invalidate();
    temperatureDropShadow.invalidate();
}

void HomeAutomationTile::setRoom(TEXTS newRoom)
{
    roomName.setTypedText(TypedText(newRoom));
    roomNameDropShadow.setTypedText(TypedText(newRoom));
    roomName.invalidate();
    roomNameDropShadow.invalidate();
}

void HomeAutomationTile::handleTickEvent()
{
    tickCounter++;

    if (tickCounter % 250 == 0)
    {
        currentRoomIndex = (currentRoomIndex + 1) % numberOfRooms;

        setCurrentState(ANIMATE_TEXT_OUT);
    }

    if (currentState == ANIMATE_TEXT_OUT)
    {
        animateTextOut();
    }
    else if (currentState == ANIMATE_TEXT_IN)
    {
        animateTextIn();
    }
}

void HomeAutomationTile::setCurrentState(States newState)
{
    animationCounter = 0;
    currentState = newState;
}

void HomeAutomationTile::animateTextOut()
{
    uint8_t duration = 30;
    uint8_t waitDuration = 14;

    if (animationCounter <= duration)
    {
        uint8_t delta = (uint8_t) EasingEquations::cubicEaseIn(animationCounter, 0, getHeight(), duration);
        roomName.moveTo(roomName.getX(), roomNameY - delta);
        roomNameDropShadow.moveTo(roomNameDropShadow.getX(), roomNameY - delta + 1);
        temperature.moveTo(temperature.getX(), temperatureY + delta);
        temperatureDropShadow.moveTo(temperatureDropShadow.getX(), temperatureY + delta + 1);
    }
    else if (animationCounter <= duration + waitDuration)
    {
        // Just wait
    }
    else
    {
        // Final step: stop the animation
        setRoom(rooms[currentRoomIndex].getRoomName());
        setTemperature(rooms[currentRoomIndex].getCurrentTemperature());

        setCurrentState(ANIMATE_TEXT_IN);
        return;
    }
    animationCounter++;
}

void HomeAutomationTile::animateTextIn()
{
    uint8_t duration = 30;

    if (animationCounter <= duration)
    {
        uint8_t delta = (uint8_t) EasingEquations::cubicEaseOut(animationCounter, 0, getHeight(), duration);
        roomName.moveTo(roomName.getX(), -getHeight() + roomNameY + delta);
        roomNameDropShadow.moveTo(roomNameDropShadow.getX(), -getHeight() + roomNameY + delta + dropShadowOffsetY);
        temperature.moveTo(temperature.getX(), getHeight() + temperatureY - delta);
        temperatureDropShadow.moveTo(temperatureDropShadow.getX(), getHeight() + temperatureY - delta + dropShadowOffsetY);
    }
    else
    {
        // Final step: stop the animation
        setCurrentState(NO_ANIMATION);
        return;
    }
    animationCounter++;
}

void HomeAutomationTile::initialize()
{
    setRoom(rooms[currentRoomIndex].getRoomName());
    setTemperature(rooms[currentRoomIndex].getCurrentTemperature());
}

