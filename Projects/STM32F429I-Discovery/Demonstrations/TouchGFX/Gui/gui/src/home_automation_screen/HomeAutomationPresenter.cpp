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
  


#include <gui/home_automation_screen/HomeAutomationPresenter.hpp>
#include <gui/home_automation_screen/HomeAutomationView.hpp>
#include <gui/common/FrontendApplication.hpp>

HomeAutomationPresenter::HomeAutomationPresenter(HomeAutomationView& v) :
    view(v)
{
}

HomeAutomationPresenter::~HomeAutomationPresenter()
{
}

void HomeAutomationPresenter::activate()
{
    for (uint8_t i = 0; i < model->getNumberOfRooms(); i++)
    {
        view.insertRoom(model->getRoomTemperatureInfo(i));
    }
    roomSelected(model->getSelectedRoom());
}

void HomeAutomationPresenter::deactivate()
{

}

void HomeAutomationPresenter::roomUpdated(RoomTemperatureInfo& room)
{
    view.updateRoom(room);
}

void HomeAutomationPresenter::roomSelected(uint8_t roomId)
{
    model->setSelectedRoom(roomId);
    view.setSelectedMenuItem(model->getRoomTemperatureInfoById(roomId));
}

void HomeAutomationPresenter::newRoomTemperature(int16_t temperature)
{
    model->setRoomTemperature(temperature);
}

void HomeAutomationPresenter::newScheduleAccepted()
{
    view.saveScheduleInfo(model->getRoomTemperatureInfoById(model->getSelectedRoom()));
}

RoomTemperatureInfo& HomeAutomationPresenter::getSelectedRoom()
{
    return model->getRoomTemperatureInfo(model->getSelectedRoom());
}

void HomeAutomationPresenter::exitScreen()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMenuScreen();
}



