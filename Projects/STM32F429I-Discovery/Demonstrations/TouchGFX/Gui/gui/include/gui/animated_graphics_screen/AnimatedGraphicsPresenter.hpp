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
  


#ifndef ANIMATED_GRAPHICS_PRESENTER_HPP
#define ANIMATED_GRAPHICS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class AnimatedGraphicsView;

class AnimatedGraphicsPresenter : public Presenter, public ModelListener
{
public:
    AnimatedGraphicsPresenter(AnimatedGraphicsView& v);
    virtual ~AnimatedGraphicsPresenter();

    virtual void activate();
    virtual void deactivate();
private:
    AnimatedGraphicsPresenter();

    AnimatedGraphicsView& view;
};

#endif // ANIMATED_GRAPHICS_PRESENTER_HPP
