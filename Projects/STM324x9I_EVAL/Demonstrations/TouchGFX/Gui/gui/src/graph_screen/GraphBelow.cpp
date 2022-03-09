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
  


#include <gui/graph_screen/GraphBelow.hpp>

using namespace touchgfx;

GraphBelow::~GraphBelow()
{
}

bool GraphBelow::drawCanvasWidget(const Rect& invalidatedArea) const
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
        canvas.moveTo(widgetXindex(firstIndex), widgetYindex(firstIndex));
        for (int index = firstIndex + 1; index <= lastIndex; index++)
        {
            canvas.lineTo(widgetXindex(index), widgetYindex(index));
        }
        canvas.lineTo(widgetXindex(lastIndex), widgetY(0));
        canvas.lineTo(widgetXindex(firstIndex), widgetY(0));
        //            canvas.lineTo(widgetXindex(lastIndex), widgetY(bottom));
        //            canvas.lineTo(widgetXindex(firstIndex), widgetY(bottom));
        return canvas.render();
    }
    return true;
}

Rect GraphBelow::getMinimalRectContainingIndices(int firstIndex, int lastIndex) const
{
    int minX = widgetXindex(firstIndex).to<int>();
    int maxX = widgetXindex(lastIndex).to<int>();
    int zeroY = widgetY(0).to<int>();
    int firstY = widgetYindex(firstIndex).to<int>();
    int minY = MIN(zeroY, firstY);
    int maxY = MAX(zeroY, firstY);
    for (int index = firstIndex + 1; index <= lastIndex; index++)
    {
        int y = widgetYindex(index).to<int>();
        minY = MIN(minY, y);
        maxY = MAX(maxY, y);
    }

    return Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}
