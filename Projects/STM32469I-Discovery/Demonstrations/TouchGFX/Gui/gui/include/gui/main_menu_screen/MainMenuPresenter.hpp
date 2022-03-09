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
  

#ifndef MAIN_MENU_PRESENTER_HPP
#define MAIN_MENU_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class MainMenuView;

class MainMenuPresenter : public DemoPresenter
{
public:
    enum MainMenuType
    {
        ANIMATING_BUTTONS_MENU = 0,
        CAROUSEL_MENU,
        NO_MENU
    };

    MainMenuPresenter(MainMenuView& v);
    virtual ~MainMenuPresenter() {};

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    void setSelectedDemoScreenIndex(int elementIndex);
    void setPreviousSelectedMenuType(MainMenuType menuType);

    int getSelectedDemoScreenIndex();

    /**
     * @fn MainMenuType MainMenuPresenter::getPreviousSelectedMenuType();
     *
     * @brief Gets the previous selected menu type. 
     *        Menu types are saved so that you can return to the same menu type
     *        when leaving a demo screen. NO_MENU is used when toggling between
     *        menus.
     *
     * @return The previous selected menu type.
     */
    MainMenuType getPreviousSelectedMenuType();

    virtual void screenSaverMinorTick();

private:
    MainMenuPresenter();
    MainMenuView& view;
};

#endif // MAIN_MENU_PRESENTER_HPP
