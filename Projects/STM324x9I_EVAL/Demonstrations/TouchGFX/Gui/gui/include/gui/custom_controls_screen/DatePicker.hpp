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
  


#ifndef DATE_PICKER_HPP
#define DATE_PICKER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <BitmapDatabase.hpp>
#include <gui/custom_controls_screen/WheelSelector.hpp>
#include <gui/custom_controls_screen/WheelSelectorExtra.hpp>

using namespace touchgfx;

/**
 * @class DatePicker
 *
 * @brief A date picker.
 *        Container with a day, month and year scrollable wheel.
 *
 * @see Container
 */
class DatePicker : public Container
{
public:
    DatePicker();
    virtual ~DatePicker();

    void setday(int index, int duration, EasingEquation equation);
    void setMonth(int index, int duration, EasingEquation equation);
    void setYear(int index, int duration, EasingEquation equation);

    void reset();

protected:
    static const int START_YEAR = 2012;

    Image background;
    Image shadowTop;
    Image shadowBottom;
    Image glassOverlay;

    WheelSelectorExtra days;
    WheelSelector months;
    WheelSelector years;

    Callback<DatePicker, const WheelSelector&, const int&> onSelectedElementChanged;
    void selectedElementChangedHandler(const WheelSelector& wheel, const int& index);

    uint16_t getNumberOfDays(uint16_t month, uint16_t year);

};


#endif /* DATE_PICKER_HPP */
