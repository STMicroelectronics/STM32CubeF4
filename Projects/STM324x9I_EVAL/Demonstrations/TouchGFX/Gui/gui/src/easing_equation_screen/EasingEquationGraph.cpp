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
