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
  


#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <gui/graph_screen/GraphLine.hpp>
#include <gui/graph_screen/GraphBelow.hpp>
#include <gui/graph_screen/GraphDots.hpp>
#include <gui/graph_screen/PainterVerticalAlpha.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#elif USE_BPP==4
#include <touchgfx/widgets/canvas/PainterGRAY4.hpp>
#include <touchgfx/widgets/canvas/PainterGRAY4Bitmap.hpp>
#elif USE_BPP==2
#include <touchgfx/widgets/canvas/PainterGRAY2.hpp>
#include <touchgfx/widgets/canvas/PainterGRAY2Bitmap.hpp>
#else
#error Unknown USE_BPP
#endif
#include <BitmapDatabase.hpp>

#include <touchgfx/widgets/Box.hpp>

using namespace touchgfx;

class Graph : public Container
{
public:
    static const int NUMBER_OF_POINTS = 17;

    Graph();
    virtual ~Graph() { }

    void setup(int newWidth, int newHeight, colortype lineColor, colortype backgroundColor);

    bool addValue(int x, int y)
    {
        return graphLine.addValue(x, y);
    }

    bool deleteValue(int x)
    {
        return graphLine.deleteValue(x);
    }

    void clear()
    {
        graphLine.clearGraph();
    }

    void setRange(int left, int right, int top, int bottom) ;

    void setLineVisible(bool lineVisible);
    void setDotsVisible(bool dotsVisible);
    void setDotsBackgroundVisible(bool dotsBackgroundVisible);
    void setAreaVisible(bool areaVisible);

    void setDotShape(int startAngle, int angleStep, int lineWidth);
    void setDotBackgroundShape(int startAngle, int angleStep, int lineWidth);

    uint8_t getAlpha()
    {
        return myAlpha;
    }
    void setAlpha(uint8_t alpha)
    {
        myAlpha = alpha;
        graphLine.setAlpha(myAlpha);
        graphArea.setAlpha(myAlpha);
        graphDots.setAlpha(myAlpha);
        graphDotsBackground.setAlpha(myAlpha);
        invalidate();
    }
protected:

    GraphLine graphLine;
    GraphBelow graphArea;
    GraphDots graphDots;
    GraphDots graphDotsBackground;

    GraphLine::GraphPoint graphBuffer[NUMBER_OF_POINTS];

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 graphLinePainter;
    PainterRGB565 graphDotsPainter;
    PainterRGB565 graphDotsBackgroundPainter;
#elif USE_BPP==24
    PainterRGB888 graphLinePainter;
    PainterRGB888 graphDotsPainter;
    PainterRGB888 graphDotsBackgroundPainter;
#elif USE_BPP==4
    PainterGRAY4 graphLinePainter;
    PainterGRAY4 graphDotsPainter;
    PainterGRAY4 graphDotsBackgroundPainter;
#elif USE_BPP==2
    PainterGRAY2 graphLinePainter;
    PainterGRAY2 graphDotsPainter;
    PainterGRAY2 graphDotsBackgroundPainter;
#else
#error Unknown USE_BPP
#endif
    PainterVerticalAlpha graphAreaPainter;

    uint8_t myAlpha;
};


#endif /* GRAPH_HPP */
