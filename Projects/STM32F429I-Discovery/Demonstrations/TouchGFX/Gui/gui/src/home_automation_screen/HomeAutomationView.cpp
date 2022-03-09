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
  


#include <gui/home_automation_screen/HomeAutomationView.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

HomeAutomationView::HomeAutomationView() :
    currentState(ROOM_STATE),
    animationCounter(0),
    jogWheel(INITIAL_TEMPERATURE, JOGWHEEL_DELTA),
    onButtonPressed(this, &HomeAutomationView::buttonPressedhandler),
    onJogWheelValueChanged(this, &HomeAutomationView::jogWheelValueChanged),
    onJogWheelEndDragEvent(this, &HomeAutomationView::jogWheelEndDragEvent),
    onMenuItemSelected(this, &HomeAutomationView::menuItemSelected),
    onScheduleMenuItemSelected(this, &HomeAutomationView::scheduleMenuItemSelected),
    onScheduleMenuItemValueUpdated(this, &HomeAutomationView::scheduleMenuItemValueUpdated)
{
}

void HomeAutomationView::setupScreen()
{
    menuTopBar.setBitmap(Bitmap(BITMAP_TEMPERATURE_MENU_TOP_EXTENDED_ID));
    menuTopBar.setXY(0, 0);

    menuTopBarHeadline.setPosition(0, 6, HAL::DISPLAY_WIDTH, 36);
    menuTopBarHeadline.setTypedText(TypedText(T_TEMPERATURE_MENU_HEADLINE));
    menuTopBarHeadline.setColor(Color::getColorFrom24BitRGB(0x35, 0xBC, 0xFF));

    scheduleButton.setBitmaps(Bitmap(BITMAP_SCHEDULE_BUTTON_ID), Bitmap(BITMAP_SCHEDULE_BUTTON_PRESSED_ID));
    scheduleButton.setXY(0, HAL::DISPLAY_HEIGHT - scheduleButton.getHeight());
    scheduleButton.setAction(onButtonPressed);

    adjustTempButton.setBitmaps(Bitmap(BITMAP_TEMPERATURE_ADJUST_BUTTON_ID), Bitmap(BITMAP_TEMPERATURE_ADJUST_BUTTON_PRESSED_ID));
    adjustTempButton.setXY(scheduleButton.getWidth(), HAL::DISPLAY_HEIGHT - adjustTempButton.getHeight());
    adjustTempButton.setAction(onButtonPressed);

    exitButton.setBitmaps(Bitmap(BITMAP_EXIT_BUTTON_ID), Bitmap(BITMAP_EXIT_BUTTON_PRESSED_ID));
    exitButton.setXY(2, 0);
    exitButton.setAction(onButtonPressed);

    jogWheel.setBitmaps(Bitmap(BITMAP_TEMPERATURE_JOGWHEEL_ID), Bitmap(BITMAP_TEMPERATURE_JOGWHEEL_ID));
    jogWheelContainer.setPosition(HAL::DISPLAY_WIDTH, menuTopBar.getHeight(), jogWheel.getWidth(), jogWheel.getHeight());
    jogWheel.setXY(0, 0);
    jogWheel.setValueUpdatedCallback(onJogWheelValueChanged);
    jogWheel.setEndDragEventCallback(onJogWheelEndDragEvent);

    backButton.setBitmaps(Bitmap(BITMAP_TEMPERATURE_CANCEL_BUTTON_ID), Bitmap(BITMAP_TEMPERATURE_CANCEL_BUTTON_PRESSED_ID));
    backButton.setXY(0, HAL::DISPLAY_HEIGHT - backButton.getHeight());
    backButton.setAction(onButtonPressed);
    backButton.setVisible(false);

    okButton.setBitmaps(Bitmap(BITMAP_TEMPERATURE_OK_BUTTON_ID), Bitmap(BITMAP_TEMPERATURE_OK_BUTTON_PRESSED_ID));
    okButton.setXY(backButton.getWidth(), HAL::DISPLAY_HEIGHT - okButton.getHeight());
    okButton.setAction(onButtonPressed);
    okButton.setVisible(false);

    temperatureText.setPosition(74, 92, 100, 40);
    temperatureText.setTypedText(TypedText(T_TEMPERATUREINDICATOR));
    temperatureText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    temperatureText.setWildcard(temperatureTextBuffer);
    jogWheelValueChanged(INITIAL_TEMPERATURE);

    jogWheelContainer.add(jogWheel);
    jogWheelContainer.add(temperatureText);

    scrollableMenu.setXY(0, menuTopBar.getHeight());
    scrollableMenu.setWidth(HAL::DISPLAY_WIDTH);
    scrollableMenu.setHeight(HAL::DISPLAY_HEIGHT - menuTopBar.getHeight() - scheduleButton.getHeight());
    scrollableMenu.add(menu);
    scrollableMenu.setScrollbarsVisible(false);
    scrollableMenu.enableHorizontalScroll(false);
    scrollableMenu.setScrollThreshold(8);
    menu.setXY(0, 0);
    currentNumberOfRooms = 0;


    scheduleMenuTopBarHeadline.setPosition(6, 6, 230, 36);
    scheduleMenuTopBarHeadline.setTypedText(TypedText(T_TEMPERATURE_MENU_HEADLINE));
    scheduleMenuTopBarHeadline.setColor(Color::getColorFrom24BitRGB(0x35, 0xBC, 0xFF));
    scheduleMenuTopBarHeadline.setAlpha(0);

    scheduleMenu.setXY(0, 0);
    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setWeekday((RoomTemperatureInfo::weekday)(RoomTemperatureInfo::MONDAY + i));
        scheduleMenuItems[i].setMenuItemSelectedCallback(onScheduleMenuItemSelected);
        scheduleMenuItems[i].setMenuItemValueUpdatedCallback(onScheduleMenuItemValueUpdated);
        scheduleMenuItems[i].setSlider(&slider);
        scheduleMenu.add(scheduleMenuItems[i]);
    }

    scheduleDayNightImage.setBitmap(Bitmap(BITMAP_TEMPERATURE_MENU_TOP_DAY_NIGHT_ID));
    scheduleDayNightImage.setXY(0, menuTopBar.getHeight());

    sliderBackground.setBitmap(Bitmap(BITMAP_TEMPERATURE_MENU_ITEM_SELECTED_ID));
    sliderBackground.setXY(0, scheduleButton.getY() - sliderBackground.getHeight());
    sliderBackground.setVisible(false);

    slider.setXY(HAL::DISPLAY_WIDTH / 2 - slider.getWidth() / 2, sliderBackground.getY() + 3);
    slider.setVisible(false);
    slider.setLimits(10, 30);
    slider.setTouchable(false);

    scrollableScheduleMenu.setXY(0, scheduleDayNightImage.getY() + scheduleDayNightImage.getHeight());
    scrollableScheduleMenu.setWidth(HAL::DISPLAY_WIDTH);
    scrollableScheduleMenu.setHeight(sliderBackground.getY() - (scheduleDayNightImage.getY() + scheduleDayNightImage.getHeight()));
    scrollableScheduleMenu.add(scheduleMenu);
    scrollableScheduleMenu.setScrollbarsVisible(false);
    scrollableScheduleMenu.enableHorizontalScroll(false);
    scrollableScheduleMenu.setVisible(false);

    showRoomElements(255);

    add(scrollableMenu);
    add(scrollableScheduleMenu);
    add(scheduleButton);
    add(adjustTempButton);
    add(jogWheelContainer);
    add(menuTopBar);
    add(exitButton);
    add(menuTopBarHeadline);
    add(scheduleMenuTopBarHeadline);
    add(scheduleDayNightImage);
    add(okButton);
    add(backButton);
    add(sliderBackground);
    add(slider);
}

