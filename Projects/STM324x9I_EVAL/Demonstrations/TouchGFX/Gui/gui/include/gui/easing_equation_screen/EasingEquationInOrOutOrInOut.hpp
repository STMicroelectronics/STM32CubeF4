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
  


#ifndef EASINGEQUATIONINOROUTORINOUT_HPP
#define EASINGEQUATIONINOROUTORINOUT_HPP

#include <touchgfx/widgets/RadioButtonGroup.hpp>
#include <gui/easing_equation_screen/EasingEquationButton.hpp>
#include <touchgfx/containers/Container.hpp>
#include <gui/easing_equation_screen/EasingEquations.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>

using namespace touchgfx;

class EasingEquationInOrOutOrInOut : public Container
{
public:
    EasingEquationInOrOutOrInOut();
    void setInOrOutSelectedCallback(GenericCallback<EasingEquationsInOrOut>& callback);
    void selectFirst();
private:
    EasingEquationButton in;
    EasingEquationButton out;
    EasingEquationButton inOut;
    RadioButtonGroup<3> buttons;

    Callback<EasingEquationInOrOutOrInOut, const AbstractButton&> onSelected;
    void selected(const AbstractButton& btn);
    void setButtonColors();
    touchgfx::GenericCallback<EasingEquationsInOrOut>* inOrOutSelectedCallback;
};

#endif /* EASINGEQUATIONINOROUTORINOUT_HPP */
