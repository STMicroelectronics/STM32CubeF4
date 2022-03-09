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
  


#include <gui/menu_screen/LiveDataDisplayTile.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/FrontendApplication.hpp>

LiveDataDisplayTile::LiveDataDisplayTile() :
    currentState(NO_ANIMATION),
    animationCounter(0),
    tickCounter(0),
    currentCityIndex(0)
{
    Application::getInstance()->registerTimerWidget(this);

    setWidth(HAL::DISPLAY_WIDTH - 2 * 20);
    setHeight(110);

    background.setBitmap(Bitmap(BITMAP_WEATHER_BACKGROUND_2_ID));
    background.setXY(getWidth() - background.getWidth(), 0);
    hiddenBackground.setBitmap(Bitmap(BITMAP_WEATHER_BACKGROUND_3_ID));
    hiddenBackground.setXY(-background.getWidth(), 0);

    textContainerX = 16;
    textContainer.setPosition(textContainerX, 6, 120, getHeight() - 6);

    cityName.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    cityName.setPosition(0, 72, 130, 24);

    cityNameDropShadow.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));
    cityNameDropShadow.setPosition(cityName.getX() + 1, cityName.getY() + 1, cityName.getWidth(), cityName.getHeight());
    cityNameDropShadow.setAlpha(128);

    largeTemperature.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    largeTemperature.setTypedText(TypedText(T_WEATHER_TILE_TEMPERATURE));
    largeTemperature.setPosition(0, 0, 100, 80);
    Unicode::snprintf(largeTemperatureBuffer, 4, "%d", 0);
    largeTemperature.setWildcard(largeTemperatureBuffer);

    largeTemperatureDropShadow.setColor(Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
    largeTemperatureDropShadow.setTypedText(TypedText(T_WEATHER_TILE_TEMPERATURE));
    largeTemperatureDropShadow.setPosition(largeTemperature.getX() + 1, largeTemperature.getY() + 2, largeTemperature.getWidth(), largeTemperature.getHeight());
    largeTemperatureDropShadow.setWildcard(largeTemperatureBuffer);
    largeTemperatureDropShadow.setAlpha(128);

    cities[0] = T_WEATHER_CITY_2;
    cities[1] = T_WEATHER_CITY_1;
    cities[2] = T_WEATHER_CITY_3;
    temperatures[0] = 24;
    temperatures[1] = 32;
    temperatures[2] = 16;
    backgrounds[0] = BITMAP_WEATHER_BACKGROUND_2_ID;
    backgrounds[1] = BITMAP_WEATHER_BACKGROUND_3_ID;
    backgrounds[2] = BITMAP_WEATHER_BACKGROUND_1_ID;

    setCity(cities[currentCityIndex]);
    setTemperature(temperatures[currentCityIndex]);
    updateBackground();

    textContainer.add(cityNameDropShadow);
    textContainer.add(cityName);
    textContainer.add(largeTemperatureDropShadow);
    textContainer.add(largeTemperature);

    tileId.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    tileId.setTypedText(TypedText(T_LIVE_DATA_DISPLAY_TILE));
    tileId.setPosition(getWidth() - 54, getHeight() - 24, 50, 20);

    add(background);
    add(hiddenBackground);
    add(textContainer);
    add(tileId);
}

LiveDataDisplayTile::~LiveDataDisplayTile()
{
    Application::getInstance()->unregisterTimerWidget(this);
}

void LiveDataDisplayTile::setTemperature(int16_t newTemperature)
{
    Unicode::snprintf(largeTemperatureBuffer, 4, "%d", newTemperature);
    largeTemperature.invalidate();
    largeTemperatureDropShadow.invalidate();
}

void LiveDataDisplayTile::setCity(TEXTS newCity)
{
    cityName.setTypedText(TypedText(newCity));
    cityNameDropShadow.setTypedText(TypedText(newCity));
    cityName.invalidate();
    cityNameDropShadow.invalidate();
}

void LiveDataDisplayTile::updateBackground()
{
    background.setBitmap(Bitmap(backgrounds[currentCityIndex]));
    background.setXY(getWidth() - background.getWidth(), 0);

    hiddenBackground.setBitmap(Bitmap(backgrounds[(currentCityIndex + 1) % NUMBER_OF_CITIES]));
    hiddenBackground.setXY(-background.getWidth(), 0);

    background.invalidate();
    hiddenBackground.invalidate();
}

void LiveDataDisplayTile::handleTickEvent()
{
    tickCounter++;

    if (tickCounter % 370 == 0)
    {
        currentCityIndex = (currentCityIndex + 1) % NUMBER_OF_CITIES;

        setCurrentState(ANIMATE_TEXT_OUT);
    }

    if (currentState == ANIMATE_TEXT_OUT)
    {
        animateTextOut();
    }
    else if (currentState == ANIMATE_BACKGROUND)
    {
        animateBackground();
    }
}

void LiveDataDisplayTile::setCurrentState(States newState)
{
    animationCounter = 0;
    currentState = newState;
}

void LiveDataDisplayTile::animateTextOut()
{
    uint8_t duration = 20;
    uint8_t waitDuration = 10;

    if (animationCounter <= duration)
    {
        uint8_t delta = (uint8_t) EasingEquations::cubicEaseIn(animationCounter, 0, getWidth(), duration);
        textContainer.moveTo(textContainerX - delta , textContainer.getY());
    }
    else if (animationCounter <= duration + waitDuration)
    {
        // Just wait
    }
    else
    {
        // Final step: stop the animation
        setCity(cities[currentCityIndex]);
        setTemperature(temperatures[currentCityIndex]);

        setCurrentState(ANIMATE_BACKGROUND);
        return;
    }
    animationCounter++;
}

void LiveDataDisplayTile::animateBackground()
{
    uint8_t duration = 20;

    if (animationCounter <= duration)
    {
        uint8_t delta = (uint8_t) EasingEquations::cubicEaseInOut(animationCounter, 0, getWidth(), duration);

        hiddenBackground.moveTo(-background.getWidth() + delta , hiddenBackground.getY());
        background.moveTo(getWidth() - background.getWidth() + delta, background.getY());
        textContainer.moveTo(textContainerX - getWidth() + delta , textContainer.getY());
    }
    else
    {
        // Final step: stop the animation
        updateBackground();

        setCurrentState(NO_ANIMATION);
        return;
    }
    animationCounter++;
}

