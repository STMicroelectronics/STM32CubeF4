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
  


#include <gui/easing_equation_screen/EasingEquationGraph.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <math.h>

EasingEquationGraph::EasingEquationGraph()
{

}

void EasingEquationGraph::setup(int newWidth, int newHeight, colortype lineColor, uint16_t lineWidth)
{
    setWidth(newWidth);
    setHeight(newHeight);

    graphLinePainter.setColor(lineColor);

    graphLine.setPosition(0, 0, getWidth(), getHeight());
    graphLine.setPainter(graphLinePainter);
    graphLine.setBuffer(graphBuffer, NUMBER_OF_POINTS);
    graphLine.setLineWidth(lineWidth);
    add(graphLine);
}

void EasingEquationGraph::setRange(int left, int right, int top, int bottom)
{
    graphLine.setRange(left, right, top, bottom);
}
