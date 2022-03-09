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
  


#ifndef GAUGE_HPP
#define GAUGE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/canvas/Shape.hpp>
#include <BitmapDatabase.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#elif USE_BPP==4
#include <touchgfx/widgets/canvas/PainterGRAY4.hpp>
#elif USE_BPP==2
#include <touchgfx/widgets/canvas/PainterGRAY2.hpp>
#else
#error Unknown USE_BPP
#endif
#include <touchgfx/EasingEquations.hpp>

using namespace touchgfx;

/**
 * @class Gauge Gauge.hpp Gauge.hpp
 *
 * @brief A gauge.
 *
 *        A gauge with a needle to point to a set value. The gauge has a bitmap for background
 *        another bitmap for the center (to cover the end of the needle) to enhance the
 *        graphics expression.
 *
 *        To easily be able to set the gauge to a new value, the minimum and maximum values has
 *        to be set with the corresponding degrees so the needle can be positioned correctly at
 *        each value. The range is assumed to be linear.
 *
 *        When the gauge is set to a new value, the needle is animated to the new position
 *        using the given easing equation and duration.
 *
 * @see Container
 */
class Gauge : public Container
{
public:
    /**
     * @fn Gauge::Gauge();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    Gauge();

    /**
     * @fn virtual Gauge::~Gauge();
     *
     * @brief Destructor.
     *
     *        Destructor.
     */
    virtual ~Gauge();

    /**
     * @fn void Gauge::setBitmaps(Bitmap backgroundBmp, Bitmap centerBmp);
     *
     * @brief Sets the bitmaps to be used for background and center (to cover the middle of the
     *        needle).
     *
     *        Sets the bitmaps to be used for background and center (to cover the middle of the
     *        needle).
     *
     * @param backgroundBmp The background bitmap.
     * @param centerBmp     The center bitmap.
     */
    void setBitmaps(Bitmap backgroundBmp, Bitmap centerBmp);

    /**
     * @fn void Gauge::setLimits(int newMinValue, int newMaxValue, int newMinDegree, int newMaxDegree);
     *
     * @brief Sets the limits of the gauge.
     *
     *        Sets the limits of the gauge. The min/max values and angles are given. This
     *        allows for easy setting of a new value and have Gauge find the right angle to
     *        move the needle to.
     *
     * @param newMinValue  The new minimum value.
     * @param newMaxValue  The new maximum value.
     * @param newMinDegree The new minimum degree.
     * @param newMaxDegree The new maximum degree.
     */
    void setLimits(int newMinValue, int newMaxValue, int newMinDegree, int newMaxDegree);

    /**
     * @fn void Gauge::setValue(int newValue);
     *
     * @brief Sets the new value to move the needle to.
     *
     *        Sets the new value to move the needle to. The Gauge should be invalidated before
     *        and after setting a new value.
     *
     * @note The value is not actually set until the animation has finished.
     *
     * @param newValue The new value.
     *
     * @see getValue
     * @see setAnimationDuration
     * @see setEasingEquation
     */
    void setValue(int newValue);

    /**
     * @fn int Gauge::getValue();
     *
     * @brief Gets the current value.
     *
     *        Gets the current value.
     *
     * @note If a value is set using setValue, getValue will not return that value before the
     *       given animation has gone through the given animation steps.
     *
     * @return The value.
     *
     * @see setValue
     */
    int getValue();

    /**
     * @fn void Gauge::setAnimationDuration(uint16_t animationDuration);
     *
     * @brief Sets animation duration.
     *
     *        Sets animation duration for a new value to take effect. The animation is
     *        performed using the equation set through setEasingEquation.
     *
     * @param animationDuration Duration of the animation.
     *
     * @see setValue
     * @see setEasingEquation
     */
    void setAnimationDuration(uint16_t animationDuration);

    /**
     * @fn void Gauge::setEasingEquation(EasingEquation newEasingEquation);
     *
     * @brief Sets easing equation.
     *
     *        Sets easing equation used when a new value is assigned to the gauge.
     *
     * @param newEasingEquation The easing equation.
     *
     * @see setValue
     */
    void setEasingEquation(EasingEquation newEasingEquation);

    /**
     * @fn virtual void Gauge::handleTickEvent();
     *
     * @brief Handles the tick event.
     *
     *        Handles the tick event used when an animation is used to set a new value.
     */
    virtual void handleTickEvent();

    /**
     * @class Needle Gauge.hpp
     *        C:\Projects\playground\app\demo\touchgfx_demo2015_480x272_8MB\gui\include\gui\custom_controls_screen\Gauge.hpp
     *
     * @brief The needle shape.
     *
     *        The needle shape.
     *
     * @see Shape
     */
    class Needle : public Shape<3>
    {
    public:
        Needle()
        {
            ShapePoint<float> points[3] = { { -2.5f, 0}, {0.0f, -70}, {2.5f, 0} };
            setShape(points);
        }
    };

    /**
     * @class NeedleShadow Gauge.hpp
     *        C:\Projects\playground\app\demo\touchgfx_demo2015_480x272_8MB\gui\include\gui\custom_controls_screen\Gauge.hpp
     *
     * @brief The needle shadow shape.
     *
     *        The needle shadow shape.
     *
     * @see Shape
     */
    class NeedleShadow : public Shape<3>
    {
    public:
        NeedleShadow()
        {
            ShapePoint<float> points[3] = { { -4, 0}, {0.0f, -80}, {4, 0} };
            setShape(points);
        }
    };

protected:
    Image background; ///< The background of the gauge
    Image center;     ///< The center image to cover the end of the needle

    Needle needle;               ///< The needle shape
    NeedleShadow needleShadow;   ///< The needle shadow shape
#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 colorPainter;  ///< The painter for the needle
    PainterRGB565 shadowPainter; ///< The painter for the needle shadow
#elif USE_BPP==24
    PainterRGB888 colorPainter;  ///< The painter for the needle
    PainterRGB888 shadowPainter; ///< The painter for the needle shadow
#elif USE_BPP==4
    PainterGRAY4 colorPainter;  ///< The painter for the needle
    PainterGRAY4 shadowPainter; ///< The painter for the needle shadow
#elif USE_BPP==2
    PainterGRAY2 colorPainter;  ///< The painter for the needle
    PainterGRAY2 shadowPainter; ///< The painter for the needle shadow
#else
#error Unknown USE_BPP
#endif

    int minValue;   ///< The minimum value of the gauge
    int maxValue;   ///< The maximum value of the gauge
    int minDegree;  ///< The minimum degree to which the minimum value is mapped
    int maxDegree;  ///< The maximum degree to which the maximum value is mapped

    int currentValue;      ///< The current value of the needle, updated at the end of an animation
    int deltaValue;        ///< The delta value when a new value is set (difference between current value and new value)
    float degreesPerValue; ///< The number of degrees each unit of value corresponds to

    bool gaugeAnimationRunning;      ///< True if gauge animation is running
    uint16_t gaugeAnimationCounter;  ///< The gauge animation counter
    uint16_t gaugeAnimationDuration; ///< The gauge animation duration

    EasingEquation easingEquation;  ///< The easing equation for the animation
};

#endif /* GAUGE_HPP */
