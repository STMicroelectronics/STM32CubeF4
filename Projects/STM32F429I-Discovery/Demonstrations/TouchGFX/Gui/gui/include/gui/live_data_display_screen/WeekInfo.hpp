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
  


#ifndef WEEK_INFO_HPP_
#define WEEK_INFO_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/live_data_display_screen/CityInfo.hpp>

using namespace touchgfx;

class WeekInfo : public Container
{
public:
    WeekInfo();

    void setAlpha(int alpha);
    void setTextColor(colortype color);

    void setInfo(CityInfo::Cities city);

private:
    static const int NUMBER_OF_DAYS = 3;

    Image smallIcons[NUMBER_OF_DAYS];
    TextArea days[NUMBER_OF_DAYS];
    TextAreaWithOneWildcard smallTemperature[NUMBER_OF_DAYS];
    TextAreaWithOneWildcard smallTemperatureDropShadow[NUMBER_OF_DAYS];
    Unicode::UnicodeChar smallTemperatureBuffer[NUMBER_OF_DAYS][5];
};

#endif /* WEEK_INFO_HPP_ */
