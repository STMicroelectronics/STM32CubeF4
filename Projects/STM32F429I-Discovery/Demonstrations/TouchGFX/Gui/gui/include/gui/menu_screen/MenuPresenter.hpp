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
  


#ifndef MENU_PRESENTER_HPP
#define MENU_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/model/Model.hpp>

using namespace touchgfx;

class MenuView;

class MenuPresenter : public Presenter, public ModelListener
{
public:
    MenuPresenter(MenuView& v);
    virtual ~MenuPresenter();

    virtual void activate();
    virtual void deactivate();

private:
    MenuPresenter();

    MenuView& view;
};

#endif // MENU_PRESENTER_HPP
