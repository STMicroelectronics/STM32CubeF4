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
  


#ifndef TILE_HPP_
#define TILE_HPP_

#include <touchgfx/mixins/ClickListener.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

/**
 * Container wrapping tile classes. Adding pressed and clicked events to the tile.
 */
template<class T>
class Tile : public Container
{
public:
    Tile() :
        pressedActive(false),
        onTileClicked(this, &Tile::tileClickedhandler)
    {
        setWidth(tile.getWidth() + 6);
        setHeight(tile.getHeight() + 6);

        tile.setXY(3, 3);
        tile.setClickAction(onTileClicked);

        add(tile);
    }

    virtual ~Tile()
    {

    }

    T& getTile()
    {
        return tile;
    }

private:

    ClickListener<T> tile;

    bool pressedActive;

    Callback<Tile, const T&, const ClickEvent& > onTileClicked;

    void tileClickedhandler(const T& t, const ClickEvent& event)
    {
        if (&t == &tile)
        {
            if (event.getType() == ClickEvent::RELEASED)
            {
                if (pressedActive)
                {
                    tile.gotoAssociatedScreen();
                }
            }
            else if (event.getType() == ClickEvent::PRESSED)
            {
                pressedActive = true;
            }
            else if (event.getType() == ClickEvent::CANCEL)
            {
                pressedActive = false;
            }
        }
    }
};

#endif /* TILE_HPP_ */
