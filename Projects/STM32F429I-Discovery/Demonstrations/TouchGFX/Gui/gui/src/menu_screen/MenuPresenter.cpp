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
