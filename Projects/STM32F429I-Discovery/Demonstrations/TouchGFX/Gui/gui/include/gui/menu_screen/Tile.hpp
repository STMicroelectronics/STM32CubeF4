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