void HomeAutomationView::tearDownScreen()
{

}

void HomeAutomationView::handleTickEvent()
{
    if (currentState == ANIMATE_JOG_WHEEL_IN)
    {
        animateJogWheelIn();
    }
    else if (currentState == ANIMATE_JOG_WHEEL_OUT)
    {
        animateJogWheelOut();
    }
    else if (currentState == ANIMATE_SCHEDULE_IN)
    {
        animateScheduleIn();
    }
    else if (currentState == ANIMATE_SCHEDULE_OUT)
    {
        animateScheduleOut();
    }
}

void HomeAutomationView::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &okButton)
    {
        if (currentState == JOG_WHEEL_STATE)
        {
            presenter->newRoomTemperature(jogWheel.getValue());
            setCurrentState(ANIMATE_JOG_WHEEL_OUT);
        }
        else if (currentState == SCHEDULE_STATE)
        {
            presenter->newScheduleAccepted();
            setCurrentState(ANIMATE_SCHEDULE_OUT);
        }
    }
    else if (&button == &backButton)
    {
        if (currentState == JOG_WHEEL_STATE)
        {
            setCurrentState(ANIMATE_JOG_WHEEL_OUT);
        }
        else if (currentState == SCHEDULE_STATE)
        {
            setCurrentState(ANIMATE_SCHEDULE_OUT);
        }
    }
    else if (&button == &scheduleButton)
    {
        setCurrentState(ANIMATE_SCHEDULE_IN);
    }
    else if (&button == &adjustTempButton)
    {
        jogWheel.setValue(presenter->getSelectedRoom().getCurrentTemperature());
        setCurrentState(ANIMATE_JOG_WHEEL_IN);
    }
    else if (&button == &exitButton)
    {
        presenter->exitScreen();
    }
}

