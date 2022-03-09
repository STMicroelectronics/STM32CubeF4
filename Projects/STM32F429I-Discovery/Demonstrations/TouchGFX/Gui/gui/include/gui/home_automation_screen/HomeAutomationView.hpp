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
  


#ifndef HOME_AUTOMATION_VIEW_HPP
#define HOME_AUTOMATION_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <gui/home_automation_screen/JogWheel.hpp>
#include <gui/home_automation_screen/TemperatureMenuItem.hpp>
#include <gui/home_automation_screen/TemperatureScheduleMenuItem.hpp>
#include <gui/home_automation_screen/TemperatureSlider.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>

class HomeAutomationView : public View<HomeAutomationPresenter>
{
public:

    HomeAutomationView();

    virtual ~HomeAutomationView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    // New Info from the presenter
    void insertRoom(RoomTemperatureInfo& room);
    void updateRoom(RoomTemperatureInfo& room);

    void setSelectedMenuItem(RoomTemperatureInfo& room);
    void saveScheduleInfo(RoomTemperatureInfo& room);

private:
    enum States
    {
        ANIMATE_JOG_WHEEL_IN,
        ANIMATE_JOG_WHEEL_OUT,
        ANIMATE_SCHEDULE_IN,
        ANIMATE_SCHEDULE_OUT,
        JOG_WHEEL_STATE,
        SCHEDULE_STATE,
        ROOM_STATE
    } currentState;

    uint8_t animationCounter;

    static const int16_t INITIAL_TEMPERATURE = 6;
    static const int16_t JOGWHEEL_DELTA = 6;

    Image menuTopBar;
    TextArea menuTopBarHeadline;
    Button scheduleButton;
    Button adjustTempButton;
    Button exitButton;

    // Scroll menu in jog wheel mode
    ListLayout menu;
    ScrollableContainer scrollableMenu;
    static const uint8_t NUMBER_OF_ROOMS = 8;
    TemperatureMenuItem menuItems[NUMBER_OF_ROOMS];
    uint8_t currentNumberOfRooms;
    TEXTS selectedRoomName;

    // Jog wheel widgets
    Container jogWheelContainer;
    JogWheel jogWheel;
    TextAreaWithOneWildcard temperatureText;
    Unicode::UnicodeChar temperatureTextBuffer[5];
    Button okButton;
    Button backButton;

    // Scrollmenu in schedule mode
    TextArea scheduleMenuTopBarHeadline;
    Image scheduleDayNightImage;
    ListLayout scheduleMenu;
    ScrollableContainer scrollableScheduleMenu;
    TemperatureScheduleMenuItem scheduleMenuItems[RoomTemperatureInfo::NUMBER_OF_DAYS];

    TemperatureSlider slider;
    Image sliderBackground;

    Callback<HomeAutomationView, const AbstractButton&> onButtonPressed;
    Callback<HomeAutomationView, int16_t> onJogWheelValueChanged;
    Callback<HomeAutomationView, int16_t> onJogWheelEndDragEvent;
    Callback<HomeAutomationView, const TemperatureMenuItem&> onMenuItemSelected;
    Callback<HomeAutomationView, const TemperatureScheduleMenuItem&> onScheduleMenuItemSelected;
    Callback<HomeAutomationView, const TemperatureScheduleMenuItem&> onScheduleMenuItemValueUpdated;

    void buttonPressedhandler(const AbstractButton& button);
    void jogWheelValueChanged(int16_t value);
    void jogWheelEndDragEvent(int16_t value);
    void menuItemSelected(const TemperatureMenuItem&);
    void scheduleMenuItemSelected(const TemperatureScheduleMenuItem&);
    void scheduleMenuItemValueUpdated(const TemperatureScheduleMenuItem&);

    void setCurrentState(States newState);
    void animateJogWheelIn();
    void animateJogWheelOut();
    void animateScheduleIn();
    void animateScheduleOut();

    void showRoomElements(uint8_t startAlpha);
    void showScheduleElements(uint8_t startAlpha);
    void setActiveStateForRoomElements(bool active);
    void setActiveStateForScheduleElements(bool active);
    void setAlphaForRoomElements(uint8_t alpha);
    void setAlphaForScheduleElements(uint8_t alpha);
    void hideAllElements();

    void setOkButtonState(bool valuesUpdated);

};

#endif // HOME_AUTOMATION_VIEW_HPP
