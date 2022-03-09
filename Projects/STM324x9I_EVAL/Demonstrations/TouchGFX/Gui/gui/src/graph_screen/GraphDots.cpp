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
  


#include <gui/graph_screen/GraphDots.hpp>

using namespace touchgfx;

GraphDots::GraphDots() :
    AbstractGraph(), startAngle(0), angleStep(30)
{
}

void GraphDots::setDotShape(int dotStartAngle, int dotAngleStep)
{
    startAngle = dotStartAngle;
    angleStep = dotAngleStep;
}

bool GraphDots::drawCanvasWidget(const Rect& invalidatedArea) const
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
        for (int index = firstIndex; index <= lastIndex; index++)
        {
            CWRUtil::Q5 circleCenterX, circleCenterY;
            circleCenterX = widgetXindex(index);
            circleCenterY = widgetYindex(index);
            canvas.moveTo(circleCenterX + ((lineWidth * CWRUtil::sine(startAngle)) / 2), circleCenterY - ((lineWidth * CWRUtil::cosine(startAngle)) / 2));
            for (int angle = startAngle + angleStep; (angle % 360) != startAngle; angle += angleStep)
            {
                canvas.lineTo(circleCenterX + ((lineWidth * CWRUtil::sine(angle)) / 2), circleCenterY - ((lineWidth * CWRUtil::cosine(angle)) / 2));
            }
        }
        return canvas.render();
    }
    return true;
}

Rect GraphDots::getMinimalRectContainingIndices(int firstIndex, int lastIndex) const
{
    int minX = (widgetXindex(firstIndex) - lineWidth / 2).to<int>();
    int maxX = (widgetXindex(lastIndex) + lineWidth / 2).to<int>();
    int minY = (widgetYindex(firstIndex) - lineWidth / 2).to<int>();
    int maxY = (widgetYindex(firstIndex) + lineWidth / 2).to<int>();
    for (int index = firstIndex + 1; index <= lastIndex; index++)
    {
        minY = MIN(minY, (widgetYindex(index) - lineWidth / 2).to<int>());
        maxY = MAX(maxY, (widgetYindex(index) + lineWidth / 2).to<int>());
    }

    Rect minimalRect = Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);

    return minimalRect;
}
