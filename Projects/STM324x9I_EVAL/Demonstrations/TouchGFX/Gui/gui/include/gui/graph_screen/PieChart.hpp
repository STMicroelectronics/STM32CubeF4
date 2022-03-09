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
  


#ifndef SEMICIRCLE_HPP
#define SEMICIRCLE_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/AbstractPainterRGB565.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/AbstractPainterRGB888.hpp>
#elif USE_BPP==4
#include <touchgfx/widgets/canvas/AbstractPainterGRAY4.hpp>
#elif USE_BPP==2
#include <touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp>
#else
#error Unknown USE_BPP
#endif
#include <BitmapDatabase.hpp>

#define NUM_ITEMS 6 ///< Number of items in the pie chart
#define DISPLACEMENT 15 ///< How far from the center is a slice moved when selected

using namespace touchgfx;

/**
 * @class PieChart PieChart.hpp
 *        C:\Projects\playground\app\demo\touchgfx_demo2015\gui\include\gui\graph_screen\PieChart.hpp
 *
 * @brief A pie chart.
 *
 *        The PieChart class supports drawing a circular pie chart. It supports a start/end
 *        angle and inner/outer radius allowing everything from a semi donut to a full circle
 *        to be drawn. A pie chart can also be animated from one look to another look.
 *
 * @see Container
 */
class PieChart : public Container
{
public:
    /**
     * @fn PieChart::PieChart();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    PieChart();

    /**
     * @fn void PieChart::setSlices(int newShares[NUM_ITEMS]);
     *
     * @brief Sets the sizes of the slices.
     *
     *        Set the sizes of the slices that make up the pie chart. Each value in the array
     *        should be >0. Each slice is sized relative to the other values in the pie chart.
     *
     * @note The PieChart is not redrawn.
     *
     * @param newShares The new shares.
     */
    void setSlices(int newShares[NUM_ITEMS]);

#if !defined(USE_BPP) || USE_BPP==16
    /**
     * @fn void PieChart::setPainter(int index, AbstractPainterRGB565& painter);
     *
     * @brief Sets the painter for a given slice.
     *
     *        Sets the painter for a given slices.
     *
     * @note The PieChart is not redrawn.
     *
     * @param index        Zero-based index of the.
     * @param [in] painter The new painters.
     */
    void setPainter(int index, AbstractPainterRGB565& painter);
#elif USE_BPP==24
    /**
     * @fn void PieChart::setPainter(int index, AbstractPainterRGB888& painter);
     *
     * @brief Sets the painter for a given slice.
     *
     *        Sets the painter for a given slices.
     *
     * @note The PieChart is not redrawn.
     *
     * @param index        Zero-based index of the.
     * @param [in] painter The new painters.
     */
    void setPainter(int index, AbstractPainterRGB888& painter);
#elif USE_BPP==4
    /**
     * @fn void PieChart::setPainter(int index, AbstractPainterGRAY4& painter);
     *
     * @brief Sets the painter for a given slice.
     *
     *        Sets the painter for a given slices.
     *
     * @param index        Zero-based index of the.
     * @param [in] painter The new painters.
     *
     * @note The PieChart is not redrawn.
     */
    void setPainter(int index, AbstractPainterGRAY4& painter);
#elif USE_BPP==2
    /**
     * @fn void PieChart::setPainter(int index, AbstractPainterGRAY2& painter);
     *
     * @brief Sets the painter for a given slice.
     *
     *        Sets the painter for a given slices.
     *
     * @param index        Zero-based index of the.
     * @param [in] painter The new painters.
     *
     * @note The PieChart is not redrawn.
     */
    void setPainter(int index, AbstractPainterGRAY2& painter);
#else
#error Unknown USE_BPP
#endif

    /**
     * @fn void PieChart::setRadiuses(int innerRadius, int outerRadius);
     *
     * @brief Sets the radiuses.
     *
     *        Sets the radiuses. If the inner radius is zero a full circle is drawn.
     *
     * @note The PieChart is not redrawn.
     *
     * @param innerRadius The inner radius.
     * @param outerRadius The outer radius.
     *
     * @see startAnimation
     * @see setPieChart
     */
    void setRadiuses(int innerRadius, int outerRadius);

    /**
     * @fn void PieChart::setAngles(int newMinAngle, int newMaxAngle);
     *
     * @brief Sets the angles of the pie chart.
     *
     *        Sets the angles of the pie chart. Example values could be (-90,90) which will
     *        draw a semi donut with a flat bottom, (0,360) will draw a full circle with the
     *        first slice starting from the top going clockwise.
     *
     * @note The PieChart is not redrawn.
     *
     * @param newMinAngle The minimum angle.
     * @param newMaxAngle The maximum angle.
     *
     * @see getAngels
     * @see setPieChart
     */
    void setAngles(int newMinAngle, int newMaxAngle);

