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
