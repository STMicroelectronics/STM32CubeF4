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
  


#include <gui/easing_equation_screen/EasingEquationPresenter.hpp>
#include <gui/easing_equation_screen/EasingEquationView.hpp>
#include <gui/common/FrontendApplication.hpp>

EasingEquationPresenter::EasingEquationPresenter(EasingEquationView& v) :
    DemoPresenter(v),
    view(v)
{
}

EasingEquationPresenter::~EasingEquationPresenter()
{
}

void EasingEquationPresenter::activate()
{
}

void EasingEquationPresenter::deactivate()
{

}
