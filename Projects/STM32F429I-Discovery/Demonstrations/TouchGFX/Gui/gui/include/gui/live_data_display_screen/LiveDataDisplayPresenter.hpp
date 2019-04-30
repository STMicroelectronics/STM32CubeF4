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