void HomeAutomationView::jogWheelValueChanged(int16_t value)
{
    Unicode::snprintf(temperatureTextBuffer, 5, "%d", value);
    temperatureText.invalidate();

    setOkButtonState(true);
}

void HomeAutomationView::jogWheelEndDragEvent(int16_t value)
{
    //presenter->newRoomTemperature(value);
}

void HomeAutomationView::setSelectedMenuItem(RoomTemperatureInfo& room)
{
    for (uint8_t i = 0; i < currentNumberOfRooms; i++)
    {
        menuItems[i].setSelected(menuItems[i].getRoomId() == room.getRoomId());
    }
    menu.invalidate();

    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setRoomTemperatureInfo(room);
    }
    scheduleMenu.invalidate();

    scheduleMenuTopBarHeadline.setTypedText(TypedText(room.getRoomScheduleHeadline()));
    scheduleMenuTopBarHeadline.invalidate();

    selectedRoomName = room.getRoomNameHeadline();
    jogWheel.setValue(room.getCurrentTemperature());
}

void HomeAutomationView::insertRoom(RoomTemperatureInfo& room)
{
    menuItems[currentNumberOfRooms].setRoomTemperatureInfo(room);
    menuItems[currentNumberOfRooms].setMenuItemSelectedCallback(onMenuItemSelected);

    // Pair the model object with the graphical representation
    menuItems[currentNumberOfRooms].setRoomId(currentNumberOfRooms);
    room.setRoomId(currentNumberOfRooms);

    menu.add(menuItems[currentNumberOfRooms]);

    currentNumberOfRooms++;

    menu.invalidate();
}

void HomeAutomationView::updateRoom(RoomTemperatureInfo& room)
{
    menuItems[room.getRoomId()].setRoomTemperatureInfo(room);
}

void HomeAutomationView::menuItemSelected(const TemperatureMenuItem& menuItem)
{
    presenter->roomSelected(menuItem.getRoomId());
}

void HomeAutomationView::scheduleMenuItemSelected(const TemperatureScheduleMenuItem& menuItem)
{
    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setSelected(&scheduleMenuItems[i] == &menuItem);
    }
}

