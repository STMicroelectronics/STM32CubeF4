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
  


#ifndef LIVE_DATA_DISPLAY_PRESENTER_HPP
#define LIVE_DATA_DISPLAY_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class LiveDataDisplayView;

class LiveDataDisplayPresenter : public Presenter, public ModelListener
{
public:
    LiveDataDisplayPresenter(LiveDataDisplayView& v);
    virtual ~LiveDataDisplayPresenter();

    virtual void activate();
    virtual void deactivate();
private:
    LiveDataDisplayPresenter();

    LiveDataDisplayView& view;
};

#endif // LIVE_DATA_DISPLAY_PRESENTER_HPP
