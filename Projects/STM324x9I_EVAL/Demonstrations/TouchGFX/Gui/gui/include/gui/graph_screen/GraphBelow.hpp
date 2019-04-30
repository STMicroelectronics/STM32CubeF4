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
  


#ifndef GRAPHBELOW_HPP
#define GRAPHBELOW_HPP

#include <gui/graph_screen/GraphLine.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

/**
* @class GraphBelow GraphBelow.hpp gui/common/GraphBelow.hpp
*
* @brief Simple widget capable of drawing a graph line. The graph line consists of a number of
*        points with different x values. Points on the graph can be added and removed and the
*        coordinate system of the graph can easily be setup.
*
* @see CanvasWidget
*/
class GraphBelow : public GraphLine
{
public:
    /**
    * @fn virtual GraphBelow::~GraphBelow()
    *
    * @brief Virtual Destructor. Not used.
    *
    *        Virtual Destructor. Not used.
    */
    virtual ~GraphBelow();

    /**
    * @fn virtual bool GraphBelow::drawCanvasWidget(const Rect& invalidatedArea) const
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
    virtual Rect getMinimalRectContainingIndices(int firstIndex, int lastIndex) const;

};

#endif