void HomeAutomationView::saveScheduleInfo(RoomTemperatureInfo& room)
{
    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        room.setScheduleDayTemp((RoomTemperatureInfo::weekday) i, scheduleMenuItems[i].getDayTemperature());
        room.setScheduleNightTemp((RoomTemperatureInfo::weekday) i, scheduleMenuItems[i].getNightTemperature());
    }
}

void HomeAutomationView::scheduleMenuItemValueUpdated(const TemperatureScheduleMenuItem&)
{
    setOkButtonState(true);
}

void HomeAutomationView::setCurrentState(States newState)
{
    animationCounter = 0;
    currentState = newState;
}

void HomeAutomationView::animateJogWheelIn()
{
    uint8_t duration = 14;

    if (animationCounter == 0)
    {
        setActiveStateForRoomElements(false);
        setOkButtonState(false);
    }

    if (animationCounter <= 6)
    {
        uint8_t alpha = 255 - (uint8_t) EasingEquations::linearEaseNone(animationCounter, 0, 255, 6);
        menuTopBarHeadline.setAlpha(alpha);
        scheduleButton.setAlpha(alpha);
        adjustTempButton.setAlpha(alpha);
        exitButton.setAlpha(alpha);

        menuTopBarHeadline.invalidate();
        scheduleButton.invalidate();
        adjustTempButton.invalidate();
        exitButton.invalidate();
    }
    else if (animationCounter == 7)
    {
        menuTopBarHeadline.setTypedText(TypedText(selectedRoomName));
        okButton.setVisible(true);
        okButton.setAlpha(0);
        okButton.invalidate();
        backButton.setVisible(true);
        backButton.setAlpha(0);
        backButton.invalidate();
    }
    else
    {
        uint8_t alpha = (uint8_t) EasingEquations::linearEaseNone(animationCounter - 7, 0, 255, duration - 6);

        menuTopBarHeadline.setAlpha((uint8_t) alpha);
        menuTopBarHeadline.invalidate();
        okButton.setAlpha(alpha);
        okButton.invalidate();
        backButton.setAlpha(alpha);
        backButton.invalidate();
    }

    if (animationCounter <= duration)
    {
        // Move jogWheel
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, jogWheelContainer.getWidth(), duration);
        jogWheelContainer.moveTo(HAL::DISPLAY_WIDTH - delta, jogWheelContainer.getY());
    }
    else
    {
        // Final step: stop the animation
        backButton.setTouchable(true);

        setCurrentState(JOG_WHEEL_STATE);
    }
    animationCounter++;
}

void HomeAutomationView::animateJogWheelOut()
{
    uint8_t duration = 14;

    if (animationCounter == 0)
    {
        okButton.setTouchable(false);
        backButton.setTouchable(false);
    }

    if (animationCounter <= 6)
    {
        uint8_t alpha = 255 - (uint8_t) EasingEquations::linearEaseNone(animationCounter, 0, 255, 6);

        menuTopBarHeadline.setAlpha(alpha);
        okButton.setAlpha(alpha);
        backButton.setAlpha(alpha);

        menuTopBarHeadline.invalidate();
        okButton.invalidate();
        backButton.invalidate();
    }
    else if (animationCounter == 7)
    {
        menuTopBarHeadline.setTypedText(TypedText(T_TEMPERATURE_MENU_HEADLINE));
        okButton.setVisible(false);
        backButton.setVisible(false);

        okButton.invalidate();
        backButton.invalidate();
    }
    else
    {
        uint8_t alpha = (uint8_t) EasingEquations::linearEaseNone(animationCounter - 7, 0, 255, duration - 6);

        menuTopBarHeadline.setAlpha(alpha);
        scheduleButton.setAlpha(alpha);
        adjustTempButton.setAlpha(alpha);
        exitButton.setAlpha(alpha);

        menuTopBarHeadline.invalidate();
        scheduleButton.invalidate();
        adjustTempButton.invalidate();
        exitButton.invalidate();
    }


    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::cubicEaseOut(animationCounter, 0, jogWheelContainer.getWidth(), duration);
        jogWheelContainer.moveTo(HAL::DISPLAY_WIDTH - jogWheelContainer.getWidth() + delta , jogWheelContainer.getY());
    }
    else
    {
        // Final step: stop the animation
        setActiveStateForRoomElements(true);

        menuTopBarHeadline.setTypedText(TypedText(T_TEMPERATURE_MENU_HEADLINE));

        setCurrentState(ROOM_STATE);
    }
    animationCounter++;
}

