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
