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
  

#include <gui/common/DemoPresenter.hpp>

DemoPresenter::DemoPresenter(DemoViewInterface& view) :
    viewInterface(view)
{
}

void DemoPresenter::backOptionSelected()
{
    if (model->getPreviousSelectedMainMenuType() == Model::CAROUSEL_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuCarouselScreen();
    }
    else if (model->getPreviousSelectedMainMenuType() == Model::ANIMATING_BUTTONS_MENU)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoMainMenuAnimatingButtonsScreen();
    }
}

void DemoPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    viewInterface.updateProcessorLoad(mcuLoad);
}

