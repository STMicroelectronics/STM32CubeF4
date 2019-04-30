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
  


#ifndef ANIMATEDGRAPHICSTILE_HPP_
#define ANIMATEDGRAPHICSTILE_HPP_

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/common/FrontendApplication.hpp>

using namespace touchgfx;

class AnimatedGraphicsTile : public Container
{
public:
    AnimatedGraphicsTile();
    virtual ~AnimatedGraphicsTile();

    int16_t getWidth()
    {
        return Container::getWidth();
    }
    int16_t getHeight()
    {
        return Container::getHeight();
    }

    void gotoAssociatedScreen()
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoAnimatedGraphicsScreen();
    }
private:
    TextArea tileId;
};

#endif /* ANIMATEDGRAPHICSTILE_HPP_ */
