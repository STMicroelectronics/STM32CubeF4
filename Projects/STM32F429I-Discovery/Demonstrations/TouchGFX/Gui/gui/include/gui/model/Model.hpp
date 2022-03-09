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