void HomeAutomationView::animateScheduleIn()
{
    uint8_t duration = 12;

    if (animationCounter == 0)
    {
        setActiveStateForRoomElements(false);

        for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
        {
            // Preselect row 1
            scheduleMenuItems[i].setSelected(i == 1);
        }

        setOkButtonState(false);
    }

    if (animationCounter <= 5)
    {
        uint8_t alpha = 255 - (uint8_t) EasingEquations::linearEaseNone(animationCounter, 0, 255, 5);
        setAlphaForRoomElements(alpha);
    }
    else if (animationCounter == 6)
    {
        showScheduleElements(0);
    }
    else if (animationCounter <= duration)
    {
        uint8_t alpha = (uint8_t) EasingEquations::linearEaseNone(animationCounter - 6, 0, 255, duration - 6);
        setAlphaForScheduleElements(alpha);
    }
    else
    {
        // Final step: stop the animation
        setActiveStateForScheduleElements(true);
        setOkButtonState(false);

        setCurrentState(SCHEDULE_STATE);
    }
    animationCounter++;
}

void HomeAutomationView::animateScheduleOut()
{
    uint8_t duration = 12;

    if (animationCounter == 0)
    {
        setActiveStateForScheduleElements(false);
    }

    if (animationCounter <= 5)
    {
        uint8_t alpha = 255 - (uint8_t) EasingEquations::linearEaseNone(animationCounter, 0, 255, 5);
        setAlphaForScheduleElements(alpha);
    }
    else if (animationCounter == 6)
    {
        showRoomElements(0);
    }
    else if (animationCounter <= duration)
    {
        uint8_t alpha = (uint8_t) EasingEquations::linearEaseNone(animationCounter - 6, 0, 255, duration - 6);
        setAlphaForRoomElements(alpha);
    }
    else
    {
        // Final step: stop the animation
        setActiveStateForRoomElements(true);

        setCurrentState(ROOM_STATE);
    }
    animationCounter++;
}

void HomeAutomationView::showRoomElements(uint8_t startAlpha)
{
    hideAllElements();

    menuTopBarHeadline.setTypedText(TypedText(T_TEMPERATURE_MENU_HEADLINE));

    menuTopBarHeadline.setVisible(true);
    scheduleButton.setVisible(true);
    adjustTempButton.setVisible(true);
    exitButton.setVisible(true);

    menuTopBarHeadline.setAlpha(startAlpha);
    scheduleButton.setAlpha(startAlpha);
    adjustTempButton.setAlpha(startAlpha);
    exitButton.setAlpha(startAlpha);

    for (uint8_t i = 0; i < currentNumberOfRooms; i++)
    {
        menuItems[i].setVisible(true);
        menuItems[i].setTextsAlpha(startAlpha);
        menuItems[i].invalidate();
    }

    menuTopBarHeadline.invalidate();
    scheduleButton.invalidate();
    adjustTempButton.invalidate();
    exitButton.invalidate();
}

