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
  


#ifndef LEGEND_ENTRY_HPP
#define LEGEND_ENTRY_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

using namespace touchgfx;

class LegendEntry : public Container
{
public:
    LegendEntry();
    virtual ~LegendEntry();

    void setBitmaps(BitmapId squareNotSelectedBmp, BitmapId squareSelectedBmp);
    void setText(TEXTS textId);
    void setValueText(int value);
    void setSelected(bool selected);

    void setSelectionCallback(GenericCallback<const LegendEntry&>& callback)
    {
        selectionAction = &callback;
    }

private:
    BitmapId squareNotSelected;
    BitmapId squareSelected;

    Image square;

    TextArea text;
    TextAreaWithOneWildcard valueText;
    Unicode::UnicodeChar valueTextBuffer[6];

    Button selectButton;

    Callback<LegendEntry, const AbstractButton&> onButtonPressed;
    void buttonPressedHandler(const AbstractButton& button);

    GenericCallback<const LegendEntry&>* selectionAction;
};

#endif
