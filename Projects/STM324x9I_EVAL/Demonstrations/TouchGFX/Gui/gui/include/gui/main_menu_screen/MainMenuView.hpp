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
