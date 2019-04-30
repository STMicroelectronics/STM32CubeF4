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