    /**
     * @fn void PieChart::setPieChart(int newStartAngle, int newEndAngle, int newMinRadius, int newMaxRadius);
     *
     * @brief Sets pie chart.
     *
     *        Sets pie chart with the given angles and radiuses by calling setAngles and
     *        setRadiuses. Also sets the current shape of the PieChart.
     *
     * @note The PieChart is not redrawn.
     *
     * @param newStartAngle The start angle.
     * @param newEndAngle   The end angle.
     * @param newMinRadius  The minimum radius.
     * @param newMaxRadius  The maximum radius.
     *
     * @see setAngles
     * @see setRadiuses
     */
    void setPieChart(int newStartAngle, int newEndAngle, int newMinRadius, int newMaxRadius);

    /**
     * @fn void PieChart::startPieChartAnimation(int toStartAngle, int toEndAngle, int toMinRadius, int toMaxRadius, int steps, EasingEquation newAngleEquation = &EasingEquations::linearEaseNone, EasingEquation newRadiusEquation = &EasingEquations::linearEaseNone);
     *
     * @brief Starts an animation.
     *
     *        Starts an animation. The parameters given specifies what the pie chart should be
     *        animated into.
     *
     * @param toStartAngle      The start angle.
     * @param toEndAngle        The end angle.
     * @param toMinRadius       The minimum radius.
     * @param toMaxRadius       The maximum radius.
     * @param steps             The steps.
     * @param newAngleEquation  The angle equation.
     * @param newRadiusEquation The radius equation.
     */
    void startPieChartAnimation(int toStartAngle, int toEndAngle, int toMinRadius, int toMaxRadius, int steps, EasingEquation newAngleEquation = &EasingEquations::linearEaseNone, EasingEquation newRadiusEquation = &EasingEquations::linearEaseNone);

    /**
     * @fn void PieChart::getAngles(int &currentMinAngle, int &currentMaxAngle) const;
     *
     * @brief Gets the angles.
     *
     *        Gets the angles previously set with setAngles.
     *
     * @param [out] currentMinAngle The minimum angle.
     * @param [out] currentMaxAngle The maximum angle.
     *
     * @see setAngles
     */
    void getAngles(int& currentMinAngle, int& currentMaxAngle) const;

    /**
     * @fn void PieChart::setCenter(int newCenterX, int newCenterY);
     *
     * @brief Sets a center of the pie chart.
     *
     *        Sets a center of the pie chart relative to the top left corner of the widget.
     *
     * @note The PieChart is not redrawn. It is recommended to issue an invalidate before and
     *       after calling setCenter.
     *
     * @param newCenterX The center x coordinate.
     * @param newCenterY The center y coordinate.
     */
    void setCenter(int newCenterX, int newCenterY);

    /**
     * @fn void PieChart::setSelectAnimationSteps(int steps);
     *
     * @brief Sets select animation steps.
     *
     *        Sets select animation steps. This is the number of steps to animate the slice
     *        that is selected when it is moved towards or away from the center of the pie.
     *
     * @param steps The steps.
     */
    void setSelectAnimationSteps(int steps);

    /**
     * @fn void PieChart::recalculatePieChart();
     *
     * @brief Calculates the chart.
     *
     *        Calculates the chart after changes of slice sizes, displacements of selected
     *        slices, etc.
     */
    void recalculatePieChart();

    /**
     * @fn void PieChart::animateSelected(int steps, EasingEquation progressionEquation = &EasingEquations::linearEaseNone);
     *
     * @brief Start animation of the selected slice(s).
     *
     *        Start animation of the selected slice(s).
     *
     * @param steps               The steps.
     * @param progressionEquation The progression equation.
     *
     * @see setSelected
     */
    void animateSelected(int steps, EasingEquation progressionEquation = &EasingEquations::linearEaseNone);

    /**
     * @fn void PieChart::setSelected(int slice, bool value);
     *
     * @brief Selects or de-selects a slice.
     *
     *        Selects or de-selects a slice. The slice will automatically be animated. If a
     *        selection callback has been set, it is also called.
     *
     * @param slice Zero-based index of the slice.
     * @param value true to value.
     *
     * @see animateSelected
     * @see setPieChartSelectionCallback
     */
    void setSelected(int slice, bool value);

