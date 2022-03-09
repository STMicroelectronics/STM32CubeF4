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
