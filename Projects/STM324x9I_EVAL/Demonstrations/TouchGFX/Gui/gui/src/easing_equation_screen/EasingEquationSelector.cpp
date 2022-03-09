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
  


#include <gui/easing_equation_screen/EasingEquationSelector.hpp>
#include <touchgfx/Bitmap.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/TypedText.hpp>

EasingEquationSelector::EasingEquationSelector() :
    onSelected(this, &EasingEquationSelector::selected),
    easingEquationSelected(0)
{
    EasingEquationSelection selections[] =
    {
        { EasingEquations::backEaseIn,   EasingEquations::backEaseOut,    EasingEquations::backEaseInOut,    T_EASING_EQUATION_BACK,        T_EASING_EQUATION_BACK_SHORT        },
        { EasingEquations::bounceEaseIn, EasingEquations::bounceEaseOut,  EasingEquations::bounceEaseInOut,  T_EASING_EQUATION_BOUNCE,      T_EASING_EQUATION_BOUNCE_SHORT      },
        { EasingEquations::circEaseIn,   EasingEquations::circEaseOut,    EasingEquations::circEaseInOut,    T_EASING_EQUATION_CIRCULAR,    T_EASING_EQUATION_CIRCULAR_SHORT    },
        { EasingEquations::cubicEaseIn,  EasingEquations::cubicEaseOut,   EasingEquations::cubicEaseInOut,   T_EASING_EQUATION_CUBIC,       T_EASING_EQUATION_CUBIC_SHORT       },
        { EasingEquations::elasticEaseIn, EasingEquations::elasticEaseOut, EasingEquations::elasticEaseInOut, T_EASING_EQUATION_ELASTIC,     T_EASING_EQUATION_ELASTIC_SHORT     },
        { EasingEquations::expoEaseIn,   EasingEquations::expoEaseOut,    EasingEquations::expoEaseInOut,    T_EASING_EQUATION_EXPONENTIAL, T_EASING_EQUATION_EXPONENTIAL_SHORT },
        { EasingEquations::linearEaseIn, EasingEquations::linearEaseOut,  EasingEquations::linearEaseInOut,  T_EASING_EQUATION_LINEAR,      T_EASING_EQUATION_LINEAR_SHORT      },
        { EasingEquations::quadEaseIn,   EasingEquations::quadEaseOut,    EasingEquations::quadEaseInOut,    T_EASING_EQUATION_QUADRATIC,   T_EASING_EQUATION_QUADRATIC_SHORT   },
        { EasingEquations::quartEaseIn,  EasingEquations::quartEaseOut,   EasingEquations::quartEaseInOut,   T_EASING_EQUATION_QUARTIC,     T_EASING_EQUATION_QUARTIC_SHORT     },
        { EasingEquations::quintEaseIn,  EasingEquations::quintEaseOut,   EasingEquations::quintEaseInOut,   T_EASING_EQUATION_QUINTIC,     T_EASING_EQUATION_QUINTIC_SHORT     },
        { EasingEquations::sineEaseIn,   EasingEquations::sineEaseOut,    EasingEquations::sineEaseInOut,    T_EASING_EQUATION_SINUSOIDAL,  T_EASING_EQUATION_SINUSOIDAL_SHORT  }
    };

    for (int i = 0; i < NUMBER_OF_EQUATION_TYPES; i++)
    {
        buttons[i].setup(selections[i], Bitmap(BITMAP_EASING_SIDE_MENU_BUTTON_ID), Bitmap(BITMAP_EASING_SIDE_MENU_BUTTON_ACTIVE_ID),
                         Bitmap(BITMAP_EASING_SIDE_MENU_BUTTON_SELECTED_ID), Bitmap(BITMAP_EASING_SIDE_MENU_BUTTON_ACTIVE_ID));
        equationsGroup.add(buttons[i].getRadioButton());
        easings.add(buttons[i]);
    }

    // Put some extra height in the list so that you can slide the list up and see the TouchGFX logo
    easings.setHeight(easings.getHeight() + 40);

    scrollable.setPosition(0, 0, easings.getWidth(), HAL::DISPLAY_HEIGHT);
    scrollable.setScrollbarsVisible(false);
    scrollable.add(easings);
    add(scrollable);

    setWidth(scrollable.getWidth());
    setHeight(scrollable.getHeight());
}

void EasingEquationSelector::setEasingEquationSelectedCallback(GenericCallback<const EasingEquationSelection&>& callback)
{
    this->easingEquationSelected = &callback;
    equationsGroup.setRadioButtonSelectedHandler(onSelected);
}

void EasingEquationSelector::selected(const AbstractButton& btn)
{
    for (int i = 0; i < NUMBER_OF_EQUATION_TYPES; i++)
    {
        if (&btn == &buttons[i].getRadioButton())
        {
            if (easingEquationSelected && easingEquationSelected->isValid())
            {
                easingEquationSelected->execute(buttons[i].getEasingEquation());
            }
        }
    }
}

void EasingEquationSelector::select(int index)
{
    equationsGroup.setSelected(buttons[index].getRadioButton());
}
