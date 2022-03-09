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
  


#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/menu_screen/MenuPresenter.hpp>
#include <gui/menu_screen/Tile.hpp>
#include <gui/menu_screen/LiveDataDisplayTile.hpp>
#include <gui/menu_screen/HomeAutomationTile.hpp>
#include <gui/menu_screen/AnimatedGraphicsTile.hpp>
#include <gui/menu_screen/ChromArtTile.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>

class MenuView : public View<MenuPresenter>
{
public:

    MenuView();

    virtual ~MenuView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();

    void addRoomToHomeAutomationTile(RoomTemperatureInfo& room);
    void initializeTiles();

private:
    static const int TILE_OFFSET = 17;

    Image backgroundTop;
    Image backgroundBottom;

    Tile<LiveDataDisplayTile> liveDataDisplayTile;
    Tile<HomeAutomationTile> homeAutomationTile;
    Tile<AnimatedGraphicsTile> animatedGraphicsTile;
    Tile<ChromArtTile> chromArtTile;

    Callback<MenuView, const AbstractButton&> onButtonPressed;

    void buttonPressedhandler(const AbstractButton& button);
};

#endif // MENU_VIEW_HPP
