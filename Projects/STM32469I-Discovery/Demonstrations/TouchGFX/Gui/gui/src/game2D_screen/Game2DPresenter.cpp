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
