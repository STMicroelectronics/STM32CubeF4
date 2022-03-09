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
  


#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>

#ifdef SIMULATOR
#include <ctime>
#ifndef _MSC_VER
#include <sys/time.h>
#endif /* _MSC_VER*/
#else

static volatile long lastUs;
extern uint32_t SystemCoreClock;
static int freqMHz;
#endif /* SIMULATOR */

Model::Model() :
    modelListener(0)
{
#ifndef SIMULATOR
    lastUs = HAL::getInstance()->getCPUCycles();
    freqMHz = SystemCoreClock / 1000000;
#endif


    numberOfRooms = 8;
    setSelectedRoom(1);

    roomTemperatureInfo[0].setRoomName(T_TEMPERATURE_ROOM0);
    roomTemperatureInfo[0].setRoomNameHeadline(T_TEMPERATURE_ROOM0_HEADLINE);
    roomTemperatureInfo[0].setRoomScheduleHeadline(T_TEMPERATURE_ROOM0_SCHEDULE);
    roomTemperatureInfo[0].setCurrentTemperature(22);

    roomTemperatureInfo[1].setRoomName(T_TEMPERATURE_ROOM1);
    roomTemperatureInfo[1].setRoomNameHeadline(T_TEMPERATURE_ROOM1_HEADLINE);
    roomTemperatureInfo[1].setRoomScheduleHeadline(T_TEMPERATURE_ROOM1_SCHEDULE);
    roomTemperatureInfo[1].setCurrentTemperature(20);

    roomTemperatureInfo[2].setRoomName(T_TEMPERATURE_ROOM2);
    roomTemperatureInfo[2].setRoomNameHeadline(T_TEMPERATURE_ROOM2_HEADLINE);
    roomTemperatureInfo[2].setRoomScheduleHeadline(T_TEMPERATURE_ROOM2_SCHEDULE);
    roomTemperatureInfo[2].setCurrentTemperature(18);

    roomTemperatureInfo[3].setRoomName(T_TEMPERATURE_ROOM3);
    roomTemperatureInfo[3].setRoomNameHeadline(T_TEMPERATURE_ROOM3_HEADLINE);
    roomTemperatureInfo[3].setRoomScheduleHeadline(T_TEMPERATURE_ROOM3_SCHEDULE);
    roomTemperatureInfo[3].setCurrentTemperature(22);

    roomTemperatureInfo[4].setRoomName(T_TEMPERATURE_ROOM4);
    roomTemperatureInfo[4].setRoomNameHeadline(T_TEMPERATURE_ROOM4_HEADLINE);
    roomTemperatureInfo[4].setRoomScheduleHeadline(T_TEMPERATURE_ROOM4_SCHEDULE);
    roomTemperatureInfo[4].setCurrentTemperature(24);

    roomTemperatureInfo[5].setRoomName(T_TEMPERATURE_ROOM5);
    roomTemperatureInfo[5].setRoomNameHeadline(T_TEMPERATURE_ROOM5_HEADLINE);
    roomTemperatureInfo[5].setRoomScheduleHeadline(T_TEMPERATURE_ROOM5_SCHEDULE);
    roomTemperatureInfo[5].setCurrentTemperature(22);

    roomTemperatureInfo[6].setRoomName(T_TEMPERATURE_ROOM6);
    roomTemperatureInfo[6].setRoomNameHeadline(T_TEMPERATURE_ROOM6_HEADLINE);
    roomTemperatureInfo[6].setRoomScheduleHeadline(T_TEMPERATURE_ROOM6_SCHEDULE);
    roomTemperatureInfo[6].setCurrentTemperature(20);

    roomTemperatureInfo[7].setRoomName(T_TEMPERATURE_ROOM7);
    roomTemperatureInfo[7].setRoomNameHeadline(T_TEMPERATURE_ROOM7_HEADLINE);
    roomTemperatureInfo[7].setRoomScheduleHeadline(T_TEMPERATURE_ROOM7_SCHEDULE);
    roomTemperatureInfo[7].setCurrentTemperature(19);

    for (uint8_t room = 0; room < numberOfRooms; room++)
    {
        for (uint8_t day = RoomTemperatureInfo::MONDAY; day < RoomTemperatureInfo::NUMBER_OF_DAYS; day++)
        {
            roomTemperatureInfo[room].setScheduleDayTemp((RoomTemperatureInfo::weekday) day, 20);
            roomTemperatureInfo[room].setScheduleNightTemp((RoomTemperatureInfo::weekday) day, 18);
        }
    }
}
#ifndef SIMULATOR
//extern volatile uint32_t mcu_load_pct;
static uint8_t mcuLoadLast = 0;
#endif // SIMULATOR

void Model::tick()
{
    Time previousTime = currentTime;

#ifdef SIMULATOR
#ifdef _MSC_VER
    time_t rawtime;
    struct tm timenow;
    time(&rawtime);
    localtime_s(&timenow, &rawtime);

    currentTime.hours =   timenow.tm_hour;
    currentTime.minutes = timenow.tm_min;
    currentTime.seconds = timenow.tm_sec;
    currentTime.milliseconds = 0;

#else
    timeval timenow;
    gettimeofday(&timenow, NULL);

    currentTime.hours = (timenow.tv_sec / 60 / 60) % 24;
    currentTime.minutes = (timenow.tv_sec / 60) % 60;
    currentTime.seconds = timenow.tv_sec % 60;
    currentTime.milliseconds = timenow.tv_usec / 1000;
#endif  /*_MSC_VER*/
#else
    static int milliseconds = 123456;
    uint8_t mcuLoadPct = HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != /*mcu_load_pct*/ mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);
    }

    //long now = cpu_cycles();
    long now = HAL::getInstance()->getCPUCycles();
    milliseconds += (now - lastUs + freqMHz / 2) / freqMHz / 1000;
    lastUs = now;
    currentTime.hours = (milliseconds / 1000 / 60 / 60) % 24;
    currentTime.minutes = (milliseconds / 1000 / 60) % 60;
    currentTime.seconds = (milliseconds / 1000) % 60;
    currentTime.milliseconds = milliseconds % 1000;

#endif /* SIMULATOR */

    if (currentTime.seconds != previousTime.seconds)
    {
        if (modelListener)
        {
            modelListener->timeUpdated(currentTime);
        }
    }
}

uint8_t Model::getNumberOfRooms()
{
    return numberOfRooms;
}

RoomTemperatureInfo& Model::getRoomTemperatureInfo(uint8_t roomIndex)
{
    return roomTemperatureInfo[roomIndex];
}

RoomTemperatureInfo& Model::getRoomTemperatureInfoById(uint8_t roomId)
{
    for (uint8_t i = 0; i < numberOfRooms; i++)
    {
        if (roomTemperatureInfo[i].getRoomId() == roomId)
        {
            return roomTemperatureInfo[i];
        }
    }

    assert(false);
    return roomTemperatureInfo[0];
}

void Model::setSelectedRoom(uint8_t roomId)
{
    selectedRoom = roomId;
}

uint8_t Model::getSelectedRoom()
{
    return selectedRoom;
}

void Model::setRoomTemperature(int16_t temperature)
{
    getRoomTemperatureInfoById(selectedRoom).setCurrentTemperature(temperature);
    modelListener->roomUpdated(getRoomTemperatureInfoById(selectedRoom));
}
