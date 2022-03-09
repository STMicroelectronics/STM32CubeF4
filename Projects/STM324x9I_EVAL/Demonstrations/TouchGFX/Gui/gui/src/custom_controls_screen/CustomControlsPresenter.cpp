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
  


#include <gui/custom_controls_screen/CustomControlsPresenter.hpp>
#include <gui/custom_controls_screen/CustomControlsView.hpp>
#include <gui/common/FrontendApplication.hpp>

CustomControlsPresenter::CustomControlsPresenter(CustomControlsView& v) :
    DemoPresenter(v),
    view(v)
{
}

CustomControlsPresenter::~CustomControlsPresenter()
{
}

void CustomControlsPresenter::activate()
{
}

void CustomControlsPresenter::deactivate()
{

}

void CustomControlsPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
