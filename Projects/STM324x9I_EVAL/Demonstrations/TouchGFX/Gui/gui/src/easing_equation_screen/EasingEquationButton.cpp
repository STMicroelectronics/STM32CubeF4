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
#include <touchgfx/Color.hpp>

EasingEquationButton::EasingEquationButton()
{
    buttonText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    buttonTextShadow.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));

    add(radioButton);
    add(buttonTextShadow);
    add(buttonText);
}

void EasingEquationButton::setup(EasingEquationSelection newEasingEquation, const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed)
{
    radioButton.setBitmaps(bmpUnselected, bmpUnselectedPressed, bmpSelected, bmpSelectedPressed);
    setWidth(radioButton.getWidth());
    setHeight(radioButton.getHeight());

    easingEquation = newEasingEquation;
    buttonText.setTypedText(TypedText(newEasingEquation.EasingEquationShortName));
    buttonText.resizeToCurrentText();
    buttonTextShadow.setTypedText(buttonText.getTypedText());
    buttonTextShadow.resizeToCurrentText();
    buttonText.setXY((getWidth() - buttonText.getWidth()) / 2, (getHeight() - buttonText.getHeight()) / 2);
    buttonTextShadow.setXY(buttonText.getX() + 1, buttonText.getY() + 1);

    invalidate();
}

RadioButton& EasingEquationButton::getRadioButton()
{
    return radioButton;
}

const EasingEquationSelection& EasingEquationButton::getEasingEquation() const
{
    return easingEquation;
}

void EasingEquationButton::setTextColor(colortype textColor, colortype shadowColor)
{
    buttonText.setColor(textColor);
    buttonTextShadow.setColor(shadowColor);

    buttonText.invalidate();
    buttonTextShadow.invalidate();
}

void EasingEquationButton::setTextShadowVisible(bool textShadowVisible)
{
    buttonTextShadow.setVisible(textShadowVisible);
    buttonTextShadow.invalidate();
}
