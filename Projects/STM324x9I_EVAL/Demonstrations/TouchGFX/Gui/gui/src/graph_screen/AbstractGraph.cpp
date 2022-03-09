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
  


#include <gui/graph_screen/AbstractGraph.hpp>

using namespace touchgfx;

AbstractGraph::AbstractGraph() :
    CanvasWidget(), linkedGraph(0), left(-1), right(-1), top(-1), bottom(-1), points(0), maxPoints(0), lineWidth(0)
{
    clearGraph();
}

AbstractGraph::~AbstractGraph()
{
}

CWRUtil::Q5 AbstractGraph::widgetX(int x) const
{
    if (graphWidth == 0)
    {
        return CWRUtil::toQ5(0);
    }
    return CWRUtil::toQ5(x - left) * widgetWidth / graphWidth;
}

CWRUtil::Q5 AbstractGraph::widgetY(int y) const
{
    if (graphHeight == 0)
    {
        return CWRUtil::toQ5(0);
    }
    return CWRUtil::toQ5(y - top) * widgetHeight / graphHeight;
}

CWRUtil::Q5 AbstractGraph::widgetXindex(int index) const
{
    return widgetX(points[index].x);
}

CWRUtil::Q5 AbstractGraph::widgetYindex(int index) const
{
    return widgetY(points[index].y);
}

void AbstractGraph::setBuffer(GraphPoint* pointsBuffer, int numPointsInBuffer)
{
    points = pointsBuffer;
    maxPoints = numPointsInBuffer;
    numPoints = 0;
    if (linkedGraph)
    {
        linkedGraph->setBuffer(pointsBuffer, numPointsInBuffer);
    }
}

void AbstractGraph::setLinkedGraph(AbstractGraph& other)
{
    if (linkedGraph == &other)
    {
        return;
    }
    if (linkedGraph != 0)
    {
        linkedGraph->setLinkedGraph(other);
    }
    else
    {
        linkedGraph = &other;
    }
}

void AbstractGraph::setRange(int newLleft, int newRight, int newTop, int newBottom)
{
    if (left != newLleft || right != newRight || top != newTop || bottom != newBottom)
    {
        assignRange(newLleft, newRight, newTop, newBottom);

        if (numPoints > 0)
        {
            updateAndInvalidateCacheForLinesBetweenIndeces(0, numPoints - 1);
        }
    }
}

void AbstractGraph::setX(int16_t x)
{
    CanvasWidget::setX(x);
    if (linkedGraph)
    {
        linkedGraph->setX(x);
    }
}

void AbstractGraph::setY(int16_t y)
{
    CanvasWidget::setY(y);
    if (linkedGraph)
    {
        linkedGraph->setY(y);
    }
}

void AbstractGraph::setWidth(int16_t width)
{
    assignWidth(width);
    if (numPoints > 0)
    {
        updateAndInvalidateCacheForLinesBetweenIndeces(0, numPoints - 1);
    }
}

void AbstractGraph::setHeight(int16_t height)
{
    assignHeight(height);
    if (numPoints > 0)
    {
        updateAndInvalidateCacheForLinesBetweenIndeces(0, numPoints - 1);
    }
}

void AbstractGraph::clearGraph()
{
    numPoints = 0;
    if (linkedGraph)
    {
        linkedGraph->clearGraph();
    }
}

bool AbstractGraph::addValue(int x, int y)
{
    if (numPoints >= maxPoints)
    {
        return false;
    }

    if (numPoints == 0 || x > points[numPoints - 1].x)
    {
        // Point added at end of graph line
        points[numPoints].x = x;
        points[numPoints].y = y;
        setNumPoints(numPoints + 1);
        updateAndInvalidateCacheForLinesBetweenIndeces(numPoints - 2, numPoints - 1);
    }
    else
    {
        int index = findX(x);
        assert(points[index].x >= x && "Bug in binary search algorithm");
        if (points[index].x > x)
        {
            // Point with given X not already in graph line, insert it
            invalidateLineFromIndex(index);
            insertPointAtIndex(index);
            points[index].x = x;
            points[index].y = y;
            updateAndInvalidateCacheForLinesBetweenIndeces(index - 1, index + 1);
        }
        else // (points[index].x == x)
        {
            // Point with given X already exists in graph line, replace it (unless the
            // exact same point was already there)
            if (points[index].y != y)
            {
                // New Y value, so update the point on the graph line
                invalidateLineFromIndex(index - 1);
                invalidateLineFromIndex(index);
                points[index].x = x; // Actually not necessary
                points[index].y = y;
                updateAndInvalidateCacheForLinesBetweenIndeces(index - 1, index + 1);
            }
        }
    }
    return true;
}

