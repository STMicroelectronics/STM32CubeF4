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
  


#include <gui/custom_controls_screen/CustomControlsView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/Utils.hpp>

CustomControlsView::CustomControlsView() :
    DemoView<CustomControlsPresenter>(),
    onButtonPressed(this, &CustomControlsView::buttonPressedHandler),
    slideMenuElementSelectedCallback(this, &CustomControlsView::slideMenuElementSelectedHandler),
    threeWayButtonPressedCallback(this, &CustomControlsView::threeWayButtonPressedHandler),
    currentAnimationState(NO_ANIMATION),
    tickCounter(0)
{
    menuBackground.setBitmap(Bitmap(BITMAP_CONTROLS_MENU_BACKGROUND_ID));
    menuBackground.setXY(0, 0);
    add(menuBackground);

    background.setBitmap(Bitmap(BITMAP_CONTROLS_BACKGROUND_ID));
    background.setXY(menuBackground.getWidth(), 0);
    add(background);

    menuUp.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_ARROW_UP_ID), Bitmap(BITMAP_CONTROL_MENU_ARROW_UP_PRESSED_ID));
    menuUp.setXY(menuBackground.getX(), menuBackground.getY());
    menuUp.setAction(onButtonPressed);
    add(menuUp);

    menuDown.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_ARROW_DOWN_ID), Bitmap(BITMAP_CONTROL_MENU_ARROW_DOWN_PRESSED_ID));
    menuDown.setXY(menuUp.getX(), menuBackground.getRect().bottom() - menuDown.getHeight());
    menuDown.setAction(onButtonPressed);
    add(menuDown);

    int numberOfElements = (Utils::reducedPerformance()) ? 3 : 4;

    slideMenu.setPosition(menuBackground.getX() + ((menuBackground.getWidth() - Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_LARGE_ID).getWidth()) / 2) - 7, menuUp.getRect().bottom(), 60, menuDown.getY() - menuUp.getRect().bottom());
    slideMenu.setup(numberOfElements, 0, Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_SMALL_ID), Bitmap(BITMAP_CONTROL_MENU_ICON_DATEPICKER_LARGE_ID));
    slideMenu.setBitmapsForElement(0, BITMAP_CONTROL_MENU_ICON_GAUGE_SMALL_ID, BITMAP_CONTROL_MENU_ICON_GAUGE_LARGE_ID);
    slideMenu.setBitmapsForElement(1, BITMAP_CONTROL_MENU_ICON_THREE_WAY_SMALL_ID, BITMAP_CONTROL_MENU_ICON_THREE_WAY_LARGE_ID);
    slideMenu.setBitmapsForElement(2, BITMAP_CONTROL_MENU_ICON_PERCENTAGE_SMALL_ID, BITMAP_CONTROL_MENU_ICON_PERCENTAGE_LARGE_ID);

    if (!Utils::reducedPerformance())
    {
        slideMenu.setBitmapsForElement(3, BITMAP_CONTROL_MENU_ICON_DATEPICKER_SMALL_ID, BITMAP_CONTROL_MENU_ICON_DATEPICKER_LARGE_ID);
    }

    slideMenu.setAnimationDuration(8);
    slideMenu.setElementSelectedCallback(slideMenuElementSelectedCallback);
    add(slideMenu);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_BUTTON_ID), Bitmap(BITMAP_CONTROL_MENU_BUTTON_PRESSED_ID));
    gotoMenuButton.setXY(HAL::DISPLAY_WIDTH - gotoMenuButton.getWidth() - 3, 2);
    add(gotoMenuButton);

    threeWayProgressBar.setXY(background.getX() + (background.getWidth() - threeWayProgressBar.getWidth()) / 2, background.getY() + (background.getHeight() - threeWayProgressBar.getHeight()) / 2);
    threeWayProgressBar.setButtonClickedCallback(threeWayButtonPressedCallback);
    threeWayProgressBar.setVisible(false);
    add(threeWayProgressBar);

    percentageBar.setXY(background.getX() + (background.getWidth() - percentageBar.getWidth()) / 2, background.getY() + (background.getHeight() - percentageBar.getHeight()) / 2);
    percentageBar.setBarAngle(-40);
    percentageBar.setVisible(false);
    add(percentageBar);

    datePicker.setXY(background.getX() + (background.getWidth() - datePicker.getWidth()) / 2, background.getY() + (background.getHeight() - datePicker.getHeight()) / 2);
    datePicker.setVisible(false);
    add(datePicker);

    gauge.setBitmaps(Bitmap(BITMAP_GAUGE_BACKGROUND_ID), Bitmap(BITMAP_GAUGE_NEEDLE_PIN_ID));
    gauge.setXY(background.getX() + (background.getWidth() - gauge.getWidth()) / 2, background.getY() + (background.getHeight() - gauge.getHeight()) / 2);
    gauge.setLimits(0, 60, 240, 480);
    gauge.setValue(40);
    gauge.setAnimationDuration(20);
    gauge.setVisible(false);
    add(gauge);

    statusMessage.setPosition(background.getX() + 6, 20, 100, 48);
    statusMessage.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    statusMessage.setVisible(false);
    add(statusMessage);

    mcuLoadTxt.setXY(HAL::DISPLAY_WIDTH - 106, 258 - mcuLoadTxt.getTypedText().getFont()->getFontHeight());
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0x17, 0x3C, 0x51));
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right(), mcuLoadTxt.getY(), 34, 14);
    mcuLoadValueTxt.setColor(mcuLoadTxt.getColor());
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        barAnimationState[i] = NO_ANIMATION;
    }

    slideMenuElementSelectedHandler(slideMenu);
}

