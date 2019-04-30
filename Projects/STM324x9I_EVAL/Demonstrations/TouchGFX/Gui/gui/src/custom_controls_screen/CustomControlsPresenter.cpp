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
