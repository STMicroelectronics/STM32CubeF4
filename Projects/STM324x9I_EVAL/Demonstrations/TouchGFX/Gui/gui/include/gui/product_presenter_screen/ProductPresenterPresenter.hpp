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
  


#ifndef PRODUCT_PRESENTER_PRESENTER_HPP
#define PRODUCT_PRESENTER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class ProductPresenterView;

class ProductPresenterPresenter : public DemoPresenter
{
public:
    ProductPresenterPresenter(ProductPresenterView& v);
    virtual ~ProductPresenterPresenter();

    virtual void activate();
    virtual void deactivate();

    virtual void screenSaverMinorTick();

private:
    ProductPresenterPresenter();

    ProductPresenterView& view;
};

#endif // PRODUCT_PRESENTER_PRESENTER_HPP
