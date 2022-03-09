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
  


#ifndef ABSTRACTGRAPH_HPP
#define ABSTRACTGRAPH_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

/**
 * @class AbstractGraph AbstractGraph.hpp gui/common/AbstractGraph.hpp
 *
 * @brief Simple widget capable of drawing a graph line. The graph line consists of a number of
 *        points with different x values. Points on the graph can be added and removed and the
 *        coordinate system of the graph can easily be setup. The graph is limited to integer
 *        values for x and y and the values should not exceed 1000.
 *
 * @see CanvasWidget
 */
class AbstractGraph : public CanvasWidget
{
public:
    /**
     * @class GraphPoint AbstractGraph.hpp gui/common/AbstractGraph.hpp
     *
     * @brief A graph point on the graph line.
     *
     *        A graph point on the graph line. The point stores the slope of the line going from
     *        this point to the next on the graph line.
     */
    class GraphPoint
    {
    public:
        /**
         * @fn GraphPoint::GraphPoint()
         *
         * @brief Default constructor.
         *
         *        Default constructor.
         */
        GraphPoint()
        {
        }

        int x; ///< The x coordinate of the points.
        int y; ///< The y coordinate of the points.
        CWRUtil::Q5 dx, dy; ///< The slope from the GridPoint to the next on the AbstractGraph
    };

    /**
     * @fn AbstractGraph::AbstractGraph()
     *
     * @brief Constructs a new NewGraph.
     *
     *        Constructs a new NewGraph.
     */
    AbstractGraph();

    /**
     * @fn virtual AbstractGraph::~AbstractGraph()
     *
     * @brief Virtual Destructor.
     *
     *        Virtual Destructor. Not used.
     */
    virtual ~AbstractGraph();

    /**
     * @fn CWRUtil::Q5 AbstractGraph::widgetX(int x) const;
     *
     * @brief Calculate Widget x coordinate.
     *
     *        Calculate widget x coordinate from a graph x coordinate.
     *
     * @param x The x coordinate.
     *
     * @return The x coordinate in the widget in Q5 format.
     */
    CWRUtil::Q5 widgetX(int x) const;

    /**
     * @fn CWRUtil::Q5 AbstractGraph::widgetY(int y) const;
     *
     * @brief Calculate Widget y coordinate.
     *
     *        Calculate widget y coordinate from a graph y coordinate.
     *
     * @param y The y coordinate.
     *
     * @return The y coordinate in the widget in Q5 format.
     */
    CWRUtil::Q5 widgetY(int y) const;

    /**
     * @fn CWRUtil::Q5 AbstractGraph::widgetXindex(int index) const;
     *
     * @brief Calculate Widget x coordinate of the index'th point.
     *
     *        Calculate widget x coordinate of the index'th point.
     *
     * @param index The index.
     *
     * @return The x coordinate in the widget in Q5 format.
     */
    CWRUtil::Q5 widgetXindex(int index) const;

    /**
     * @fn CWRUtil::Q5 AbstractGraph::widgetYindex(int index) const;
     *
     * @brief Calculate Widget y coordinate of the index'th point.
     *
     *        Calculate widget y coordinate of the index'th point.
     *
     * @param index The index.
     *
     * @return The y coordinate in the widget in Q5 format.
     */
    CWRUtil::Q5 widgetYindex(int index) const;

    /**
     * @fn void AbstractGraph::setBuffer(GraphPoint *pointsBuffer, int numPointsInBuffer)
     *
     * @brief Sets a buffer to be used to store points on the graph line.
     *
     *        Sets a buffer to be used to store points on the graph line.
     *
     * @param [in] pointsBuffer If non-null, buffer for points data.
     * @param numPointsInBuffer Number of points in buffers.
     */
    void setBuffer(GraphPoint* pointsBuffer, int numPointsInBuffer);

    /**
     * @fn void AbstractGraph::setLinkedGraph(AbstractGraph &other);
     *
     * @brief Sets linked graph.
     *
     *        Sets linked graph which is updated when necessary. Invalidation etc is also
     *        handled. Linked graphs share the memory used for graph points and will thus
     *        reduce memory requirements.
     *
     * @note this is only intended to link to another graph with lineWidth==0 at the moment.
     *
     * @param [in] other The other graph to link to.
     */
    void setLinkedGraph(AbstractGraph& other);

    /**
     * @fn void AbstractGraph::setRange(int newLleft, int newRight, int newTop, int newBottom)
     *
     * @brief Sets the range of the graph.
     *
     *        Sets the range of the graph.
     *
     * @param newLeft   The value at the left of the graph area.
     * @param newRight  The value at the right of the graph area.
     * @param newTop    The value at the top of the graph area.
     * @param newBottom The value at the bottom of the graph area.
     */
    void setRange(int newLleft, int newRight, int newTop, int newBottom);

