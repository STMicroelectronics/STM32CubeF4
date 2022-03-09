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
  


#ifndef CUSTOM_CONTROLS_PRESENTER_HPP
#define CUSTOM_CONTROLS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class CustomControlsView;

class CustomControlsPresenter : public DemoPresenter
{
public:
    CustomControlsPresenter(CustomControlsView& v);
    virtual ~CustomControlsPresenter();

    virtual void activate();
    virtual void deactivate();

    virtual void screenSaverMinorTick();

private:
    CustomControlsPresenter();

    CustomControlsView& view;
};

#endif // CUSTOM_CONTROLS_PRESENTER_HPP
