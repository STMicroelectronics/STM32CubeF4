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
  


#ifndef EASING_EQUATION_GRAPH_HPP
#define EASING_EQUATION_GRAPH_HPP

#include <gui/graph_screen/GraphLine.hpp>
#include <touchgfx/containers/Container.hpp>
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
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class EasingEquationGraph : public Container
{
public:

    EasingEquationGraph();
    virtual ~EasingEquationGraph() { }

    void setup(int newWidth, int newHeight, colortype lineColor, uint16_t lineWidth);

    template <class T>
    bool addValue(T x, T y)
    {
        return graphLine.addValue(x, y);
    }

    template <class T>
    bool deleteValue(T x)
    {
        return graphLine.deleteValue(x);
    }

    void clear()
    {
        graphLine.clearGraph();
    }

    void setRange(int left, int right, int top, int bottom) ;

protected:
    static const int NUMBER_OF_POINTS = 52;

    GraphLine graphLine;

    GraphLine::GraphPoint graphBuffer[NUMBER_OF_POINTS];

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 graphLinePainter;
#elif USE_BPP==24
    PainterRGB888 graphLinePainter;
#elif USE_BPP==4
    PainterGRAY4 graphLinePainter;
#elif USE_BPP==2
    PainterGRAY2 graphLinePainter;
#else
#error Unknown USE_BPP
#endif
};


#endif /* EASING_EQUATION_GRAPH_HPP */
