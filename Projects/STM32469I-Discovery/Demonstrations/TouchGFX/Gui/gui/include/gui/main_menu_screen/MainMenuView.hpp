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


class MainMenuView : public DemoView<MainMenuPresenter>
{
public:
    enum DemoID
    {
        PRODUCT_PRESENTER = 0,
        GAME2D,
        CUSTOM_CONTROLS,
        GRAPH,
        NUMBER_OF_DEMO_SCREENS,
        NO_DEMO_SCREEN
    };

    MainMenuView();
    virtual ~MainMenuView();

    virtual void setupScreen() = 0;
    virtual void tearDownScreen() = 0;

    virtual void screenSaverMinorTick() = 0;
protected:

    DemoID demoIDs[NUMBER_OF_DEMO_SCREENS];
    int selectedDemoScreenIndex;

    Button stMenuButton;

    Callback<MainMenuView, const AbstractButton&> onStMenuButtonPressed;
    void stMenuButtonPressedHandler(const AbstractButton& button);

    void setSelectedDemoScreen(int demoIndex);
    void gotoSelectedDemoScreen();
};

#endif // MAIN_MENU_VIEW_HPP
