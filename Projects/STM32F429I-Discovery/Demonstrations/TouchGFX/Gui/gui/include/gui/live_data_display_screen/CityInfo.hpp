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
