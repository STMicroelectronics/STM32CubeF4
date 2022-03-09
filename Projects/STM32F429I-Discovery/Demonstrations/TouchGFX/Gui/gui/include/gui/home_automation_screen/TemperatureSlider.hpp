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
  


#ifndef TEMPERATURE_SLIDER_HPP_
#define TEMPERATURE_SLIDER_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class TemperatureSlider : public Container
{
public:
    TemperatureSlider();
    virtual ~TemperatureSlider();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void setValueChangedCallback(GenericCallback< int16_t >& callback)
    {
        valueChangedCallback = &callback;
    }

    void setLimits(int16_t min, int16_t max);
    void setValue(int16_t value);

    int16_t getValue();

    void setAlpha(uint8_t alpha);

private:
    Image sliderBackground;
    Image slider;

    int16_t minLimit;
    int16_t maxLimit;

    uint8_t sliderRadius;

    GenericCallback< int16_t >* valueChangedCallback;

    void setSlider(int16_t x, bool callback);
};


#endif /* TEMPERATURE_SLIDER_HPP_ */
