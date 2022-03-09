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
  


#include <gui/live_data_display_screen/LiveDataDisplayPresenter.hpp>
#include <gui/live_data_display_screen/LiveDataDisplayView.hpp>
#include <gui/common/FrontendApplication.hpp>

LiveDataDisplayPresenter::LiveDataDisplayPresenter(LiveDataDisplayView& v) :
    view(v)
{
}

LiveDataDisplayPresenter::~LiveDataDisplayPresenter()
{
}

void LiveDataDisplayPresenter::activate()
{

}

void LiveDataDisplayPresenter::deactivate()
{

}
