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
  


#include <gui/graph_screen/PieChart.hpp>

PieChart::PieChart():
    minRadius(0),
    maxRadius(0),
    minAngle(0),
    maxAngle(0),
    centerX(0),
    centerY(0),
    animationRunning(false),
    selectAnimationSteps(4),
    selectionAction(0),
    animationEndedAction(0),
    animationEndedCallbackActive(false)
{
    sliceSum = 0;
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        slice[i] = 0;
        displacement[i] = 0;
        expand[i] = 0;
        selected[i] = false;
        circle[i].setCenter(0, 0);
        circle[i].setArc(0, 0);
        circle[i].setVisible(false);
        add(circle[i]);
    }
    startPieChartAnimation(0, 0, 0, 0, 0);
    setTouchable(true);
}

void PieChart::setSlices(int newShares[NUM_ITEMS])
{
    sliceSum = 0;
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        slice[i] = newShares[i];
        sliceSum += slice[i];
    }
}

#if !defined(USE_BPP) || USE_BPP==16
void PieChart::setPainter(int index, AbstractPainterRGB565& painter)
#elif USE_BPP==24
void PieChart::setPainter(int index, AbstractPainterRGB888& painter)
#elif USE_BPP==4
void PieChart::setPainter(int index, AbstractPainterGRAY4& painter)
#elif USE_BPP==2
void PieChart::setPainter(int index, AbstractPainterGRAY2& painter)
#else
#error Unknown USE_BPP
#endif
{
    if (index >= 0 && index < NUM_ITEMS)
    {
        circle[index].setPainter(painter);
    }
}

void PieChart::setRadiuses(int innerRadius, int outerRadius)
{
    this->minRadius = innerRadius;
    this->maxRadius = outerRadius;
    recalculatePieChart();
}

void PieChart::setAngles(int newMinAngle, int newMaxAngle)
{
    minAngle = newMinAngle;
    maxAngle = newMaxAngle;
    recalculatePieChart();
}

void PieChart::getAngles(int& currentMinAngle, int& currentMaxAngle) const
{
    currentMinAngle = minAngle;
    currentMaxAngle = maxAngle;
}

void PieChart::setPieChart(int newStartAngle, int newEndAngle, int newMinRadius, int newMaxRadius)
{
    setAngles(newStartAngle, newEndAngle);
    setRadiuses(newMinRadius, newMaxRadius);
    // Set new 'exit' parameters, these will be pushed to Init parameters when the animation starts
    startAngleExit = minAngle;
    endAngleExit   = maxAngle;
    minRadiusExit  = newMinRadius;
    maxRadiusExit  = newMaxRadius;
}

void PieChart::setCenter(int newCenterX, int newCenterY)
{
    centerX = newCenterX;
    centerY = newCenterY;
    recalculatePieChart();
}

void PieChart::setSelectAnimationSteps(int steps)
{
    if (steps >= 0)
    {
        selectAnimationSteps = steps;
    }
}

void PieChart::setSelected(int index, bool value)
{
    if (index < 0 || index >= NUM_ITEMS)
    {
        return;
    }
    if (value == selected[index])
    {
        return;
    }
    selected[index] = value;
    expand[index] = value ? 1 : -1;
    animateSelected(selectAnimationSteps);
    // Update legend
    if (selectionAction && selectionAction->isValid())
    {
        selectionAction->execute(*this, index);
    }
}

bool PieChart::isSelected(int share) const
{
    return (share >= 0 && share < NUM_ITEMS) ? selected[share] : false;
}

void PieChart::unselectAll()
{
    // Stop animation
    animationRunning = false;

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        selected[i] = 0;
        expand[i] = 0;
        displacement[i] = 0;
    }

    recalculatePieChart();
}

void PieChart::recalculatePieChart()
{
    if (sliceSum == 0)
    {
        return;
    }
    int accum = 0;
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        int lineWidth = (maxRadius - minRadius);
        circle[i].setVisible(lineWidth > 0);
        if (minRadius > 0)
        {
            circle[i].setRadius(minRadius + lineWidth / 2);
            circle[i].setLineWidth(lineWidth);
        }
        else
        {
            circle[i].setRadius(maxRadius);
            circle[i].setLineWidth(0);
        }
        int arcStart = minAngle + accum * (maxAngle - minAngle) / sliceSum;
        accum += slice[i];
        int arcEnd = minAngle + accum * (maxAngle - minAngle) / sliceSum;
        circle[i].updateArcStart(arcStart);
        circle[i].updateArcEnd(arcEnd);
        int middleAngle = (arcStart + arcEnd) / 2;
        if (displacement[i] != 0)
        {
            circle[i].setCenter(centerX + (CWRUtil::toQ5(displacement[i]) * CWRUtil::sine(middleAngle)).to<int>(),
                                centerY - (CWRUtil::toQ5(displacement[i]) * CWRUtil::cosine(middleAngle)).to<int>());
        }
        else
        {
            circle[i].setCenter(centerX, centerY);
        }
    }
}

void PieChart::startPieChartAnimation(int toStartAngle, int toEndAngle, int toMinRadius, int toMaxRadius, int steps, EasingEquation newAngleEquation /*= &EasingEquations::linearEaseNone*/, EasingEquation newRadiusEquation /*= &EasingEquations::linearEaseNone*/)
{
    // Use current 'exit' position as the 'init' position
    startAngleInit = startAngleExit;
    endAngleInit   = endAngleExit;
    minRadiusInit  = minRadiusExit;
    maxRadiusInit  = maxRadiusExit;

    // Set new 'exit' parameters
    startAngleExit = toStartAngle;
    endAngleExit   = toEndAngle;
    minRadiusExit  = toMinRadius;
    maxRadiusExit  = toMaxRadius;

    currentStep = 0;
    animationSteps = steps;
    this->angleEquation = newAngleEquation;
    this->radiusEquation = newRadiusEquation;
    Application::getInstance()->registerTimerWidget(this);
    animationRunning = true;
    animationType = CHANGE_PIECHART;
}

