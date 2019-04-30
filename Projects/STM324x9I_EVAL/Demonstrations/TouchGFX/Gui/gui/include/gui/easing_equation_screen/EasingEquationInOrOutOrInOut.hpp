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
