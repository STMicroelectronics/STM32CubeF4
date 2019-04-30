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
  


#ifndef MAIN_MENU_PRESENTER_HPP
#define MAIN_MENU_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class MainMenuView;

class MainMenuPresenter : public DemoPresenter
{
public:
    enum DemoID
    {
        CUSTOM_CONTROLS = 0,
        GRAPH_SCREEN,
        EASING_EQUATION,
        PRODUCT_PRESENTER,
        NO_DEMO_SCREEN
    };

    MainMenuPresenter(MainMenuView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MainMenuPresenter() {};

    void demoSelected(DemoID id, uint8_t menuIndex);
private:
    MainMenuPresenter();

    MainMenuView& view;
};

#endif // MAIN_MENU_PRESENTER_HPP
