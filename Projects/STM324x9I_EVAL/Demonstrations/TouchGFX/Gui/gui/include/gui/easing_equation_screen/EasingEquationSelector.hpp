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
  


#ifndef EASINGEQUATIONSELECTOR_HPP
#define EASINGEQUATIONSELECTOR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/widgets/RadioButtonGroup.hpp>
#include <gui/easing_equation_screen/EasingEquationButton.hpp>

using namespace touchgfx;

class EasingEquationSelector : public Container
{
public:
    EasingEquationSelector();
    void setEasingEquationSelectedCallback(GenericCallback<const EasingEquationSelection&>& callback);
    void select(int index);

private:
    static const int NUMBER_OF_EQUATION_TYPES = 11;

    ListLayout easings;
    ScrollableContainer scrollable;
    RadioButtonGroup<NUMBER_OF_EQUATION_TYPES> equationsGroup;
    EasingEquationButton buttons[NUMBER_OF_EQUATION_TYPES];

    Callback<EasingEquationSelector, const AbstractButton&> onSelected;
    void selected(const AbstractButton& btn);

    GenericCallback<const EasingEquationSelection&>* easingEquationSelected;
};

#endif /* EASINGEQUATIONSELECTOR_HPP */
