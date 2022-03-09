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
  


#include <gui/custom_controls_screen/Gauge.hpp>
#include <touchgfx/Color.hpp>

Gauge::Gauge() :
    gaugeAnimationRunning(false),
    gaugeAnimationDuration(10),
    easingEquation(EasingEquations::backEaseOut)
{
    add(background);
    add(needleShadow);
    add(needle);
    add(center);
}

Gauge::~Gauge()
{
}


void Gauge::setBitmaps(Bitmap backgroundBmp, Bitmap centerBmp)
{
    background.setBitmap(backgroundBmp);
    background.setXY(0, 0);

    center.setBitmap(centerBmp);
    center.setXY((background.getWidth() - center.getWidth()) / 2, (background.getHeight() - center.getHeight()) / 2);

    colorPainter.setColor(Color::getColorFrom24BitRGB(0xAE, 0x18, 0x24));
    needle.setPosition(0, 0, background.getWidth(), background.getHeight());
    needle.setOrigin(background.getWidth() / 2, background.getHeight() / 2);
    needle.setPainter(colorPainter);

    shadowPainter.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0), 60);
    needleShadow.setPosition(0, 0, background.getWidth(), background.getHeight());
    needleShadow.setOrigin(background.getWidth() / 2, background.getHeight() / 2);
    needleShadow.setPainter(shadowPainter);


    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

void Gauge::setLimits(int newMinValue, int newMaxValue, int newMinDegree, int newMaxDegree)
{
    minValue = newMinValue;
    maxValue = newMaxValue;
    minDegree = newMinDegree;
    maxDegree = newMaxDegree;

    degreesPerValue = ((maxDegree - minDegree) / ((float)maxValue - minValue));

    currentValue = minValue;
    needle.updateAngle((int)((currentValue - minValue) * degreesPerValue) + minDegree);
    needleShadow.updateAngle((int)((currentValue - minValue) * degreesPerValue) + minDegree);
}

void Gauge::setValue(int newValue)
{
    if (newValue > maxValue)
    {
        newValue = maxValue;
    }
    else if (newValue < minValue)
    {
        newValue = minValue;
    }
    deltaValue = newValue - currentValue;

    Application::getInstance()->registerTimerWidget(this);
    gaugeAnimationRunning = true;
    gaugeAnimationCounter = 0;
}

int Gauge::getValue()
{
    return currentValue;
}

void Gauge::setAnimationDuration(uint16_t animationDuration)
{
    gaugeAnimationDuration = animationDuration;
}

void Gauge::setEasingEquation(EasingEquation newEasingEquation)
{
    easingEquation = newEasingEquation;
}

void Gauge::handleTickEvent()
{
    if (gaugeAnimationRunning)
    {
        if (gaugeAnimationCounter <= gaugeAnimationDuration)
        {
            int16_t delta = easingEquation(gaugeAnimationCounter, 0, deltaValue, gaugeAnimationDuration);

            needle.updateAngle((int)(((currentValue + delta) - minValue) * degreesPerValue)  + minDegree);
            needleShadow.updateAngle((int)(((currentValue + delta) - minValue) * degreesPerValue)  + minDegree);
            gaugeAnimationCounter++;
        }
        else
        {
            // End of animation
            gaugeAnimationRunning = false;
            gaugeAnimationCounter = 0;
            Application::getInstance()->unregisterTimerWidget(this);

            currentValue = currentValue + deltaValue;
        }
    }
}
