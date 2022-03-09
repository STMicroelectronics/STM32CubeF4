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
  


#include <gui/custom_controls_screen/ThreeWayProgressBar.hpp>
#include <BitmapDatabase.hpp>
#include <assert.h>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ThreeWayProgressBar::ThreeWayProgressBar() :
    buttonClickedAction(0),
    buttonPressedCallback(this, &ThreeWayProgressBar::buttonPressedHandler)
{
    background.setBitmap(Bitmap(BITMAP_CONTROLS_WHEEL_BACKGROUND_ID));
    background.setXY(14, 14);
    add(background);

    centerButton.setBitmaps(Bitmap(BITMAP_CONTROL_CENTER_BUTTON_ID), Bitmap(BITMAP_CONTROL_CENTER_BUTTON_PRESSED_ID));
    centerButton.setXY(background.getX() + (background.getWidth() - centerButton.getWidth()) / 2, background.getY() + (background.getHeight() - centerButton.getHeight()) / 2);
    centerButton.setAction(buttonPressedCallback);
    centerButton.setLabelColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    centerButton.setLabelColorPressed(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(centerButton);

    bitmapPainter.setBitmap(Bitmap(BITMAP_CONTROL_COLOR_WHEEL_ID));

    radius = 103;

    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        bars[i].setPosition(background.getX(), background.getY(), background.getWidth(), background.getHeight());
        bars[i].setCenter(bars[i].getWidth() / 2.0f, bars[i].getHeight() / 2.0f);
        bars[i].setRadius(radius);
        bars[i].setLineWidth(25);
        bars[i].setPainter(bitmapPainter);
        bars[i].setCapPrecision(15);
        bars[i].setArc(i * 120, i * 120);
        bars[i].setVisible(false);
        add(bars[i]);
    }

    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        add(circles[i]);
    }

    setWidth(background.getWidth() + 28);
    setHeight(background.getHeight() + 28);

    reset();
}

ThreeWayProgressBar::~ThreeWayProgressBar()
{
}

void ThreeWayProgressBar::reset()
{
    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        // Center circles before animating out
        circles[i].moveTo(background.getX() + ((background.getWidth() - circles[i].getWidth()) / 2) + (-30 + ((i + 1) % 3) * 30), background.getY() + (background.getHeight() - circles[i].getHeight()) / 2);
        circles[i].setPercentage(0);
    }

    circles[0].setIcon(BITMAP_SMALL_CIRCLE_BIN_ICON_INACTIVE_ID, BITMAP_SMALL_CIRCLE_BIN_ICON_ACTIVE_ID);
    circles[1].setIcon(BITMAP_SMALL_CIRCLE_PAPERS_ICON_INACTIVE_ID, BITMAP_SMALL_CIRCLE_PAPERS_ICON_ACTIVE_ID);
    circles[2].setIcon(BITMAP_SMALL_CIRCLE_FOLDER_ICON_INACTIVE_ID, BITMAP_SMALL_CIRCLE_FOLDER_ICON_ACTIVE_ID);

    circles[0].startMoveAnimation(circles[0].getX(), circles[0].getY() - radius, 30, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
    circles[1].startMoveAnimation(background.getX() + background.getWidth() - circles[1].getWidth(), 158, 30, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);
    circles[2].startMoveAnimation(background.getX(), 158, 30, EasingEquations::cubicEaseInOut, EasingEquations::cubicEaseInOut);

    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        bars[i].setVisible(false);
        circles[i].setActive(false);
    }

    setActive(false);
}


void ThreeWayProgressBar::stopAnimation()
{
    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        circles[i].cancelMoveAnimation();
    }
}


void ThreeWayProgressBar::buttonPressedHandler(const AbstractButton& button)
{
    if (buttonClickedAction && buttonClickedAction->isValid())
    {
        buttonClickedAction->execute(*this);
    }
}

void ThreeWayProgressBar::setActive(bool active)
{
    centerButton.setTouchable(!active);

    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        if (active)
        {
            bars[i].setVisible(true);
            setBarPercentage(i, 0);
        }
        circles[i].setActive(active);
    }

    centerButton.setLabelText(TypedText((active) ? T_THREE_WAY_PROGRESS_BAR_SCANNING : T_THREE_WAY_PROGRESS_BAR_SCAN));

    invalidate();
}

void ThreeWayProgressBar::setBarPercentage(int barIndex, int percentage)
{
    if (percentage > 100)
    {
        percentage = 100;
    }

    bars[barIndex].updateArcEnd((int)((barIndex * 120) + (120 * percentage / 100.f)));
    circles[barIndex].setPercentage(percentage);
}





