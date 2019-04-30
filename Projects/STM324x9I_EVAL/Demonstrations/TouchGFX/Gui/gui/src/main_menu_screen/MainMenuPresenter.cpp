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
  


#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/main_menu_screen/MainMenuView.hpp>

MainMenuPresenter::MainMenuPresenter(MainMenuView& v) :
    DemoPresenter(v),
    view(v)
{
}

void MainMenuPresenter::activate()
{
    view.setSelectedMenuIndex(model->getSelectedMenuIndex());
}

void MainMenuPresenter::deactivate()
{

}

void MainMenuPresenter::demoSelected(DemoID id, uint8_t menuIndex)
{
    model->setSelectedMenuIndex(menuIndex);
    switch (id)
    {
    case CUSTOM_CONTROLS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoCustomControlsScreen();
        break;
    case GRAPH_SCREEN:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGraphScreen();
        break;
    case EASING_EQUATION:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoEasingEquationScreen();
        break;
    case PRODUCT_PRESENTER:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoProductPresenterScreen();
        break;
    case NO_DEMO_SCREEN:
        break;
    default:
        break;
    }
}
