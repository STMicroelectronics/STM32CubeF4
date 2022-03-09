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
  


#include <gui/custom_controls_screen/WheelSelectorExtra.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>

WheelSelectorExtra::WheelSelectorExtra() :
    WheelSelector()
{
}

WheelSelectorExtra::~WheelSelectorExtra()
{
}

void WheelSelectorExtra::setupDatePickerWheelDay(int width, int height, int textXPosition, int textYPositionOfIndex0Selected, TEXTS wheelText, TEXTS wheelTextExtra0, TEXTS wheelTextExtra1, TEXTS wheelTextExtra2)
{
    WheelSelector::setup(width, height, textXPosition, textYPositionOfIndex0Selected, wheelText);

    textExtra[0].setTypedText(TypedText(wheelTextExtra0));
    textExtra[1].setTypedText(TypedText(wheelTextExtra1));
    textExtra[2].setTypedText(TypedText(wheelTextExtra2));

    textExtra[0].setXY(text.getX(), text.getRect().bottom());
    textExtra[1].setXY(text.getX(), textExtra[0].getRect().bottom());
    textExtra[2].setXY(text.getX(), textExtra[1].getRect().bottom());

    for (int i = 0; i < NUMBER_OF_EXTRA_TEXTS; i++)
    {
        textExtraIsVisible[i] = true;
        textExtra[i].setMoveAnimationEndedAction(textMoveEndedCallback);
        textContainer.add(textExtra[i]);

        textExtraSelected[i].setTypedText(textExtra[i].getTypedText());
        textExtraSelected[i].setX(textExtra[i].getX());
        textExtraSelected[i].setMoveAnimationEndedAction(textMoveEndedCallback);
        textSelectedContainer.add(textExtraSelected[i]);
    }

    adjustBottomCutOff();
    reset();
}

void WheelSelectorExtra::setTextColor(colortype normalColor, colortype selectedColor, colortype selectedBackgroundColor, int selectedYOffset /*= 0*/, int selectedHeight /*= 0*/)
{
    WheelSelector::setTextColor(normalColor, selectedColor, selectedBackgroundColor, selectedYOffset, selectedHeight);
    for (int i = 0; i < NUMBER_OF_EXTRA_TEXTS; i++)
    {
        textExtra[i].setColor(text.getColor());
        textExtraSelected[i].setY(textExtra[i].getY() - selectedYOffset);
        textExtraSelected[i].setColor(textSelected.getColor());
    }
}

void WheelSelectorExtra::moveText(int newYPosition)
{
    adjustForBoundries(newYPosition);

    text.moveTo(text.getX(), newYPosition);
    textExtra[0].moveTo(text.getX(), newYPosition + text.getHeight());
    textExtra[1].moveTo(text.getX(), newYPosition + text.getHeight() + textExtra[0].getHeight());
    textExtra[2].moveTo(text.getX(), newYPosition + text.getHeight() + textExtra[0].getHeight() + textExtra[1].getHeight());

    textSelected.moveTo(text.getX(), newYPosition - selectedAreaYOffset);
    textExtraSelected[0].moveTo(text.getX(), textExtra[0].getY() - selectedAreaYOffset);
    textExtraSelected[1].moveTo(text.getX(), textExtra[1].getY() - selectedAreaYOffset);
    textExtraSelected[2].moveTo(text.getX(), textExtra[2].getY() - selectedAreaYOffset);
}

void WheelSelectorExtra::animateMoveText(int newYPosition, int duration, EasingEquation equation)
{
    text.startMoveAnimation(text.getX(), newYPosition, duration, EasingEquations::linearEaseNone, equation);
    textExtra[0].startMoveAnimation(text.getX(), newYPosition + text.getHeight(), duration, EasingEquations::linearEaseNone, equation);
    textExtra[1].startMoveAnimation(text.getX(), newYPosition + text.getHeight() + textExtra[0].getHeight(), duration, EasingEquations::linearEaseNone, equation);
    textExtra[2].startMoveAnimation(text.getX(), newYPosition + text.getHeight() + textExtra[0].getHeight() + textExtra[1].getHeight(), duration, EasingEquations::linearEaseNone, equation);

    textSelected.startMoveAnimation(text.getX(), newYPosition - selectedAreaYOffset, duration, EasingEquations::linearEaseNone, equation);
    textExtraSelected[0].startMoveAnimation(text.getX(), newYPosition + text.getHeight() - selectedAreaYOffset, duration, EasingEquations::linearEaseNone, equation);
    textExtraSelected[1].startMoveAnimation(text.getX(), newYPosition + text.getHeight() + textExtra[0].getHeight() - selectedAreaYOffset, duration, EasingEquations::linearEaseNone, equation);
    textExtraSelected[2].startMoveAnimation(text.getX(), newYPosition + text.getHeight() + textExtra[0].getHeight() + textExtra[1].getHeight() - selectedAreaYOffset, duration, EasingEquations::linearEaseNone, equation);
}

void WheelSelectorExtra::fadeExtraText(int extraTextIndex, uint8_t endAlpha, uint16_t duration)
{
    assert(extraTextIndex < NUMBER_OF_EXTRA_TEXTS);

    textExtra[extraTextIndex].startFadeAnimation(endAlpha, duration);
    textExtraSelected[extraTextIndex].startFadeAnimation(endAlpha, duration);
    textExtraIsVisible[extraTextIndex] = (endAlpha != 0);

    adjustBottomCutOff();
}

void WheelSelectorExtra::adjustBottomCutOff()
{
    bottomCutOff = topCutOff - (textExtra[2].getRect().bottom() - text.getY()) +  - (spaceBetweenTextRows / 3);
    for (int i = 0; i < NUMBER_OF_EXTRA_TEXTS; i++)
    {
        if (!textExtraIsVisible[i])
        {
            bottomCutOff = topCutOff - (textExtra[i].getY() - text.getY()) +  - (spaceBetweenTextRows / 3);
            break;
        }
    }
}
