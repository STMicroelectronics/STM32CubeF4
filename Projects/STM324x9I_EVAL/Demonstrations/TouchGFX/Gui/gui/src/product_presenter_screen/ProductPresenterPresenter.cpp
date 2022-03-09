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
  


#include <gui/product_presenter_screen/ProductPresenterPresenter.hpp>
#include <gui/product_presenter_screen//ProductPresenterView.hpp>
#include <gui/common/FrontendApplication.hpp>

ProductPresenterPresenter::ProductPresenterPresenter(ProductPresenterView& v) :
    DemoPresenter(v),
    view(v)
{
}

ProductPresenterPresenter::~ProductPresenterPresenter()
{
}

void ProductPresenterPresenter::activate()
{
}

void ProductPresenterPresenter::deactivate()
{

}

void ProductPresenterPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