    /**
     * @fn bool PieChart::isSelected(int slice) const;
     *
     * @brief Query if 'slice' is selected.
     *
     * @param slice The slice.
     *
     * @return true if selected, false if not.
     *
     * @see setSelected
     */
    bool isSelected(int slice) const;

    /**
     * @fn void PieChart::unselectAll();
     *
     * @brief Unselect all slices and halt animation.
     *
     *        Unselect all slices and halt animation.
     */
    void unselectAll();

    /**
     * @fn void PieChart::setPieChartSelectionCallback(GenericCallback<const PieChart&, int>& callback)
     *
     * @brief Associates an action to be performed when slice is selected or de-selected.
     *
     *        Associates an action to be performed when slice is selected or de-selected.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the PieChart.
     *
     * @see GenericCallback
     */
    void setPieChartSelectionCallback(GenericCallback<const PieChart&, int>& callback);

    /**
     * @fn void PieChart::setPieChartAnimationEndedCallback(GenericCallback<const PieChart&>& callback)
     *
     * @brief Associates an action to be performed when an animation finishes.
     *
     *        Associates an action to be performed when an animation finishes.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the PieChart.
     *
     * @see GenericCallback
     */
    void setPieChartAnimationEndedCallback(GenericCallback<const PieChart&>& callback);

    /**
     * @fn void PieChart::activatePieChartAnimationEndedCallback(bool active);
     *
     * @brief Activates or deactivates the callback when an animation ends.
     *
     * @param active true to activate callback, false to deactivate.
     */
    void activatePieChartAnimationEndedCallback(bool active);

    /**
     * @fn virtual void PieChart::setWidth(int16_t width);
     *
     * @brief Sets width of the widget.
     *
     *        Sets width of the widget and all pie chart slices in the PieChart. Inherited from
     *        Container.
     *
     * @param width The width.
     */
    virtual void setWidth(int16_t width);

    /**
     * @fn virtual void PieChart::setHeight(int16_t height);
     *
     * @brief Sets height of the widget.
     *
     *        Sets height of the widget and all pie chart slices in the PieChart. Inherited from
     *        Container.
     *
     * @param height The height.
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn virtual void PieChart::handleClickEvent(const ClickEvent& evt);
     *
     * @brief Handles the click event described by evt.
     *
     *        Handles the click event described by evt. Clicking a slice will (de)select it,
     *        clicking the center of the pie chart will (de)select all slices.
     *
     * @param evt The event.
     */
    virtual void handleClickEvent(const ClickEvent& evt);

    /**
     * @fn virtual void PieChart::handleTickEvent();
     *
     * @brief Handles the tick event.
     *
     *        Handles the tick event. This takes care of animation.
     */
    virtual void handleTickEvent();

private:
    int slice[NUM_ITEMS];                     ///< The size of each slice
    int sliceSum;                             ///< The sum of all slices
    Circle circle[NUM_ITEMS];                 ///< Each slice is a (part of a) Circle
    int displacement[NUM_ITEMS];              ///< How far has each slice been moved from the center during (de)select
    int minRadius, maxRadius;                 ///< Min/max radius of the PieChart
    int minAngle, maxAngle;                   ///< Min/max angle of the PieChart
    int centerX, centerY;                     ///< Center of the PieChart

    bool animationRunning;    ///< True when an animation is ongoing
    int expand[NUM_ITEMS];    ///< Should animation expand the given slice (-1,0,1)
    bool selected[NUM_ITEMS]; ///< Is a slice selected
    int selectAnimationSteps;
    enum
    {
        CHANGE_PIECHART, ///< Animation is changing the shape of the PieChart
        SELECT_DESELECT  ///< Animation is moving a slice to (de)select
    };
    int animationType;                                      ///< Is the PieChart being changed or is a slice (de)selected
    int startAngleInit, startAngleExit;                     ///< startAngle Init and Exit values for animation
    int endAngleInit, endAngleExit;                         ///< endAngle Init and Exit values for animation
    int minRadiusInit, minRadiusExit;                       ///< minRadius Init and Exit values for animation
    int maxRadiusInit, maxRadiusExit;                       ///< maxRadius Init and Exit values for animation
    int currentStep;                                        ///< The current step in animation
    int animationSteps;                                     ///< The total animation steps
    EasingEquation angleEquation;                           ///< The equation for changing angles
    EasingEquation radiusEquation;                          ///< The equation for changing radius
    GenericCallback<const PieChart&, int>* selectionAction; ///< A pie has been (de)selected
    GenericCallback<const PieChart&>* animationEndedAction; ///< Animation Ended
    bool animationEndedCallbackActive;                      ///< True to activate callback when animation has ended
};

#endif
