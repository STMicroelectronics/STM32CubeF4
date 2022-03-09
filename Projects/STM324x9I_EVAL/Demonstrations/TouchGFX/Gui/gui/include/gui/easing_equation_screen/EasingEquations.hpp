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
  


#ifndef EASINGEQUATIONSENUM_HPP
#define EASINGEQUATIONSENUM_HPP

#include <touchgfx/EasingEquations.hpp>

using namespace touchgfx;

struct EasingEquationSelection
{
    EasingEquation in;
    EasingEquation out;
    EasingEquation inOut;

    TypedTextId EasingEquationName;
    TypedTextId EasingEquationShortName;
};

typedef enum
{
    EASING_EQUATION_IN,
    EASING_EQUATION_OUT,
    EASING_EQUATION_IN_OUT
} EasingEquationsInOrOut;

#endif /* EASINGEQUATIONSENUM_HPP */