CustomControlsView::~CustomControlsView()
{
}

void CustomControlsView::setupScreen()
{
}

void CustomControlsView::tearDownScreen()
{
}

void CustomControlsView::handleTickEvent()
{
    tickCounter++;

    if (getAnimationState() == ANIMATION_THREE_WAY_RUNNING)
    {
        for (int i = 0; i < NUMBER_OF_BARS; i++)
        {
            if (animationCounter < animationDuration[i])
            {
                int percentage = animationEquation[i](animationCounter, 0, 103, animationDuration[i]);

                threeWayProgressBar.setBarPercentage(i, percentage);
            }
            else
            {
                barAnimationState[i] = NO_ANIMATION;
            }

            if (getAnimationState() != ANIMATION_THREE_WAY_RUNNING)
            {
                stopThreeWayProgressBarAnimation();
            }
        }
        animationCounter++;
    }
    else if (getAnimationState() == ANIMATION_GAUGE)
    {
        if (tickCounter % 60 == 0)
        {
            if (gauge.getValue() > 50)
            {
                gauge.setEasingEquation(EasingEquations::backEaseOut);
                gauge.setAnimationDuration(20);
                gauge.setValue(0);
            }
            else
            {
                int value = gauge.getValue();
                int newValue = value + Utils::randomNumberBetween(-6, 18);
                if (newValue < value)
                {
                    gauge.setEasingEquation(EasingEquations::backEaseOut);
                    gauge.setAnimationDuration((newValue - value) * 2);
                }
                else
                {
                    gauge.setEasingEquation(EasingEquations::quadEaseOut);
                    gauge.setAnimationDuration(newValue - value);
                }
                gauge.setValue(newValue);
            }
        }

    }
}

void CustomControlsView::startThreeWayProgressBarAnimation()
{
    animationCounter = 0;

    for (int i = 0; i < NUMBER_OF_BARS; i++)
    {
        animationEquation[i] = getRandomEasingEquation();
        animationDuration[i] = Utils::randomNumberBetween(100, 200);
        barAnimationState[i] = ANIMATION_THREE_WAY_RUNNING;
    }
    currentAnimationState = ANIMATION_THREE_WAY_RUNNING;

    statusMessage.setVisible(false);
    statusMessage.invalidate();
}

CustomControlsView::AnimationState CustomControlsView::getAnimationState()
{
    if (currentAnimationState == ANIMATION_THREE_WAY_RUNNING)
    {
        for (int i = 0; i < NUMBER_OF_BARS; i++)
        {
            if (barAnimationState[i] == ANIMATION_THREE_WAY_RUNNING)
            {
                return ANIMATION_THREE_WAY_RUNNING;
            }
        }
        return NO_ANIMATION;
    }

    return currentAnimationState;
}

touchgfx::EasingEquation CustomControlsView::getRandomEasingEquation()
{
    switch (Utils::randomNumberBetween(0, 6))
    {
    case 0:
        return EasingEquations::sineEaseIn;
    case 1:
        return EasingEquations::linearEaseInOut;
    case 2:
        return EasingEquations::quadEaseIn;
    case 3:
        return EasingEquations::cubicEaseOut;
    case 4:
        return EasingEquations::cubicEaseInOut;
    case 5:
        return EasingEquations::linearEaseNone;
    default:
        return EasingEquations::sineEaseIn;
    }
}

