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
  

#ifndef GAME_2D_PRESENTER_HPP
#define GAME_2D_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class Game2DView;

class Game2DPresenter : public DemoPresenter
{
public:
    Game2DPresenter(Game2DView& v);
    virtual ~Game2DPresenter();

    virtual void activate();
    virtual void deactivate();

private:
    Game2DPresenter();

    Game2DView& view;
};

#endif // GAME_2D_PRESENTER_HPP