void PieChart::animateSelected(int steps, EasingEquation progressionEquation)
{
    this->currentStep = 0;
    this->animationSteps = steps;
    angleEquation = progressionEquation;
    Application::getInstance()->registerTimerWidget(this);
    animationRunning = true;
    animationType = SELECT_DESELECT;
}

void PieChart::setPieChartSelectionCallback(GenericCallback<const PieChart&, int>& callback)
{
    selectionAction = &callback;
}

void PieChart::setPieChartAnimationEndedCallback(GenericCallback<const PieChart&>& callback)
{
    animationEndedAction = &callback;
    animationEndedCallbackActive = true;
}

void PieChart::activatePieChartAnimationEndedCallback(bool active)
{
    animationEndedCallbackActive = active;
}

void PieChart::setWidth(int16_t width)
{
    Container::setWidth(width);
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        circle[i].setWidth(width);
    }
}

void PieChart::setHeight(int16_t height)
{
    Container::setHeight(height);
    for (int i = 0; i < NUM_ITEMS; i++)
    {
        circle[i].setHeight(height);
    }
}

void PieChart::handleClickEvent(const ClickEvent& evt)
{
    if (animationRunning)
    {
        return;
    }

    if (evt.getType() == ClickEvent::PRESSED)
    {
        int x = evt.getX() - centerX;
        int y = evt.getY() - centerY;
        int distance;
        int angle = CWRUtil::angle(x, y, distance);
        while (angle - minAngle > 360)
        {
            angle -= 360;
        }
        while (angle - minAngle < 0)
        {
            angle += 360;
        }
        if (angle >= minAngle && angle <= maxAngle && ((distance >= minRadius && distance <= maxRadius) || distance < 20))
        {
            bool allExpanded = true;
            int selectedCount = 0;
            for (int i = 0; i < NUM_ITEMS; i++)
            {
                allExpanded &= (displacement[i] > 0);
                selectedCount += (displacement[i] > 0) ? 1 : 0;
            }
            // Inside pie chart
            if (distance < 20)
            {
                // Click in the middle
                if (allExpanded) // All expanded
                {
                    // Hide all
                    for (int i = 0; i < NUM_ITEMS; i++)
                    {
                        if (displacement[i] > 0) // Expanded
                        {
                            setSelected(i, false);
                        }
                    }
                }
                else
                {
                    // Expand all
                    for (int i = 0; i < NUM_ITEMS; i++)
                    {
                        if (displacement[i] == 0) // Expanded
                        {
                            setSelected(i, true);
                        }
                    }
                }
            }
            else
            {
                // Clicked a single slice
                for (int i = 0; i < NUM_ITEMS; i++)
                {
                    expand[i] = 0;
                    if (angle >= circle[i].getArcStart() && angle < circle[i].getArcEnd())
                    {
                        // Clicked this slice
                        if (displacement[i] > 0)
                        {
                            if (selectedCount < 2)
                            {
                                setSelected(i, false);
                            }
                        }
                        else
                        {
                            setSelected(i, true);
                        }
                    }
                    else
                    {
                        // Collapse all slices that have not been clicked
                        if (displacement[i] > 0)
                        {
                            setSelected(i, false);
                        }
                    }
                }
            }
        }
    }
}

void PieChart::handleTickEvent()
{
    Container::handleTickEvent();
    if (animationRunning)
    {
        switch (animationType)
        {
        case CHANGE_PIECHART:
            {
                int newStartAngle = angleEquation(currentStep, startAngleInit, startAngleExit - startAngleInit, animationSteps);
                int newEndAngle   = angleEquation(currentStep, endAngleInit,   endAngleExit   - endAngleInit,   animationSteps);
                int newMinRadius  = radiusEquation(currentStep, minRadiusInit,  minRadiusExit  - minRadiusInit,  animationSteps);
                int newMaxRadius  = radiusEquation(currentStep, maxRadiusInit,  maxRadiusExit  - maxRadiusInit,  animationSteps);
                setAngles(newStartAngle, newEndAngle);
                setRadiuses(newMinRadius, newMaxRadius);
            }
            break;
        case SELECT_DESELECT:
            {
                int showRadius = angleEquation(currentStep, 0, DISPLACEMENT, animationSteps);
                int hideRadius = angleEquation(currentStep, DISPLACEMENT, -DISPLACEMENT, animationSteps);
                for (int i = 0; i < NUM_ITEMS; i++)
                {
                    switch (expand[i])
                    {
                    case -1:
                        displacement[i] = hideRadius;
                        break;
                    case 1:
                        displacement[i] = showRadius;
                        break;
                    }
                }
                recalculatePieChart();
            }
            break;
        }
        if (isVisible())
        {
            invalidate();
        }
        currentStep++;
        if (currentStep > animationSteps)
        {

            Application::getInstance()->unregisterTimerWidget(this);
            animationRunning = false;
            for (int i = 0; i < NUM_ITEMS; i++)
            {
                expand[i] = 0;
            }
            if (animationEndedCallbackActive && animationEndedAction && animationEndedAction->isValid())
            {
                animationEndedAction->execute(*this);
            }
        }
    }
}
