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
  


#include <gui/home_automation_screen/TemperatureSlider.hpp>


TemperatureSlider::TemperatureSlider() :
    valueChangedCallback(0)
{
    setTouchable(true);

    slider.setBitmap(Bitmap(BITMAP_TEMPERATURE_SLIDER_KNOP_ID));
    slider.setXY(0, 0);
    sliderRadius = slider.getWidth() / 2;

    sliderBackground.setBitmap(Bitmap(BITMAP_TEMPERATURE_SLIDER_ID));
    sliderBackground.setXY(0, sliderRadius - sliderBackground.getHeight() / 2);

    // Set the containers dimensions
    setWidth(sliderBackground.getWidth());
    setHeight(slider.getHeight());

    add(sliderBackground);
    add(slider);

    setSlider(30, true);
}

TemperatureSlider::~TemperatureSlider()
{

}

void TemperatureSlider::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        setSlider(evt.getX(), true);
    }
}

void TemperatureSlider::handleDragEvent(const DragEvent& evt)
{
    setSlider(evt.getNewX(), true);
}

void TemperatureSlider::setLimits(int16_t min, int16_t max)
{
    assert(min < max);
    minLimit = min;
    maxLimit = max;
}

void TemperatureSlider::setValue(int16_t value)
{
    // Round value to the limits if necessary
    value = (value < minLimit) ? minLimit : value;
    value = (value > maxLimit) ? maxLimit : value;

    float placementPercentage = ((float)(value - minLimit) / (maxLimit - minLimit));


    int16_t interval = getWidth() - sliderRadius - sliderRadius;
    setSlider(sliderRadius + (int16_t)(placementPercentage * interval), false);
}

void TemperatureSlider::setSlider(int16_t x, bool callback)
{
    // Cut off x values outside the slider area
    if (x < sliderRadius)
    {
        x = sliderRadius;
    }
    else if (x > getWidth() - sliderRadius)
    {
        x = getWidth() - sliderRadius;
    }
    invalidate();

    slider.moveTo(x - sliderRadius, 0);

    invalidate();

    // Communicate the new value if a listener is registered
    if (valueChangedCallback && callback)
    {
        valueChangedCallback->execute(getValue());
    }

}

int16_t TemperatureSlider::getValue()
{
    int16_t interval = getWidth() - sliderRadius - sliderRadius;
    float placementPercentage = (float) slider.getX() / interval;

    return minLimit + ((int16_t)(placementPercentage * (maxLimit - minLimit)));
}

void TemperatureSlider::setAlpha(uint8_t alpha)
{
    sliderBackground.setAlpha(alpha);
    slider.setAlpha(alpha);
    sliderBackground.invalidate();
    slider.invalidate();
}
