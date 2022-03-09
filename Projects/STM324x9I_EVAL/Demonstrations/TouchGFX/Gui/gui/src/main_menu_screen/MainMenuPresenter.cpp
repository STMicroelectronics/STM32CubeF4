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
