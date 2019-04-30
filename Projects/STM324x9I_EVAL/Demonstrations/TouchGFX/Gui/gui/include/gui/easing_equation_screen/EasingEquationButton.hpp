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
  


#ifndef EASING_EQUATION_BUTTON
#define EASING_EQUATION_BUTTON

#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/widgets/RadioButton.hpp>
#include <gui/easing_equation_screen/EasingEquations.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextArea.hpp>

using namespace touchgfx;

class EasingEquationButton : public Container
{
public:
    EasingEquationButton();

    void setBitmaps();
    void setTextColor(colortype textColor, colortype shadowColor);
    void setTextShadowVisible(bool textShadowVisible);

    void setup(EasingEquationSelection newEasingEquation, const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed);
    const EasingEquationSelection& getEasingEquation() const;

    RadioButton& getRadioButton();

private:
    RadioButton radioButton;

    EasingEquationSelection easingEquation;

    TextArea buttonText;
    TextArea buttonTextShadow;
};

#endif /* EASING_EQUATION_BUTTON */
