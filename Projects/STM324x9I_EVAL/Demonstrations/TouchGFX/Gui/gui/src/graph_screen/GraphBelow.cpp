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
