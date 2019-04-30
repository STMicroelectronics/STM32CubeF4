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
  


#ifndef DEMO_PRESENTER_HPP
#define DEMO_PRESENTER_HPP

#include <mvp/Presenter.hpp>
#include <gui/common/DemoViewInterface.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <gui/model/ModelListener.hpp>

using namespace touchgfx;

class DemoPresenter : public Presenter, public ModelListener
{
public:
    DemoPresenter(DemoViewInterface& view);

    void backOptionSelected();
    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    void viewStartupDone();
private:

    DemoViewInterface& viewInterface;

};

#endif
