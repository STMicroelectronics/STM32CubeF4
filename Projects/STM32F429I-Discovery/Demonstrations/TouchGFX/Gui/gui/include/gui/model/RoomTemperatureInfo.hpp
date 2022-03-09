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
  


#ifndef ROOM_TEMPERATURE_INFO_HPP_
#define ROOM_TEMPERATURE_INFO_HPP_

#include <stdint.h>
#include <touchgfx/Texts.hpp>
#include <texts/TextKeysAndLanguages.hpp>

using namespace touchgfx;

class RoomTemperatureInfo
{
public:
    enum weekday
    {
        MONDAY = 0,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY,
        NUMBER_OF_DAYS
    };

    RoomTemperatureInfo() {}
    ~RoomTemperatureInfo() {};

    void setRoomId(uint8_t id)
    {
        roomId = id;
    }
    uint8_t getRoomId()
    {
        return roomId;
    }

    void setRoomName(TEXTS name)
    {
        roomName = name;
    }
    TEXTS getRoomName()
    {
        return roomName;
    }

    void setRoomNameHeadline(TEXTS name)
    {
        roomNameHeadline = name;
    }
    TEXTS getRoomNameHeadline()
    {
        return roomNameHeadline;
    }

    void setRoomScheduleHeadline(TEXTS name)
    {
        roomScheduleHeadline = name;
    }
    TEXTS getRoomScheduleHeadline()
    {
        return roomScheduleHeadline;
    }

    void setCurrentTemperature(int16_t temp)
    {
        currentTemperature = temp;
    }
    int16_t getCurrentTemperature()
    {
        return currentTemperature;
    }

    void setScheduleDayTemp(weekday day, int16_t temp)
    {
        scheduleDayTemp[day] = temp;
    }
    int16_t getScheduleDayTemp(weekday day)
    {
        return scheduleDayTemp[day];
    }

    void setScheduleNightTemp(weekday day, int16_t temp)
    {
        scheduleNightTemp[day] = temp;
    }
    int16_t getScheduleNightTemp(weekday day)
    {
        return scheduleNightTemp[day];
    }

private:
    uint8_t roomId;

    TEXTS roomName;
    TEXTS roomNameHeadline;
    TEXTS roomScheduleHeadline;
    int16_t currentTemperature;
    int16_t scheduleDayTemp[NUMBER_OF_DAYS];
    int16_t scheduleNightTemp[NUMBER_OF_DAYS];
};


#endif /* ROOM_TEMPERATURE_INFO_HPP_ */
