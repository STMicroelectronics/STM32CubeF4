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
  


#ifndef EASINGEQUATIONPRESENTER_HPP
#define EASINGEQUATIONPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class EasingEquationView;

class EasingEquationPresenter : public DemoPresenter
{
public:
    EasingEquationPresenter(EasingEquationView& v);
    virtual ~EasingEquationPresenter();

    virtual void activate();
    virtual void deactivate();

private:
    EasingEquationPresenter();

    EasingEquationView& view;
};

#endif /* EASINGEQUATIONPRESENTER_HPP */