bool AbstractGraph::deleteValue(int x)
{
    int index = findX(x);
    if (index < numPoints && points[index].x == x)
    {
        // Point with given X found in graph line
        if (index > 0)
        {
            invalidateLineFromIndex(index - 1);
        }
        invalidateLineFromIndex(index);
        deletePointAtIndex(index);
        if (numPoints > 0)
        {
            updateAndInvalidateCacheForLinesBetweenIndeces(MAX(0, index - 1), index);
        }
        return true;
    }
    return false;
}

bool AbstractGraph::drawCanvasWidget(const Rect& invalidatedArea) const
{
    if (numPoints < 2)
    {
        // A graph line with a single (or not even a single) point is invisible
        return true;
    }

    int firstIndex = 0;
    int lastIndex = numPoints - 1;

    // We can skip the line from index if the left part of the line at index+1 is to the left of the invalidated area
    while (firstIndex < lastIndex &&
            invalidatedArea.x > (widgetXindex(firstIndex + 1) + lineWidth / 2).to<int>())
    {
        firstIndex++;
    }
    // We can skip the line from index if the left part of the line at index-1 is to the right of the invalidated area
    while (lastIndex > firstIndex &&
            invalidatedArea.right() < (widgetXindex(lastIndex - 1) - lineWidth / 2).to<int>())
    {
        lastIndex--;
    }
    if (firstIndex < lastIndex)
    {
        Canvas canvas(this, invalidatedArea);
        canvas.moveTo(xAboveOutgoing(firstIndex), yAboveOutgoing(firstIndex));
        canvas.lineTo(xAboveIncoming(firstIndex + 1), yAboveIncoming(firstIndex + 1));
        for (int index = firstIndex + 1; index < lastIndex; index++)
        {
            canvas.lineTo(xAboveOutgoing(index), yAboveOutgoing(index));
            canvas.lineTo(xAboveIncoming(index + 1), yAboveIncoming(index + 1));
        }
        if (lineWidth == 0)
        {
            CWRUtil::Q5 xBelow = xBelowIncoming(lastIndex);
            if (xBelow > xAboveIncoming(lastIndex))
            {
                canvas.lineTo(xBelow, yBelowIncoming(lastIndex));
            }
            canvas.lineTo(xAboveIncoming(lastIndex), widgetY(bottom));
            canvas.lineTo(xAboveOutgoing(firstIndex), widgetY(bottom));
            xBelow = xBelowOutgoing(firstIndex);
            if (xBelow < xAboveOutgoing(firstIndex))
            {
                canvas.lineTo(xBelow, yBelowOutgoing(firstIndex));
            }
        }
        else
        {
            for (int index = lastIndex - 1; index >= firstIndex; index--)
            {
                canvas.lineTo(xBelowIncoming(index + 1), yBelowIncoming(index + 1));
                canvas.lineTo(xBelowOutgoing(index), yBelowOutgoing(index));
            }
        }
        return canvas.render();
    }
    return true;
}

void AbstractGraph::invalidateRectContainingIndices(int firstIndex, int lastIndex) const
{
    Rect minimalRect = getMinimalRectContainingIndices(firstIndex, lastIndex);
    invalidateRect(minimalRect);

    if (linkedGraph)
    {
        linkedGraph->invalidateRectContainingIndices(firstIndex, lastIndex);
    }
}

