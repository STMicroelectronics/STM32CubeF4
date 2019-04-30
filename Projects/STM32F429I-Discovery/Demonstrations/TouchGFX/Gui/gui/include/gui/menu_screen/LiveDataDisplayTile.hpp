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
  


#ifndef LIVEDATADISPLAYTILE_HPP_
#define LIVEDATADISPLAYTILE_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/FrontendApplication.hpp>

using namespace touchgfx;

class LiveDataDisplayTile : public Container
{
public:
    LiveDataDisplayTile();
    virtual ~LiveDataDisplayTile();

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
        static_cast<FrontendApplication*>(Application::getInstance())->gotoLiveDataDisplayScreen();
    }

private:
    enum States
    {
        ANIMATE_TEXT_OUT,
        ANIMATE_BACKGROUND,
        NO_ANIMATION
    } currentState;

    uint8_t animationCounter;

    Image background;
    Image hiddenBackground;

    Container textContainer;

    TextArea cityName;
    TextArea cityNameDropShadow;

    TextAreaWithOneWildcard largeTemperature;
    TextAreaWithOneWildcard largeTemperatureDropShadow;
    Unicode::UnicodeChar largeTemperatureBuffer[5];
    int tickCounter;

    TextArea tileId;


    static const int16_t NUMBER_OF_CITIES = 3;
    int currentCityIndex;
    TEXTS cities[NUMBER_OF_CITIES];
    int temperatures[NUMBER_OF_CITIES];
    BitmapId backgrounds[NUMBER_OF_CITIES];

    int textContainerX;

    void animateTextOut();
    void animateBackground();
    void setCurrentState(States newState);

    void setTemperature(int16_t newTemperature);
    void setCity(TEXTS newCity);
    void updateBackground();

};

#endif /* LIVEDATADISPLAYTILE_HPP_ */
