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
  


#include <gui/easing_equation_screen/EasingEquationInOrOutOrInOut.hpp>
#include <gui/easing_equation_screen/EasingEquationSelector.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

EasingEquationInOrOutOrInOut::EasingEquationInOrOutOrInOut() :
    onSelected(this, &EasingEquationInOrOutOrInOut::selected),
    inOrOutSelectedCallback(0)
{

    // Only the text id is used in this group.
    EasingEquationSelection selections[] =
    {
        { EasingEquations::linearEaseNone,        EasingEquations::linearEaseNone,        EasingEquations::linearEaseNone,        T_EASING_EQUATION_EASE_IN,        T_EASING_EQUATION_EASE_IN         },
        { EasingEquations::linearEaseNone,        EasingEquations::linearEaseNone,        EasingEquations::linearEaseNone,        T_EASING_EQUATION_EASE_OUT,       T_EASING_EQUATION_EASE_OUT       },
        { EasingEquations::linearEaseNone,        EasingEquations::linearEaseNone,        EasingEquations::linearEaseNone,        T_EASING_EQUATION_EASE_INOUT,     T_EASING_EQUATION_EASE_INOUT   }
    };


    in.setup(selections[0], Bitmap(BITMAP_EASING_BOTTOM_MENU_ID), Bitmap(BITMAP_EASING_BOTTOM_MENU_ACTIVE_ID),
             Bitmap(BITMAP_EASING_BOTTOM_MENU_SELECTED_ID), Bitmap(BITMAP_EASING_BOTTOM_MENU_ACTIVE_ID));
    in.setXY(0, 0);
    in.setTextShadowVisible(false);

    out.setup(selections[1], Bitmap(BITMAP_EASING_BOTTOM_MENU_ID), Bitmap(BITMAP_EASING_BOTTOM_MENU_ACTIVE_ID),
              Bitmap(BITMAP_EASING_BOTTOM_MENU_SELECTED_ID), Bitmap(BITMAP_EASING_BOTTOM_MENU_ACTIVE_ID));
    out.setXY(in.getRect().right(), 0);
    out.setTextShadowVisible(false);

    inOut.setup(selections[2], Bitmap(BITMAP_EASING_BOTTOM_MENU_ID), Bitmap(BITMAP_EASING_BOTTOM_MENU_ACTIVE_ID),
                Bitmap(BITMAP_EASING_BOTTOM_MENU_SELECTED_ID), Bitmap(BITMAP_EASING_BOTTOM_MENU_ACTIVE_ID));
    inOut.setXY(out.getRect().right(), 0);
    inOut.setTextShadowVisible(false);

    buttons.setRadioButtonSelectedHandler(onSelected);
    buttons.add(in.getRadioButton());
    buttons.add(out.getRadioButton());
    buttons.add(inOut.getRadioButton());

    add(in);
    add(out);
    add(inOut);

    setWidth(inOut.getRect().right());
    setHeight(in.getHeight());

    setButtonColors();
}

void EasingEquationInOrOutOrInOut::setInOrOutSelectedCallback(GenericCallback<EasingEquationsInOrOut>& callback)
{
    inOrOutSelectedCallback = &callback;
}

void EasingEquationInOrOutOrInOut::selected(const AbstractButton& btn)
{
    EasingEquationsInOrOut inOrOut = (&btn == &(in.getRadioButton())) ? EASING_EQUATION_IN : (&btn == &(out.getRadioButton())) ? EASING_EQUATION_OUT : EASING_EQUATION_IN_OUT;
    setButtonColors();
    if (inOrOutSelectedCallback && inOrOutSelectedCallback->isValid())
    {
        inOrOutSelectedCallback->execute(inOrOut);
    }
}

void EasingEquationInOrOutOrInOut::selectFirst()
{
    buttons.setSelected(in.getRadioButton());
}

void EasingEquationInOrOutOrInOut::setButtonColors()
{
    int selected = buttons.getSelectedRadioButtonIndex();

    in.setTextColor(Color::getColorFrom24BitRGB(0x9D, 0xC9, 0xD1), 0);
    out.setTextColor(Color::getColorFrom24BitRGB(0x9D, 0xC9, 0xD1), 0);
    inOut.setTextColor(Color::getColorFrom24BitRGB(0x9D, 0xC9, 0xD1), 0);

    switch (selected)
    {
    case 0:
        in.setTextColor(Color::getColorFrom24BitRGB(0xFF, 0x61, 0x14), 0);
        break;
    case 1:
        out.setTextColor(Color::getColorFrom24BitRGB(0xFF, 0x61, 0x14), 0);
        break;
    case 2:
        inOut.setTextColor(Color::getColorFrom24BitRGB(0xFF, 0x61, 0x14), 0);
        break;
    default:
        break;
    }
}
