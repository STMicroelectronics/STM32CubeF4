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
  


#ifndef DEMO_VIEW_HPP
#define DEMO_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/common/DemoViewInterface.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

template <class T>
class DemoView : public View<T>, public DemoViewInterface
{
public:
    DemoView() :
        onGotoMenuButtonClicked(this, &DemoView::gotoMenuClickedHandler)
    {
        gotoMenuButton.setAction(onGotoMenuButtonClicked);

        mcuLoadTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_TEXT));
        mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

        Unicode::snprintf(mcuLoadTxtValueBuffer, 5, "%d", 0);
        mcuLoadValueTxt.setWildcard(mcuLoadTxtValueBuffer);
        mcuLoadValueTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_VALUE));
        mcuLoadValueTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    }

protected:
    Button gotoMenuButton;

    TextArea mcuLoadTxt;
    TextAreaWithOneWildcard mcuLoadValueTxt;
    Unicode::UnicodeChar mcuLoadTxtValueBuffer[5];

    Callback<DemoView, const AbstractButton&> onGotoMenuButtonClicked;

    void gotoMenuClickedHandler(const AbstractButton& button)
    {
        View<T>::presenter->backOptionSelected();
    }

    virtual void updateProcessorLoad(uint8_t mcuLoadInProcent)
    {
        Unicode::snprintf(mcuLoadTxtValueBuffer, 5, "%d", mcuLoadInProcent);
        mcuLoadValueTxt.invalidate();
    }
};

#endif
