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
  


#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <mvp/View.hpp>
#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

using namespace touchgfx;

class FrontendHeap;

/**
 * The FrontendApplication is the concrete implementation of an MVPApplication
 * for a specific application. Its primary job is to provide functions for
 * switching between screens (presenter/view pairs). By convention these functions
 * are usually called gotoXXScreen().
 *
 * Note that the base class Application is a singleton. Switching screen can be done
 * from anywhere (usually presenters) by e.g. the following:
 * static_cast<FrontendApplication*>(Application::getInstance())->gotoXXScreen();
 */
class FrontendApplication : public MVPApplication
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent();

    void gotoMenuScreen();
    void gotoMenuScreenNoAnimation();
    void gotoHomeAutomationScreen();
    void gotoAnimatedGraphicsScreen();
    void gotoLiveDataDisplayScreen();
    void gotoChromArtScreen();

private:
    Callback<FrontendApplication> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;

    void gotoMenuScreenImpl();
    void gotoMenuScreenNoAnimationImpl();
    void gotoHomeAutomationScreenImpl();
    void gotoAnimatedGraphicsScreenImpl();
    void gotoLiveDataDisplayScreenImpl();
    void gotoChromArtScreenImpl();

};

#endif /* FRONTENDAPPLICATION_HPP */
