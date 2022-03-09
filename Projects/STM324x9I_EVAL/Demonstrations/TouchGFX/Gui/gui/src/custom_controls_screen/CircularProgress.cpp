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
  


#include <gui/custom_controls_screen/CircularProgress.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

CircularProgress::CircularProgress()
{
    setTouchable(true);

    background.setBitmap(Bitmap(BITMAP_CONTROLS_WHEEL_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    centerImage.setBitmap(Bitmap(BITMAP_CONTROL_CENTER_BUTTON_ID));
    centerImage.setXY(background.getX() + (background.getWidth() - centerImage.getWidth()) / 2, background.getY() + (background.getHeight() - centerImage.getHeight()) / 2);
    add(centerImage);

    bitmapPainter.setBitmap(Bitmap(BITMAP_CONTROL_COLOR_WHEEL_ID));
    colorPainter.setColor(Color::getColorFrom24BitRGB(0x11, 0x2C, 0x3C), 70);

    int radius = 103;

    bar.setPosition(background.getX(), background.getY(), background.getWidth(), background.getHeight());
    bar.setCenter(bar.getWidth() / 2.0f, bar.getHeight() / 2.0f);
    bar.setRadius(radius);
    bar.setLineWidth(25);
    bar.setPainter(bitmapPainter);
    bar.setCapPrecision(15);
    bar.setArc(START_DEGREE, 0);
    add(bar);

    Unicode::snprintf(percentageTextBuffer, 5, "%d", 100);
    percentageText.setWildcard(percentageTextBuffer);
    percentageText.setTypedText(TypedText(T_PERCENTAGE_BAR_READOUT));
    percentageText.setXY(background.getX() + (background.getWidth() - percentageText.getWidth()) / 2, background.getY() + (background.getHeight() - percentageText.getHeight()) / 2 - 20);
    percentageText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(percentageText);

    headlineText.setTypedText(TypedText(T_PERCENTAGE_BAR_HEADLINE));
    headlineText.setXY(background.getX() + (background.getWidth() - headlineText.getWidth()) / 2, percentageText.getY() + 60);
    headlineText.setColor(Color::getColorFrom24BitRGB(0x17, 0x3C, 0x51));
    add(headlineText);

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

CircularProgress::~CircularProgress()
{

}

void CircularProgress::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        updateBar(evt.getX(), evt.getY());
    }
}

void CircularProgress::handleDragEvent(const DragEvent& evt)
{
    updateBar(evt.getNewX(), evt.getNewY());
}

void CircularProgress::updateBar(int newX, int newY)
{
    int centerX = 0;
    int centerY = 0;
    bar.getCenter(centerX, centerY);

    int16_t vx = newX - centerX;
    int16_t vy = newY - centerY;

    int length;
    int angleInDegrees = CWRUtil::angle<int>(vx, vy, length);
    if (length < 50)
    {
        return;
    }

    if (angleInDegrees > 180)
    {
        angleInDegrees -= 360;
    }
    if (angleInDegrees < START_DEGREE - 30)
    {
        return;
    }
    if (angleInDegrees > END_DEGREE + 30)
    {
        return;
    }
    if (angleInDegrees < START_DEGREE)
    {
        angleInDegrees = START_DEGREE;
    }
    if (angleInDegrees > END_DEGREE)
    {
        angleInDegrees = END_DEGREE;
    }

    setBarAngle((int)angleInDegrees);
}

void CircularProgress::setBarAngle(int angleInDegrees)
{
    bar.updateArcEnd((int)angleInDegrees);

    currentPercentage = (int)(100.0f * (angleInDegrees + END_DEGREE) / (END_DEGREE - START_DEGREE));
    Unicode::snprintf(percentageTextBuffer, 5, "%d", currentPercentage);
    percentageText.invalidate();
}
