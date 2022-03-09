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
