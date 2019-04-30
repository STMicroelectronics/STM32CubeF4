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
