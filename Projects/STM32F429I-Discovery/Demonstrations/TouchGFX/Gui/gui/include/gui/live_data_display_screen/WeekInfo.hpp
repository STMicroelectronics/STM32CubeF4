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
