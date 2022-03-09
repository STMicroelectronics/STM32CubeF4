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
  


#ifndef LIVE_DATA_DISPLAY_VIEW_HPP
#define LIVE_DATA_DISPLAY_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/live_data_display_screen/LiveDataDisplayPresenter.hpp>
#include <gui/live_data_display_screen/CityInfo.hpp>
#include <gui/live_data_display_screen/WeekInfo.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <gui/common/DotIndicator.hpp>

/* The LiveDataDisplay screen implements a weather forecast.
 * The LiveDataDisplayView class handles the visible part of the weather screen.
 * In this demo only static data are used. This data is placed in the
 * view related classes. In a real world example this will be placed in
 * the model and changed dynamically.
 */
class LiveDataDisplayView : public View<LiveDataDisplayPresenter>
{
public:
    LiveDataDisplayView();
    virtual ~LiveDataDisplayView();
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);

private:
    enum States
    {
        ANIMATE_SWIPE_CANCELLED_LEFT,
        ANIMATE_SWIPE_CANCELLED_RIGHT,
        ANIMATE_LEFT,
        ANIMATE_RIGHT,
        NO_ANIMATION
    } currentState;

    static const uint8_t NUMBER_OF_SCREENS = 4;

    uint8_t animationCounter;
    int32_t tickCounter;

    int16_t dragX;
    int16_t animateDistance;
    int16_t startX;
    uint8_t currentScreen;

    Button exitButton;

    // Background that becomes visible when trying to swipe
    // beyond the number of screens.
    Image backgroundSwipeAreaLeft;
    Image backgroundSwipeAreaRight;

    DotIndicator dotIndicator;

    // Info screens show the specific data for a city.
    // The viewport represents the visible part of the
    // swipe area
    Container infoScreensViewPort;
    ListLayout infoScreens;

    CityInfo infoScreen[4];

    // Week specific weather data are displayed by the
    // WeekInfo class.
    WeekInfo weekInfoBar;

    Callback<LiveDataDisplayView, const AbstractButton&> onButtonPressed;

    void buttonPressedhandler(const AbstractButton& button);
    void adjustInfoScreens();

    void animateSwipeCancelledLeft();
    void animateSwipeCancelledRight();
    void animateLeft();
    void animateRight();
};

#endif // LIVE_DATA_DISPLAY_VIEW_HPP
