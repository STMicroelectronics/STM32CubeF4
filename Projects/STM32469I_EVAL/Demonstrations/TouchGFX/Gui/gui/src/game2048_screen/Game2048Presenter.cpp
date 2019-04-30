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
  

#include <gui/game2048_screen/Game2048Presenter.hpp>
#include <gui/game2048_screen/Game2048View.hpp>
#include <gui/common/FrontendApplication.hpp>

Game2048Presenter::Game2048Presenter(Game2048View& v) :
    DemoPresenter(v),
    view(v)
{
}

Game2048Presenter::~Game2048Presenter()
{
}

void Game2048Presenter::activate()
{
}

void Game2048Presenter::deactivate()
{

}

void Game2048Presenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
