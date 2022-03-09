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
  

#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/game2D_screen/Game2DView.hpp>
#include <gui/common/FrontendApplication.hpp>

Game2DPresenter::Game2DPresenter(Game2DView& v) :
    DemoPresenter(v),
    view(v)
{
}

Game2DPresenter::~Game2DPresenter()
{
}

void Game2DPresenter::activate()
{
}

void Game2DPresenter::deactivate()
{

}
