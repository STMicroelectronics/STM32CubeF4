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
