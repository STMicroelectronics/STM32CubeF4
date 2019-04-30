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
  


#include <gui/menu_screen/MenuView.hpp>
#include <BitmapDatabase.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Color.hpp>

MenuView::MenuView() :
    onButtonPressed(this, &MenuView::buttonPressedhandler)
{
}

void MenuView::setupScreen()
{
    backgroundTop.setBitmap(Bitmap(BITMAP_BACKGROUND_TOP_ID));
    backgroundTop.setXY(0, 0);

    backgroundBottom.setBitmap(Bitmap(BITMAP_BACKGROUND_BOTTOM_ID));
    backgroundBottom.setXY(0, HAL::DISPLAY_HEIGHT - backgroundBottom.getHeight());


    liveDataDisplayTile.setXY(TILE_OFFSET, HAL::DISPLAY_HEIGHT - liveDataDisplayTile.getHeight() - TILE_OFFSET);

    homeAutomationTile.setXY(TILE_OFFSET, TILE_OFFSET);

    animatedGraphicsTile.setXY(TILE_OFFSET, homeAutomationTile.getY() + homeAutomationTile.getHeight() + TILE_OFFSET);

    chromArtTile.setXY(animatedGraphicsTile.getX() + animatedGraphicsTile.getWidth() + TILE_OFFSET - 3, animatedGraphicsTile.getY());

    add(backgroundTop);
    add(liveDataDisplayTile);
    add(homeAutomationTile);
    add(animatedGraphicsTile);
    add(chromArtTile);
    add(backgroundBottom);
}

void MenuView::tearDownScreen()
{

}

void MenuView::buttonPressedhandler(const AbstractButton& button)
{

}

void MenuView::addRoomToHomeAutomationTile(RoomTemperatureInfo& room)
{
    homeAutomationTile.getTile().addRoom(room);
}

void MenuView::initializeTiles()
{
    homeAutomationTile.getTile().initialize();
}
