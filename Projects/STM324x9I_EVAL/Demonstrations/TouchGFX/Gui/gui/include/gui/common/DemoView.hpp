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
