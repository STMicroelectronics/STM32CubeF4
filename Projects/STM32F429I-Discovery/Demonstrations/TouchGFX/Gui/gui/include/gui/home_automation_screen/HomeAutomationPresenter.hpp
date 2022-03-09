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
  


#ifndef HOME_AUTOMATION_PRESENTER_HPP
#define HOME_AUTOMATION_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class HomeAutomationView;

class HomeAutomationPresenter : public Presenter, public ModelListener
{
public:
    HomeAutomationPresenter(HomeAutomationView& v);
    virtual ~HomeAutomationPresenter();

    virtual void activate();
    virtual void deactivate();

    void roomSelected(uint8_t roomId);
    void newRoomTemperature(int16_t temperature);

    virtual void roomUpdated(RoomTemperatureInfo& room);
    void newScheduleAccepted();
    void getNewSchedule(RoomTemperatureInfo& room);
    void exitScreen();

    RoomTemperatureInfo& getSelectedRoom();

private:
    HomeAutomationPresenter();

    HomeAutomationView& view;
};

#endif // HOME_AUTOMATION_PRESENTER_HPP
