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
  


#include <gui/menu_screen/MenuPresenter.hpp>
#include <gui/menu_screen/MenuView.hpp>

MenuPresenter::MenuPresenter(MenuView& v)
    : view(v)
{
}

MenuPresenter::~MenuPresenter()
{
}


void MenuPresenter::activate()
{
    for (int i = 0; i < model->getNumberOfRooms(); i++)
    {
        // Avoid "Master Bedroom" - name too long :)
        if (i != 1)
        {
            view.addRoomToHomeAutomationTile(model->getRoomTemperatureInfo(i));
        }
    }
    view.initializeTiles();
}

void MenuPresenter::deactivate()
{

}
