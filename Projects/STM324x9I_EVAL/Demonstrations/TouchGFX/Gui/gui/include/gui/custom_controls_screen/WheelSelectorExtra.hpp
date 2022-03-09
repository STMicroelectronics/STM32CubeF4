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
  


#ifndef DATE_PICKER_WHEEL_DAY_HPP
#define DATE_PICKER_WHEEL_DAY_HPP

#include <gui/custom_controls_screen/WheelSelector.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

/**
 * @class WheelSelectorExtra WheelSelectorExtra.hpp gui/custom_controls_screen/WheelSelectorExtra.hpp
 *
 * @brief A wheel selector that introduces a number of extra text areas in the bottom of the date
 *        picker.
 *
 *        A wheel selector that introduces a number of extra text areas in the bottom of the
 *        date picker. This allows the contents of the wheel selector to be changed.
 *
 * @note This widget is part of the TouchGFX Open Repository.
 *       https://github.com/draupnergraphics/touchgfx-open-repository
 *
* @see WheelSelector
 */
class WheelSelectorExtra : public WheelSelector
{
public:
    /**
     * @fn WheelSelectorDay::WheelSelectorDay();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    WheelSelectorExtra();

    /**
     * @fn virtual WheelSelectorDay::~WheelSelectorDay();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~WheelSelectorExtra();

    /**
     * @fn void WheelSelectorExtra::setupDatePickerWheelDay(int width, int height, int textXPosition, int textYPositionOfIndex0Selected, TEXTS wheelText, TEXTS wheelTextExtra0, TEXTS wheelTextExtra1, TEXTS wheelTextExtra2);
     *
     * @brief Sets up the date picker wheel.
     *
     *        Sets up the date picker wheel.
     *
     * @param width                         The width.
     * @param height                        The height.
     * @param textXPosition                 The text x coordinate position.
     * @param textYPositionOfIndex0Selected The text y coordinate position of index 0 selected.
     * @param wheelText                     The wheel text.
     * @param wheelTextExtra0               The wheel extra text 1.
     * @param wheelTextExtra1               The wheel extra text 2.
     * @param wheelTextExtra2               The wheel extra text 3.
     */
    void setupDatePickerWheelDay(int width, int height, int textXPosition, int textYPositionOfIndex0Selected, TEXTS wheelText, TEXTS wheelTextExtra0, TEXTS wheelTextExtra1, TEXTS wheelTextExtra2);

    /**
     * @fn virtual void WheelSelectorExtra::setTextColor(colortype normalColor, colortype selectedColor, colortype selectedBackgroundColor, int selectedYOffset = 0, int selectedHeight = 0);
     *
     * @brief Sets text color of normal/selected text.
     *
     *        Sets text color of normal/selected text. If the colors are the same, the overlays
     *        for showing selected text is hidden, otherwise it is setup with the given y
     *        coordinate offset.
     *
     * @param normalColor             The color of normal text.
     * @param selectedColor           The color of selected text.
     * @param selectedBackgroundColor The selected background color.
     * @param selectedYOffset         The selected area (where the text changes color) y coordinate
     *                                offset.
     * @param selectedHeight          The selected height.
     */
    virtual void setTextColor(colortype normalColor, colortype selectedColor, colortype selectedBackgroundColor, int selectedYOffset = 0, int selectedHeight = 0);
    /**
     * @fn void WheelSelectorExtra::fadeExtraText(int extraTextIndex, uint8_t endAlpha, uint16_t duration);
     *
     * @brief Fade extra texts.
     *
     *        Fade extra texts so the text list is smoothly extended or retracted.
     *
     * @param extraTextIndex Zero-based index of the extra text.
     * @param endAlpha       The end alpha.
     * @param duration       The duration.
     */
    void fadeExtraText(int extraTextIndex, uint8_t endAlpha, uint16_t duration);

protected:
    static const int NUMBER_OF_EXTRA_TEXTS = 3;

    FadeAnimator<MoveAnimator<TextArea> > textExtra[NUMBER_OF_EXTRA_TEXTS];
    FadeAnimator<MoveAnimator<TextArea> > textExtraSelected[NUMBER_OF_EXTRA_TEXTS];
    bool textExtraIsVisible[NUMBER_OF_EXTRA_TEXTS];

    virtual void moveText(int newYPosition);
    virtual void animateMoveText(int newYPosition, int duration, EasingEquation equation);
    void adjustBottomCutOff();
};


#endif /* DATE_PICKER_WHEEL_DAY_HPP */
