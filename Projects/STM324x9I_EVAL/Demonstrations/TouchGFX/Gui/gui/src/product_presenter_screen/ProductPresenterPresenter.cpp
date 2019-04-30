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
