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
  


#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

#include <gui/model/Time.hpp>
#include <gui/model/RoomTemperatureInfo.hpp>

class Model
{
public:
    Model();

    /**
     * Sets the modelListener to point to the currently active presenter. Called automatically
     * when switching screen.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    uint8_t getNumberOfRooms();
    RoomTemperatureInfo& getRoomTemperatureInfo(uint8_t roomIndex);
    RoomTemperatureInfo& getRoomTemperatureInfoById(uint8_t roomId);

    void setSelectedRoom(uint8_t roomId);
    uint8_t getSelectedRoom();
    void setRoomTemperature(int16_t temperature);

    bool getMCULoadActive() const
    {
        return mcuLoadActive;
    }
    void setMCULoadActive(bool active)
    {
        mcuLoadActive = active;
    }

protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;

    bool mcuLoadActive;

    Time currentTime;

    RoomTemperatureInfo roomTemperatureInfo[10];
    uint8_t numberOfRooms;
    uint8_t selectedRoom;
};

#endif /* MODEL_HPP */