    /**
     * @fn virtual void Drawable::setX(int16_t x)
     *
     * @brief Sets the x coordinate of this drawable.
     *
     *        Sets the x coordinate of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param x The new x value, relative to the parent. A negative value is allowed.
     */
    virtual void setX(int16_t x);

    /**
     * @fn virtual void Drawable::setY(int16_t y)
     *
     * @brief Sets the y coordinate of this drawable.
     *
     *        Sets the y coordinate of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param y The new y value, relative to the parent. A negative value is allowed.
     */
    virtual void setY(int16_t y);

    /**
     * @fn virtual void AbstractGraph::setWidth(int16_t width)
     *
     * @brief Sets the width of this drawable.
     *
     *        Sets the width of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param width The new width.
     */
    virtual void setWidth(int16_t width);

    /**
     * @fn virtual void AbstractGraph::setHeight(int16_t height)
     *
     * @brief Sets the height of this drawable.
     *
     *        Sets the height of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param height The new height.
     */
    virtual void setHeight(int16_t height);

    /**
     * @fn template <class T> void AbstractGraph::setLineWidth(T width)
     *
     * @brief Sets the width for this graph line.
     *
     *        Sets the width for this graph line. A line width of 0 is handled specially as
     *        it will fill the area below the graph line.
     *
     * @note The graph line is invalidated.
     *
     * @tparam T Generic type parameter.
     * @param width The width of the line measured in pixels.
     */
    template <class T>
    void setLineWidth(T width)
    {
        CWRUtil::Q5 newLineWidth = CWRUtil::toQ5(width);
        if (lineWidth == newLineWidth)
        {
            return;
        }

        lineWidth = newLineWidth;

        if (numPoints > 0)
        {
            updateAndInvalidateCacheForLinesBetweenIndeces(0, numPoints - 1);
        }
    }

    /**
     * @fn template <class T> T AbstractGraph::getLineWidth() const
     *
     * @brief Gets line width.
     *
     *        Gets line width.
     *
     * @tparam T Generic type parameter.
     *
     * @return The line width.
     */
    template <class T>
    T getLineWidth() const
    {
        return int(lineWidth) / T(Rasterizer::POLY_BASE_SIZE);
    }

    /**
     * @fn virtual void NewGraph::clearGraph()
     *
     * @brief Clears the graph.
     *
     * @note The graph line is not invalidated.
     */
    virtual void clearGraph();

    /**
     * @fn virtual bool AbstractGraph::addValue(int x, int y)
     *
     * @brief Adds a value to the graph.
     *
     *        Adds a value to the graph.
     *
     * @note The updated graph line is invalidated.
     *
     * @param x The value.
     * @param y The y coordinate.
     *
     * @return true if it succeeds, false if it fails.
     */
    virtual bool addValue(int x, int y);

    /**
     * @fn virtual bool AbstractGraph::deleteValue(int x)
     *
     * @brief Deletes the value on the graph line at the given x coordinate.
     *
     *        Deletes the value on the graph line at the given x coordinate.
     *
     * @param x The x coordinate.
     *
     * @return true if a point with the given x value was found and deleted, false otherwise.
     */
    virtual bool deleteValue(int x);

    /**
     * @fn virtual bool AbstractGraph::drawCanvasWidget(const Rect& invalidatedArea) const
     *
     * @brief Draw the graph line.
     *
     *        Draw the graph line. Care is taken not to spend time drawing graphs lines
     *        that are outside the invalidated area.
     *
     * @param invalidatedArea The rectangle to draw, with coordinates relative to this drawable.
     *
     * @return true if it succeeds, false if there was insufficient memory to draw the line.
     */
    virtual bool drawCanvasWidget(const Rect& invalidatedArea) const;

protected:
    virtual Rect getMinimalRectContainingIndices(int firstIndex, int lastIndex) const = 0;

    virtual void invalidateRectContainingIndices(int firstIndex, int lastIndex) const;

    /**
     * @fn Rect AbstractGraph::updateCacheForLinesBetweenIndeces(int firstIndex, int lastIndex)
     *
     * @brief Updates the NewGraph cache for one element.
     *
     *        The cache is used to be able to quickly redraw the graph line without
     *        calculating the slope of all the lines that make up the graph line.
     *
     * @param firstIndex Zero-based index of the first index to update cache for.
     * @param lastIndex  Zero-based index of the last index to update cache for (actually lastIndex-1 is the last).
     *
     * @return A Rect enclosing the graph line in the given range from firstIndex to lastIndex (inclusive).
     */
    virtual void updateAndInvalidateCacheForLinesBetweenIndeces(int firstIndex, int lastIndex);

    /**
     * @fn Rect AbstractGraph::getMinimalRect() const
     *
     * @brief Gets minimal rectangle containing the NewGraph. Used for invalidating only the
     *        required part of the screen.
     *
     * @return The minimal rectangle.
     */
    virtual Rect getMinimalRect() const;