void AbstractGraph::updateAndInvalidateCacheForLinesBetweenIndeces(int firstIndex, int lastIndex)
{
    if (firstIndex < 0)
    {
        firstIndex = 0;
    }
    if (lastIndex >= numPoints)
    {
        lastIndex = numPoints - 1;
    }
    if (firstIndex > lastIndex)
    {
        return;
    }

    if (getWidth() == 0 || getHeight() == 0 || left == right || top == bottom)
    {
        // No width or no height
        return;
    }

    // Calculate the slopes of all lines
    for (int index = firstIndex; index < lastIndex; index++)
    {
        CWRUtil::Q5 dy = (widgetYindex(index + 1) - widgetYindex(index));
        CWRUtil::Q5 dx = (widgetXindex(index + 1) - widgetXindex(index));
        CWRUtil::Q5 d = CWRUtil::sqrtQ10(dy * dy + dx * dx);

        if (d != 0) // Should not happen, but if range is zero high/wide it does
        {
            points[index].dy = (lineWidth * dy / d) / 2;
            points[index].dx = (lineWidth * dx / d) / 2;
        }
    }

    invalidateRectContainingIndices(firstIndex, lastIndex);
}

Rect AbstractGraph::getMinimalRect() const
{
    if (numPoints < 1 || graphWidth == 0 || graphHeight == 0)
    {
        return Rect(0, 0, 0, 0);
    }
    return getMinimalRectContainingIndices(0, numPoints - 1);
}

void AbstractGraph::setNumPoints(int numPointsUsed)
{
    numPoints = numPointsUsed;
    if (linkedGraph)
    {
        linkedGraph->setNumPoints(numPointsUsed);
    }
}

void AbstractGraph::invalidateLineFromIndex(int index)
{
    invalidateRectContainingIndices(index, index + 1);
    if (linkedGraph)
    {
        linkedGraph->invalidateLineFromIndex(index);
    }
}

void AbstractGraph::insertPointAtIndex(int index)
{
    assert(numPoints < maxPoints && "Unable to insert more points in array");
    assert(index >= 0 && index <= numPoints && "Illegal index"); // Allow insert at first position outside array
    for (int i = numPoints; i > index; i--)
    {
        points[i] = points[i - 1];
    }
    setNumPoints(numPoints + 1);
}

void AbstractGraph::deletePointAtIndex(int index)
{
    assert(index >= 0 && index < numPoints && "Illegal index");
    setNumPoints(numPoints - 1);
    for (int i = index; i < numPoints; i++)
    {
        points[i] = points[i + 1];
    }
}

int AbstractGraph::findX(int x)
{
    // No points, or x larger than all existing x coordinates
    if (numPoints == 0 || x > points[numPoints - 1].x)
    {
        return numPoints;
    }

    // Setup search limits
    int loIndex = 0;
    int hiIndex = numPoints - 1;
    while (loIndex <= hiIndex)
    {
        int midIndex = (loIndex + hiIndex) / 2;
        int midX = points[midIndex].x;
        if (x < midX)
        {
            // Search lower half
            hiIndex = midIndex - 1;
        }
        else if (x > midX)
        {
            // Search higher half
            loIndex = midIndex + 1;
        }
        else
        {
            // Found
            return midIndex;
        }
    }
    // Return first index over wanted x coordinate
    return loIndex;
}

void AbstractGraph::assignRange(int newLeft, int newRight, int newTop, int newBottom)
{
    left = newLeft;
    right = newRight;
    top = newTop;
    bottom = newBottom;

    graphWidth = CWRUtil::toQ5(newRight - newLeft);
    graphHeight = CWRUtil::toQ5(newBottom - newTop);

    if (linkedGraph)
    {
        linkedGraph->assignRange(newLeft, newRight, newTop, newBottom);
    }
}

void AbstractGraph::assignWidth(int16_t width)
{
    CanvasWidget::setWidth(width);
    widgetWidth = CWRUtil::toQ5(getWidth());
    if (linkedGraph)
    {
        linkedGraph->assignWidth(width);
    }
}

void AbstractGraph::assignHeight(int16_t height)
{
    CanvasWidget::setHeight(height);
    widgetHeight = CWRUtil::toQ5(getHeight());
    if (linkedGraph)
    {
        linkedGraph->assignHeight(height);
    }
}
