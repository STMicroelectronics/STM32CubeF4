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
  


#ifndef GRAPHDOTS_HPP
#define GRAPHDOTS_HPP

#include <gui/graph_screen/AbstractGraph.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

/**
 * @class GraphDots GraphDots.hpp gui/common/GraphDots.hpp
 *
 * @brief Simple widget capable of drawing a graph of dots. The graph consists of a number of
 *        points with different x values. Points on the graph can be added and removed and the
 *        coordinate system of the graph can easily be setup.
 *
 * @see CanvasWidget
 */
class GraphDots : public AbstractGraph
{
public:
    /**
     * @fn GraphDots::GraphDots();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    GraphDots();

    /**
     * @fn void GraphDots::setDotShape(int dotStartAngle, int dotAngleStep);
     *
     * @brief Sets dot shape.
     *
     *        Sets dot shape. Using the start angle and angle step various shapes can be made.
     *        Example shapes are (0,30) circle, (0,90) diamond, (0,120) triangle, (0,144)
     *        pentagram, (0,135) octagram, (45,90) square, (0,72) pentagon, etc.
     *
     * @param dotStartAngle The start angle.
     * @param dotAngleStep  The angle step.
     */
    void setDotShape(int dotStartAngle, int dotAngleStep);

    /**
     * @fn virtual bool GraphDots::drawCanvasWidget(const Rect& invalidatedArea) const;
     *
     * @brief Draw the graph.
     *
     *        Draw the graph. Care is taken not to spend time drawing graph dots that are
     *        outside the invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @return true if it succeeds, false if there was insufficient memory to draw the line.
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

protected:
    /**
     * @fn virtual Rect GraphDots::getMinimalRectContainingIndices(int firstIndex, int lastIndex) const;
     *
     * @brief Gets minimal rectangle containing dots for the given indices.
     *
     *        Gets minimal rectangle containing dots for the given range of indices. Line width
     *        is taken into account to get the right rectangle.
     *
     * @param firstIndex Zero-based index of the first.
     * @param lastIndex  Zero-based index of the last.
     *
     * @return The minimal rectangle containing indices.
     */
    virtual Rect getMinimalRectContainingIndices(int firstIndex, int lastIndex) const;

    int startAngle, angleStep;

};

#endif
