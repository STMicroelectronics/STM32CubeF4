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
