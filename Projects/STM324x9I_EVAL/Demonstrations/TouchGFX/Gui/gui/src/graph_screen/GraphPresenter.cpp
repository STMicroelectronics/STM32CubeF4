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
  


#include <gui/graph_screen/GraphPresenter.hpp>
#include <gui/graph_screen/GraphView.hpp>
#include <gui/common/FrontendApplication.hpp>

GraphPresenter::GraphPresenter(GraphView& v) :
    DemoPresenter(v),
    view(v)
{
}

GraphPresenter::~GraphPresenter()
{
}

void GraphPresenter::activate()
{
}

void GraphPresenter::deactivate()
{

}

void GraphPresenter::screenSaverMajorTick()
{
    view.screenSaverMajorTick();
}
