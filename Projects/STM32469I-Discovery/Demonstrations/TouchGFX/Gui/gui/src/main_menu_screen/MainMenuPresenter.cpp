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

}

void MainMenuPresenter::deactivate()
{

}

void MainMenuPresenter::setSelectedDemoScreenIndex(int elementIndex)
{
    model->setSelectedDemoScreenIndex(elementIndex);
}

void MainMenuPresenter::setPreviousSelectedMenuType(MainMenuType menuType)
{
    model->setPreviousSelectedMainMenuType((Model::MainMenuType) menuType);
}

int MainMenuPresenter::getSelectedDemoScreenIndex()
{
    return model->getSelectedDemoScreenIndex();
}

MainMenuPresenter::MainMenuType MainMenuPresenter::getPreviousSelectedMenuType()
{
    return (MainMenuPresenter::MainMenuType) (model->getPreviousSelectedMainMenuType());
}

void MainMenuPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
