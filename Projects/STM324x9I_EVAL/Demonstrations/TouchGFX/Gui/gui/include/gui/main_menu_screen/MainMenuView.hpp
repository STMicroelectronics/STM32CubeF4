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
  


#ifndef MAIN_MENU_VIEW_HPP
#define MAIN_MENU_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/common/SwipeContainer.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

class MainMenuView : public DemoView<MainMenuPresenter>
{
public:
    MainMenuView();
    virtual ~MainMenuView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    void setSelectedMenuIndex(uint8_t demoIndex);

    virtual void afterTransition();
protected:
    static const int NUMBER_OF_DEMO_SCREENS = 4;

    MainMenuPresenter::DemoID demoScreenIndex[NUMBER_OF_DEMO_SCREENS];

    Image backgroundLeft;
    Image backgroundRight;

    Container demoScreensContainer[NUMBER_OF_DEMO_SCREENS];
    Image demoScreens[NUMBER_OF_DEMO_SCREENS];
    Button demoScreensButton[NUMBER_OF_DEMO_SCREENS];

    SwipeContainer menuContainer;

    Callback<MainMenuView, const AbstractButton& > buttonPressedCallback;

    Callback<MainMenuView, const SwipeContainer&> menuStateChangeInitiatedCallback;
    Callback<MainMenuView, const SwipeContainer&> menuStateChangeEndedCallback;

    void menuStateChangeInitiatedHandler(const SwipeContainer& sc);
    void menuStateChangeEndedHandler(const SwipeContainer& sc);

    void buttonPressedHandler(const AbstractButton& button);

    void gotoSelectedDemoScreen(uint8_t demoIndex);
};

#endif // MAIN_MENU_VIEW_HPP