    AbstractGraph* linkedGraph; ///< Graph sharing data with this graph. Updated when setting range, changing values etc.

    int         left, right, top, bottom; ///< Limits of the values of the graph line area
    GraphPoint* points;                   ///< Pointer to buffer where the coordinates are stored
    int         maxPoints;                ///< Maximum number of points possible on graph line
    int         numPoints;                ///< Number of points currently in use
    CWRUtil::Q5 lineWidth;                ///< Line with (0=fill below graph line)
    CWRUtil::Q5 widgetWidth;              ///< Width of the widget as Q5
    CWRUtil::Q5 widgetHeight;             ///< Height of the widget as Q5
    CWRUtil::Q5 graphWidth;               ///< Width of the graph range (left to right) as Q5
    CWRUtil::Q5 graphHeight;              ///< Height of the graph range (top to bottom) as Q5

    /**
     * @fn void AbstractGraph::setNumPoints(int numPointsUsed)
     *
     * @brief Sets number of used points.
     *
     *        Sets number of used points, synchronize with linked graph.
     *
     * @param numPointsUsed The number of used points.
     */
    virtual void setNumPoints(int numPointsUsed);

    /**
     * @fn void AbstractGraph::invalidateLineFromIndex(int index)
     *
     * @brief Invalidate line from index to index+1.
     *
     *        Invalidate line from index to index+1.
     *
     * @param index Zero-based index of the left end point.
     */
    virtual void invalidateLineFromIndex(int index);

    /**
     * @fn void AbstractGraph::insertPointAtIndex(int index)
     *
     * @brief Inserts a point at the given index.
     *
     *        There is only made room for the point, by moving all points after the given
     *        index one index up.
     *
     * @param index Zero-based index of the point.
     */
    virtual void insertPointAtIndex(int index);

    /**
     * @fn void AbstractGraph::deletePointAtIndex(int index)
     *
     * @brief Deletes the point at the given index.
     *
     *        Deletes the point at the given index.
     *
     * @param index Zero-based index of the point.
     */
    virtual void deletePointAtIndex(int index);

    /**
     * @fn int AbstractGraph::findX(int x)
     *
     * @brief Searches for the given x coordinate.
     *
     *        Searches for the given x coordinate. If the x coordinate is not found, the
     *        closest index is returned so that the point at the returned index has the
     *        first x value higher than the sought x coordinate (or at usedPoints if the x
     *        coordinate is higher than the x coordinate of all existing points)
     *
     * @param x The x coordinate.
     *
     * @return The index of the found x coordinate where the x should be inserted.
     */
    virtual int findX(int x);

    // A handful of functions to get the Q5 coordinates of the corners of the line segments making up the line
    CWRUtil::Q5 xAboveOutgoing(int index) const
    {
        return widgetXindex(index) + points[index].dy;
    }
    CWRUtil::Q5 yAboveOutgoing(int index) const
    {
        return widgetYindex(index) - points[index].dx;
    }
    CWRUtil::Q5 xBelowOutgoing(int index) const
    {
        return widgetXindex(index) - points[index].dy;
    }
    CWRUtil::Q5 yBelowOutgoing(int index) const
    {
        return widgetYindex(index) + points[index].dx;
    }
    CWRUtil::Q5 xAboveIncoming(int index) const
    {
        return widgetXindex(index) + points[index - 1].dy;
    }
    CWRUtil::Q5 yAboveIncoming(int index) const
    {
        return widgetYindex(index) - points[index - 1].dx;
    }
    CWRUtil::Q5 xBelowIncoming(int index) const
    {
        return widgetXindex(index) - points[index - 1].dy;
    }
    CWRUtil::Q5 yBelowIncoming(int index) const
    {
        return widgetYindex(index) + points[index - 1].dx;
    }

private:
    /**
     * @fn void AbstractGraph::assignRange(int newLeft, int newRight, int newTop, int newBottom)
     *
     * @brief Sets the range of the graph.
     *
     *        Sets the range of the graph.
     *
     * @param newLeft   The value at the left of the graph area.
     * @param newRight  The value at the right of the graph area.
     * @param newTop    The value at the top of the graph area.
     * @param newBottom The value at the bottom of the graph area.
     */
    void assignRange(int newLeft, int newRight, int newTop, int newBottom);

    /**
     * @fn virtual void AbstractGraph::assignWidth(int16_t width)
     *
     * @brief Sets the width of this drawable.
     *
     *        Sets the width of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param width The new width.
     */
    virtual void assignWidth(int16_t width);

    /**
     * @fn virtual void AbstractGraph::assignHeight(int16_t height)
     *
     * @brief Sets the height of this drawable.
     *
     *        Sets the height of this drawable.
     *
     * @note Changing this does not automatically yield a redraw.
     *
     * @param height The new height.
     */
    virtual void assignHeight(int16_t height);

};

#endif