void HomeAutomationView::showScheduleElements(uint8_t startAlpha)
{
    hideAllElements();

    scheduleMenuTopBarHeadline.setVisible(true);
    slider.setVisible(true);
    sliderBackground.setVisible(true);
    scrollableScheduleMenu.setVisible(true);
    backButton.setVisible(true);
    okButton.setVisible(true);
    scheduleDayNightImage.setVisible(true);

    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setVisible(true);
        scheduleMenuItems[i].invalidate();
    }

    setAlphaForScheduleElements(startAlpha);

    scheduleMenuTopBarHeadline.invalidate();
    scrollableScheduleMenu.invalidate();
    slider.invalidate();
    sliderBackground.invalidate();
    backButton.invalidate();
    okButton.invalidate();
    scheduleDayNightImage.invalidate();
}

void HomeAutomationView::setActiveStateForRoomElements(bool active)
{
    for (uint8_t i = 0; i < currentNumberOfRooms; i++)
    {
        menuItems[i].setTouchable(active);
    }
    scheduleButton.setTouchable(active);
    adjustTempButton.setTouchable(active);
    exitButton.setTouchable(active);
}

void HomeAutomationView::setActiveStateForScheduleElements(bool active)
{
    slider.setTouchable(active);
    backButton.setTouchable(active);
    // okButton is not handled here, should be controlled by setOkButtonState

    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setTouchable(active);
    }
}

void HomeAutomationView::setAlphaForRoomElements(uint8_t alpha)
{
    menuTopBarHeadline.setAlpha(alpha);
    scheduleButton.setAlpha(alpha);
    adjustTempButton.setAlpha(alpha);
    exitButton.setAlpha(alpha);

    for (uint8_t i = 0; i < currentNumberOfRooms; i++)
    {
        menuItems[i].setTextsAlpha(alpha);
        menuItems[i].invalidate();
    }

    menuTopBarHeadline.invalidate();
    scheduleButton.invalidate();
    adjustTempButton.invalidate();
    exitButton.invalidate();
}

void HomeAutomationView::setAlphaForScheduleElements(uint8_t alpha)
{
    scheduleMenuTopBarHeadline.setAlpha(alpha);
    slider.setAlpha(alpha);
    sliderBackground.setAlpha(alpha);
    backButton.setAlpha(alpha);
    okButton.setAlpha(alpha);
    scheduleDayNightImage.setAlpha(alpha);

    scheduleMenuTopBarHeadline.invalidate();
    slider.invalidate();
    sliderBackground.invalidate();
    backButton.invalidate();
    okButton.invalidate();
    scheduleDayNightImage.invalidate();

    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setAlpha(alpha);
        scheduleMenuItems[i].invalidate();
    }
}

void HomeAutomationView::hideAllElements()
{
    // Room Screen
    for (uint8_t i = 0; i < currentNumberOfRooms; i++)
    {
        menuItems[i].setVisible(false);
    }

    menuTopBarHeadline.setVisible(false);
    scheduleButton.setVisible(false);
    adjustTempButton.setVisible(false);
    exitButton.setVisible(false);

    // Schedule screen
    scheduleMenuTopBarHeadline.setVisible(false);
    slider.setVisible(false);
    sliderBackground.setVisible(false);
    scrollableScheduleMenu.setVisible(false);
    backButton.setVisible(false);
    okButton.setVisible(false);
    scheduleDayNightImage.setVisible(false);

    for (uint8_t i = 0; i < RoomTemperatureInfo::NUMBER_OF_DAYS; i++)
    {
        scheduleMenuItems[i].setVisible(false);
    }

}

void HomeAutomationView::setOkButtonState(bool valuesUpdated)
{
    if (valuesUpdated)
    {
        okButton.setBitmaps(Bitmap(BITMAP_TEMPERATURE_OK_BUTTON_ID), Bitmap(BITMAP_TEMPERATURE_OK_BUTTON_PRESSED_ID));
    }
    else
    {
        okButton.setBitmaps(Bitmap(BITMAP_TEMPERATURE_OK_BUTTON_INACTIVE_ID), Bitmap(BITMAP_TEMPERATURE_OK_BUTTON_INACTIVE_ID));
    }
    okButton.setTouchable(valuesUpdated);

    okButton.invalidate();
}
