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
  


#ifndef CITY_INFO_HPP_
#define CITY_INFO_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

using namespace touchgfx;

class CityInfo : public Container
{
public:
    enum Cities
    {
        COPENHAGEN,
        HONG_KONG,
        MUMBAI,
        NEW_YORK,
        NUMBER_OF_CITIES
    } city;


    CityInfo();

    void setBitmap(BitmapId bg);

    // Set the text color of background related text
    void setTextColor(colortype color)
    {
        textColor = color;
    }
    colortype getTextColor()
    {
        return textColor;
    }

    void setCity(Cities c);
    Cities getCity()
    {
        return city;
    }

    void setTemperature(int16_t newTemperature);
    int16_t getTemperature()
    {
        return currentTemperature;
    }

    // Randomly adjust temperature.
    void adjustTemperature();

private:
    colortype textColor;

    Image background;

    TextArea cityName;
    TextArea cityNameDropShadow;
    TextArea timeAndDate;
    TextArea timeAndDateDropShadow;

    TextAreaWithOneWildcard largeTemperature;
    TextAreaWithOneWildcard largeTemperatureDropShadow;
    Unicode::UnicodeChar largeTemperatureBuffer[5];

    int16_t startTemperature;
    int16_t currentTemperature;
};

#endif /* CITY_INFO_HPP_ */