void CustomControlsView::threeWayButtonPressedHandler(const ThreeWayProgressBar& button)
{
    threeWayProgressBar.setActive(true);
    startThreeWayProgressBarAnimation();
}

void CustomControlsView::stopThreeWayProgressBarAnimation()
{
    threeWayProgressBar.setActive(false);
    statusMessage.setVisible(true);
    statusMessage.setAlpha(255);
    statusMessage.setTypedText(TypedText(T_THREE_WAY_PROGRESS_BAR_STATUS));
    statusMessage.setFadeAnimationDelay(80);
    statusMessage.startFadeAnimation(0, 40, EasingEquations::cubicEaseIn);
    statusMessage.invalidate();

    currentAnimationState = NO_ANIMATION;
}

void CustomControlsView::buttonPressedHandler(const AbstractButton& button)
{
    threeWayProgressBar.stopAnimation();

    if (&button == &menuUp)
    {
        currentAnimationState = NO_ANIMATION;
        slideMenu.animateDown();
    }
    else if (&button == &menuDown)
    {
        currentAnimationState = NO_ANIMATION;
        slideMenu.animateUp();
    }
}

void CustomControlsView::slideMenuElementSelectedHandler(const VerticalSlideMenu& menu)
{
    int selectedElement = slideMenu.getSelectedElementIndex();

    // Setup menu up/down button. First set default then handle special cases
    menuUp.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_ARROW_UP_ID), Bitmap(BITMAP_CONTROL_MENU_ARROW_UP_PRESSED_ID));
    menuDown.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_ARROW_DOWN_ID), Bitmap(BITMAP_CONTROL_MENU_ARROW_DOWN_PRESSED_ID));
    menuUp.setTouchable(true);
    menuDown.setTouchable(true);

    if (selectedElement == 0)
    {
        menuUp.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_ARROW_UP_INACTIVE_ID), Bitmap(BITMAP_CONTROL_MENU_ARROW_UP_INACTIVE_ID));
        menuUp.setTouchable(false);
    }
    else if (selectedElement == slideMenu.getSize() - 1)
    {
        menuDown.setBitmaps(Bitmap(BITMAP_CONTROL_MENU_ARROW_DOWN_INACTIVE_ID), Bitmap(BITMAP_CONTROL_MENU_ARROW_DOWN_INACTIVE_ID));
        menuDown.setTouchable(false);
    }
    // Expand the active area of the buttons to make them easier to activate
    menuUp.setWidth(menuUp.getWidth() + 30);
    menuDown.setWidth(menuDown.getWidth() + 30);
    menuUp.invalidate();
    menuDown.invalidate();

    // Set the chosen control visible
    gauge.setVisible(selectedElement == 0);
    threeWayProgressBar.setVisible(selectedElement == 1);
    percentageBar.setVisible(selectedElement == 2);
    datePicker.setVisible(selectedElement == 3);
    statusMessage.setVisible(false);

    datePicker.invalidate();
    gauge.invalidate();
    threeWayProgressBar.invalidate();
    percentageBar.invalidate();
    statusMessage.invalidate();

    currentAnimationState = NO_ANIMATION;

    if (gauge.isVisible())
    {
        currentAnimationState = ANIMATION_GAUGE;
    }
    else if (threeWayProgressBar.isVisible())
    {
        threeWayProgressBar.reset();
    }
    else if (datePicker.isVisible())
    {
        datePicker.reset();

        // Set start values before animation
        datePicker.setday(14, 0, EasingEquations::backEaseInOut);
        datePicker.setMonth(6, 0, EasingEquations::backEaseInOut);
        datePicker.setYear(8, 0, EasingEquations::backEaseInOut);

        // Start animation to a specific date
        datePicker.setday(23, 40, EasingEquations::backEaseInOut);
        datePicker.setMonth(1, 42, EasingEquations::backEaseInOut);
        datePicker.setYear(3, 38, EasingEquations::backEaseInOut);
    }
}

void CustomControlsView::screenSaverMinorTick()
{
    if (slideMenu.getSelectedElementIndex() == 0)
    {
        buttonPressedHandler(menuDown);
    }
    else if (slideMenu.getSelectedElementIndex() == slideMenu.getSize() - 1)
    {
        buttonPressedHandler(menuUp);
    }
    else
    {
        if (Utils::randomNumberBetween(0, 100) < 50)
        {
            buttonPressedHandler(menuUp);
        }
        else
        {
            buttonPressedHandler(menuDown);
        }
    }
}
